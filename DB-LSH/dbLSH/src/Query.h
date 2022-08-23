#pragma once
#include "StructType.h"
#include "Preprocess.h"
#include "dblsh.h"
#include <vector>
#include <queue>

class Query
{
private:
	// the parameter "c" in "c-ANN"
	double c;
	//which chunk is accessed
	//int chunks;

	//double R_min = 4500.0f;//mnist
	//double R_min = 1.0f;
	double init_w = 1.0f;

	double* query_point;
	//std::vector<double> q_val;
	// the hash value of query point
	double* hashval;
	//std::vector<double> total_score;

	double** mydata;
	int dim;

	//double inp_LB;
	// Set of points sifted
	//std::vector<unsigned> candidate;

	//std::vector<int> keys;

public:
	// k-NN
	unsigned k;
	// Indice of query point in dataset. Be equal to -1 if the query point isn't in the dataset.
	unsigned flag;
	
	double beta = 0;

	unsigned cost = 0;

	//#access;
	int num_access = 0;
	//
	unsigned rounds = 0;
	//cost of each partition
	//std::vector<unsigned> costs;
	//
	double time_total = 0;
	//
	double time_hash = 0;
	//
	double time_sift = 0;

	double time_verify = 0;
	// query result:<indice of ANN,distance of ANN>
	std::vector<Res> res;

	void cal_hash(Hash& hash, Preprocess& prep);
	void sift(Hash& hash, Preprocess& prep);
	//void knn(std::priority_queue<Res>& res_PQ,
	//	Hash& hash, Preprocess& prep,
	//	std::vector<int>** table,
	//	std::vector<bool>& flag, int& num_res);
public:
	Query(unsigned id, double c_, unsigned k_, Hash& hash, Preprocess& prep, double beta);

	~Query();
};

class Performance
{
public:
	//#access_nodes
	long long num_access_in_RTree = 0;
	//cost
	unsigned cost = 0;
	//the average rounds of (r,c)-BC query for any point
	unsigned rounds = 0;
	//
	std::vector<unsigned> costs;
	// times of query
	unsigned num = 0;
	//
	double time_total = 0;
	//
	double time_hash = 0;
	//
	double time_sift = 0;
	//
	double time_verify = 0;
	//number of exact NN
	unsigned NN_num = 0;
	//number of results
	unsigned res_num = 0;
	//
	double ratio = 0;
public:
	Performance() {}
	//ccccc
	void update(Query& query, Preprocess& prep);
	~Performance();
};
