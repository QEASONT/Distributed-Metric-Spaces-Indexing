#pragma once
struct Data
{
	// Dimension of data
	unsigned dim = 0;
	// Number of data
	unsigned N = 0;
	// Data matrix
	double** val = nullptr;
	double** query=nullptr; // NO MORE THAN 200 POINTS
};

struct Ben
{
	unsigned N = 0;
	unsigned num = 0;
	int** indice = nullptr;
	double** dist = nullptr;
};

struct HashParam
{
	// the value of a in S hash functions
	double** rndAs1 = nullptr;
	// the value of a in S hash functions
	double** rndAs2 = nullptr;

};



