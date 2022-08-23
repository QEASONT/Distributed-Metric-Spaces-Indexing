/*********************************************************************
*                                                                    *
* Copyright (c)                                                      *
* ZJU-DBL, Zhejiang University, Hangzhou, China                      *
*                                                                    *
* All Rights Reserved.                                               *
*                                                                    *
* Permission to use, copy, and distribute this software and its      *
* documentation for NON-COMMERCIAL purposes and without fee is       *
* hereby granted provided  that this copyright notice appears in     *
* all copies.                                                        *
*                                                                    *
* THE AUTHORS MAKE NO REPRESENTATIONS OR WARRANTIES ABOUT THE        *
* SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING  *
* BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. THE AUTHOR  *
* SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A      *
* RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS    *
* DERIVATIVES.                                                       *
*                                                                    *
*********************************************************************/

#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <sys/stat.h>
#include<sys/timeb.h>
#include<random>
using namespace std;
typedef unsigned long long ull;

#include "./blockfile/blk_file.h"
#include "./blockfile/cache.h"
#include "./gadget/gadget.h"
#include "./heap/binheap.h"
#include "./heap/heap.h"
#include "./rand/rand.h"
#include "./object.h"
#include "./RAF.h"
//#include "xoshiro.h"
#include "./pb-tree.h"
#include <string>
double compdists = 0;
double IOread = 0;
double IOwrite = 0;
double cc = 0;
int MAXINT, BITS;
double EPS, MAXDIST;
int bolcksize;
int dim, num_obj, func;
float** proj;
// Note that the id of data in the file should begin with 0
Object** readobjects(char* filename, int num_obj, int dim)
{
	FILE* fp;

	int record_count = 0;
	Object** objset = new Object * [num_obj];
	for (int i = 0; i < num_obj; i++)
	{
		objset[i] = new Object();
	}

	if ((fp = fopen(filename, "r")) == NULL)
	{
		for (int i = 0; i < num_obj; i++)
			delete objset[i];
		error("Could not open the data file", TRUE);
	}
	else
	{
		fscanf(fp, "%d %d %d", &dim, &num_obj, &func);
		while (record_count < num_obj)
		{
			float d;
			objset[record_count]->id = record_count;
			objset[record_count]->size = dim;
			objset[record_count]->data = new float[objset[record_count]->size];
			for (int i = 0; i < objset[record_count]->size; i++)
			{
				fscanf(fp, "%f", &d);
				objset[record_count]->data[i] = d;
			}
			fscanf(fp, "\n");
			record_count++;
		}
	}

	return objset;
}

Object* readobjects2(char* filename)
{
	FILE* fp;
	int record_count = 0;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		error("Could not open the data file", TRUE);
		return NULL;
	}
	else
	{
		fscanf(fp, "%d %d %d", &dim, &num_obj, &func);

		Object* objset = new Object[num_obj];
		while (record_count < num_obj)
		{
			float d;
			objset[record_count].id = record_count;
			objset[record_count].size = dim;
			objset[record_count].data = new float[dim];
			for (int i = 0; i < dim; i++)
			{
				fscanf(fp, "%f", &d);
				objset[record_count].data[i] = d;
			}
			fscanf(fp, "\n");
			record_count++;
		}
		return objset;
	}

}
float** Find_Project_(int size, ull dim, int m) {


	normal_distribution<double> rnorm(0, 1);
	float** projs = new float* [m];
	std::default_random_engine generator;
	for (ull i = 0; i < m; i++)
	{
		projs[i] = new float[dim];
		for (ull j = 0; j < dim; j++)
		{
			projs[i][j] = rnorm(generator);
		}

	}
	return projs;
}
double cal_inner_product(float* v1, float* v2, int dim)
{
	double res = 0.0;
	for (int i = 0; i < dim; ++i) {
		res += v1[i] * v2[i];
	}
	return res;
}
clock_t bulidIndex(char* index_name, char* filename, char* pname, int n_p)
{
	/*******
	index_name -- the path of the index to be stored
	filename -- the path of the dataset
	pname -- the path of the pivot file
	n_p -- the number of pivots
	*******/

	//pivot selection    
	PB_Tree* pb = new PB_Tree();

	//parameter settings	
	pb->eps = EPS;
	pb->num_cand = 40; // the number of candidates for selecting pivots (if we read the pivots from pivot file, it is useless here)
	pb->num_piv = n_p;
	pb->bits = BITS;
	int keysize;
	Object* os = readobjects2(filename); // the function needed to be rewirte to read differnt formats of the dataset
	proj = Find_Project_(os->size, os[0].size, 16);


	vector<float>tmp;
	for (ull i = 0; i < os->size; i++)
	{
		for (ull j = 0; j < 16; j++)
		{
			if (!j)
			{
				tmp.clear();
			}
			tmp.push_back(cal_inner_product(os[i].data, proj[j], os[i].size));
		}
		memcpy(os[i].data, &tmp[0], tmp.size() * sizeof(float));
		os[i].size = 16;
	}
	//cout << i << endl;



	pb->readptable(pname);

	IOread = IOwrite = compdists = 0;
	clock_t begin;

	begin = clock();
	pb->H_bulkload(os, num_obj);
	keysize = os[0].keysize;
	delete[] os;
	cout << "keysize:" << keysize << endl;
	pb->bplus = new B_Tree();
	pb->bplus->init(index_name, bolcksize, NULL, keysize);
	pb->bplus->bulkload("bulkload.txt");

	////build RAF
	pb->bplus->load_root();
	B_Node* node = pb->bplus->root_ptr;
	B_Node* temp;

	while (node->level != 0)
	{
		node = node->entries[0]->get_son();
	}
	int* obj_order = new int[num_obj];
	int k = 0;
	for (int i = 0; i < node->num_entries; i++)
	{
		obj_order[k] = node->entries[i]->son;

		k++;
	}
	temp = node->get_right_sibling();
	//delete node;
	while (temp != NULL)
	{

		node = temp;
		for (int i = 0; i < node->num_entries; i++)
		{
			obj_order[k] = node->entries[i]->son;
			k++;
		}
		temp = node->get_right_sibling();
		delete node;
	}

	pb->draf = new RAF();
	pb->draf->num_obj = num_obj;
	pb->draf->init(index_name, bolcksize, NULL);

	Object** objS = readobjects(filename, num_obj, dim);
	int* result = pb->draf->buid_from_array(objS, obj_order);

	//delete object sets
	for (int i = 0; i < num_obj; i++)
		delete objS[i];
	delete[] obj_order;

	node = pb->bplus->root_ptr;


	while (node->level != 0)
	{
		node = node->entries[0]->get_son();
	}

	k = 0;
	for (int i = 0; i < node->num_entries; i++)
	{
		node->entries[i]->ptr = result[k];
		k++;
	}
	char* buffer = new char[bolcksize];
	node->write_to_buffer(buffer);

	pb->bplus->file->write_block(buffer, node->block);
	delete buffer;

	temp = node->get_right_sibling();
	//delete node;
	while (temp != NULL)
	{
		buffer = new char[bolcksize];
		node = temp;
		for (int i = 0; i < node->num_entries; i++)
		{
			node->entries[i]->ptr = result[k];
			k++;
		}
		node->write_to_buffer(buffer);

		pb->bplus->file->write_block(buffer, node->block);

		delete buffer;
		temp = node->get_right_sibling();
		delete node;
	}
	delete[] result;

	pb->bplus->close();
	return begin;
}

#define ARRAYLENGTH 100



int* build_MBR(B_Node* node, PB_Tree* pb)
{
	int* minp = new int[pb->num_piv];
	int* maxp = new int[pb->num_piv];
	for (int i = 0; i < pb->num_piv; i++)
	{
		minp[i] = MAXINT;
		maxp[i] = 0;
	}

	if (node->level == 0)
	{
		for (int i = 0; i < node->num_entries; i++)
		{
			int* t = pb->R_Zconvert(node->entries[i]->key);
			for (int j = 0; j < pb->num_piv; j++)
			{
				if (t[j] < minp[j])
					minp[j] = t[j];
				if (t[j] > maxp[j])
					maxp[j] = t[j];
			}
			delete[] t;
		}

	}
	else
	{
		for (int i = 0; i < node->num_entries; i++)
		{
			int* t = build_MBR(node->entries[i]->get_son(), pb);
			node->entries[i]->del_son();
			int* t1 = new int[pb->num_piv];
			for (int j = 0; j < pb->num_piv; j++)
			{
				t1[j] = t[pb->num_piv + j];
			}
			for (int j = 0; j < pb->num_piv; j++)
			{
				if (t[j] < minp[j])
					minp[j] = t[j];
				if (t1[j] > maxp[j])
					maxp[j] = t1[j];
			}

			node->entries[i]->min = pb->Zconvert(t);
			node->entries[i]->max = pb->Zconvert(t1);

			delete[]  t;
			delete[] t1;
		}

		char* buffer = new char[bolcksize];
		node->write_to_buffer(buffer);

		pb->bplus->file->write_block(buffer, node->block);
		delete buffer;


	}
	int* temp = new int[2 * pb->num_piv];
	for (int i = 0; i < pb->num_piv; i++)
	{
		temp[i] = minp[i];
		temp[pb->num_piv + i] = maxp[i];
	}

	delete[] minp;
	delete[] maxp;
	return temp;
}

int* H_build_MBR(B_Node* node, PB_Tree* pb)
{
	int* minp = new int[pb->num_piv];
	int* maxp = new int[pb->num_piv];
	for (int i = 0; i < pb->num_piv; i++)
	{
		minp[i] = MAXINT;
		maxp[i] = 0;
	}

	if (node->level == 0)
	{
		for (int i = 0; i < node->num_entries; i++)
		{

			unsigned* key = new unsigned[pb->num_piv];
			unsigned* t = new unsigned[pb->num_piv];
			for (int j = 0; j < pb->bplus->keysize; j++)
			{
				t[j] = 0;
				key[pb->num_piv + j - pb->bplus->keysize] = node->entries[i]->key[j];
			}
			for (int j = pb->bplus->keysize; j < pb->num_piv; j++)
			{
				t[j] = 0;
				key[j - pb->bplus->keysize] = 0;
			}
			pb->R_Hconvert(t, key, pb->num_piv);

			for (int j = 0; j < pb->num_piv; j++)
			{
				if (t[j] < minp[j])
					minp[j] = t[j];
				if (t[j] > maxp[j])
					maxp[j] = t[j];
			}
			delete[] t;
			delete[] key;
		}

	}
	else
	{
		for (int i = 0; i < node->num_entries; i++)
		{
			int* t = H_build_MBR(node->entries[i]->get_son(), pb);
			node->entries[i]->del_son();
			int* t1 = new int[pb->num_piv];
			for (int j = 0; j < pb->num_piv; j++)
			{
				t1[j] = t[pb->num_piv + j];
			}
			for (int j = 0; j < pb->num_piv; j++)
			{
				if (t[j] < minp[j])
					minp[j] = t[j];
				if (t1[j] > maxp[j])
					maxp[j] = t1[j];
			}

			unsigned* mi = new unsigned[pb->num_piv];
			unsigned* ma = new unsigned[pb->num_piv];
			pb->Hconvert(mi, (unsigned*)t, pb->num_piv);
			pb->Hconvert(ma, (unsigned*)t1, pb->num_piv);
			node->entries[i]->min = new unsigned[pb->bplus->keysize];
			node->entries[i]->max = new unsigned[pb->bplus->keysize];
			for (int j = 0; j < pb->bplus->keysize; j++)
			{
				node->entries[i]->min[j] = mi[j + pb->num_piv - pb->bplus->keysize];
				node->entries[i]->max[j] = ma[j + pb->num_piv - pb->bplus->keysize];
			}

			delete[] mi;
			delete[] ma;
			delete[]  t;
			delete[] t1;
		}

		char* buffer = new char[bolcksize];
		node->write_to_buffer(buffer);

		pb->bplus->file->write_block(buffer, node->block);
		delete buffer;

	}

	int* temp = new int[2 * pb->num_piv];
	for (int i = 0; i < pb->num_piv; i++)
	{
		temp[i] = minp[i];
		temp[pb->num_piv + i] = maxp[i];
	}

	delete[] minp;
	delete[] maxp;
	return temp;
}

void main(int argc, char** argv)
{
	timeb t_s, t_e;


	//******************************build the index***********
	clock_t begin, buildEnd, queryEnd;
	double buildComp, queryComp;
	struct stat sdata1;
	struct stat sdata2;

	int buffer_size = 32;

	//char * datafile = argv[1];// the path of input data file
	//char *pname = argv[2];// the path of input pivots
	//char * indexfile = argv[3];// the path to store the built index		
	//FILE * f = fopen(argv[4], "w"); // the path to store the building and query cost
	//MAXDIST = atof(argv[5]);// the maximum distance for the input dataset
	char* datafile = "D:\\Code\\Metric_Space_Indexing\\SPBTree\\dataset\\d200s100000.txt";// the path of input data file
	char* pname = "D:\\Code\\Metric_Space_Indexing\\SPBTree\\dataset\\d200s100000_pivot.txt";// the path of input pivots
	char* indexfile = "D:\\Code\\Metric_Space_Indexing\\SPBTree\\dataset\\d200s100000_index";// the path to store the built index		
	FILE* f = fopen("D:\\Code\\Metric_Space_Indexing\\SPBTree\\dataset\\d200s100000_cost.txt", "w"); // the path to store the building and query cost
	MAXDIST = atof("25000");// the maximum distance for the input dataset
	EPS = MAXDIST / 1000;
	MAXINT = (MAXDIST / EPS);
	BITS = ((int)log2(MAXINT) + 1); //  the bits to represent space filling curve values

	double radius[7];
	int kvalues[] = { 1, 5, 10, 20, 50, 100 };

	if (string(datafile).find("LA") != -1) {
		double r[] = { 473, 692, 989, 1409, 1875, 2314, 3096 };
		memcpy(radius, r, sizeof(r));
	}
	else if (string(datafile).find("integer") != -1) {
		double r[] = { 2321,2733, 3229,3843, 4614, 5613, 7090 };
		memcpy(radius, r, sizeof(r));
	}
	else if (string(datafile).find("sf3.txt") != -1) {
		double r[] = { 100, 200, 300, 400, 500, 600, 700 };
		memcpy(radius, r, sizeof(r));
	}
	else if (string(datafile).find("mpeg") != -1) {

		double r[] = { 3838, 4092, 4399, 4773, 5241, 5904, 7104 };
		memcpy(radius, r, sizeof(r));
	}

	//int pn = atoi(argv[6]);// the number of pivots
	//bolcksize = atoi(argv[7]);	// the page size
	//char* querydata = argv[8];// the path of input query data
	int pn = atoi("20");// the number of pivots
	bolcksize = atoi("4096");	// the page size
	char* querydata = "D:\\Code\\Metric_Space_Indexing\\SPBTree\\dataset\\d200s100000_query.txt";// the path of input query data
	fprintf(f, "pivotnum: %d\n", pn);
	compdists = 0;
	IOread = IOwrite = 0;
	begin = bulidIndex(indexfile, datafile, pname, pn);

	PB_Tree* pb = new PB_Tree();

	Cache* c = new Cache(buffer_size, bolcksize);
	pb->c = c;

	pb->num_piv = pn;
	pb->readptable(pname);
	pb->eps = EPS;
	pb->bits = BITS;

	pb->bplus = new B_Tree();
	pb->bplus->init_restore(indexfile, NULL);
	pb->bplus->load_root();
	H_build_MBR(pb->bplus->root_ptr, pb);
	buildEnd = clock() - begin;
	buildComp = compdists;
	fprintf(f, "building...\n");
	fprintf(f, "finished... %f build time\n", (double)buildEnd / CLOCKS_PER_SEC);
	fprintf(f, "finished... %f distances computed\n", buildComp);
	fprintf(f, "finished... %f IO times\n", IOread + IOwrite);

	char* bfile = new char[strlen(indexfile) + 2];
	strcpy(bfile, indexfile);
	strcat(bfile, ".b");
	char* raffile = new char[strlen(indexfile) + 4];
	strcpy(raffile, indexfile);
	strcat(raffile, ".raf");
	stat(bfile, &sdata1);
	stat(raffile, &sdata2);
	fprintf(f, "saved... %lli bytes\n", (long long)(sdata1.st_size + sdata2.st_size));
	fflush(f);
	//************end of build index*************************

	//************************ similarity searh***********************
	fprintf(f, "\nquerying...\n");

	pb->draf = new RAF();
	pb->draf->init_restore(indexfile, c);


	Object* q = new Object();
	ifstream in;
	double io = 0;
	double dists = 0;
	int qcount = 50; // the number of quereis
	q->size = dim;   // the dimension of the query object
	q->data = new float[q->size];
	double rad;
	int querycount = 0;
	cout << "start knnSearching......" << endl;
	for (int k = 0; k < 6; k++) {

		in.open(querydata);
		begin = clock();
		IOread = IOwrite = 0;
		dists = 0;
		rad = 0;
		double pf = 0;

		for (int j = 0; j < qcount; j++)
		{
			ftime(&t_s);
			querycount++;
			c->clear();
			compdists = 0;

			for (int i = 0; i < q->size; i++)
			{
				in >> q->data[i];
			}
			vector<float>tmp;

			for (ull j = 0; j < 16; j++)
			{
				if (!j)
				{
					tmp.clear();
				}
				tmp.push_back(cal_inner_product(q->data, proj[j], 16));
			}
			memcpy(q->data, &tmp[0], tmp.size() * sizeof(float));
			q->size = 16;


			rad += pb->BFkNN(q, kvalues[k]); //kNN query function
			ftime(&t_e);
			//cout << "knnSearch single: " << ((t_s.time - t_e.time) * 1000 + t_s.millitm - t_e.millitm)  << "ms" << endl;
			pf += c->page_faults;
			dists += compdists;
		}
		queryEnd = clock() - begin;
		queryComp = dists;

		cout << "knnSearching: Average" << ((t_s.time - t_e.time) * 1000 + t_s.millitm - t_e.millitm) / qcount << "ms" << endl;

		fprintf(f, "k: %d\n", kvalues[k]);
		fprintf(f, "finished... %f query time\n", (double)queryEnd / CLOCKS_PER_SEC / qcount);
		fprintf(f, "finished... %f distances computed\n", queryComp / qcount);
		fprintf(f, "finished... %f IO times\n", IOread / qcount);
		fprintf(f, "finished... %f radius\n", rad / qcount);
		in.close();
	}
	cout << "start rangeSearching......" << endl;
	for (int k = 0; k < 7; ++k) {
		in.open(querydata);
		begin = clock();
		IOread = IOwrite = 0;
		dists = 0;
		rad = 0;
		double pf = 0;
		ftime(&t_s);
		for (int j = 0; j < qcount; j++)
		{

			c->clear();
			compdists = 0;

			for (int i = 0; i < q->size; i++)
			{
				in >> q->data[i];
			}

			rad += pb->ImprovedRQ_new(q, radius[k]);  // range query function

			pf += c->page_faults;
			dists += compdists;
		}
		ftime(&t_e);
		cout << "knnSearching: " << ((t_s.time - t_e.time) * 1000 + t_s.millitm - t_e.millitm) / 1000.0 << "s" << endl;
		queryEnd = clock() - begin;
		queryComp = dists;
		fprintf(f, "r: %f\n", radius[k]);
		fprintf(f, "finished... %f query time\n", (double)queryEnd / CLOCKS_PER_SEC / qcount);
		fprintf(f, "finished... %f distances computed\n", queryComp / qcount);
		fprintf(f, "finished... %f IO times\n", IOread / qcount);

		fprintf(f, "finished... %f objs\n", rad / qcount);
		in.close();
	}
	delete q;
	q = NULL;
	pb = NULL;
	fprintf(f, "\n");

	//***********************end of similarity search*******************
}
