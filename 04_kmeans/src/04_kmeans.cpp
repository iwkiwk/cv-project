#include <iostream>
#include <armadillo>
#include <cassert>
#include <cmath>

using namespace std;

#define TEST_KMEANS  1

double calcPrecision(const arma::vec & target, const arma::vec & pred)
{
	assert(target.n_elem == pred.n_elem);

	int TP = 0;
	int FP = 0;
	int TN = 0;
	int FN = 0;

	for (int i = 0; i < target.n_elem; i++)
	{
		if (pred(i) == 1)
			if (target(i) == 1)
				TP++;
			else
				FP++;
		else
			if (target(i) == 0)
				TN++;
			else
				FN++;
	}
	if ((TP+FP) == 0)
		return 0;
	return TP*1.0/(TP+FP);
}

double calcRecall(const arma::vec & target, const arma::vec & pred)
{
	assert(target.n_elem == pred.n_elem);

	int TP = 0;
	int FP = 0;
	int TN = 0;
	int FN = 0;

	for (int i = 0; i < target.n_elem; i++)
	{
		if (pred(i) == 1)
			if (target(i) == 1)
				TP++;
			else
				FP++;
		else
			if (target(i) == 0)
				TN++;
			else
				FN++;
	}
	if ((TP+FP) == 0)
		return 0;
	return TP*1.0/(TP+FN);
}

int findClosest(const arma::rowvec & current, const arma::mat & mats)
{
	assert(current.n_elem == mats.n_cols);

	auto calcDis = [](const arma::vec & v1, const arma::vec & v2) {
		int n = v1.n_elem;
		double sum = 0;
		for (int i = 0; i < n; i++)
			sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
		return sum;
	};

	arma::vec dis(mats.n_rows);
	for (int i = 0; i < mats.n_rows; i++)
		dis[i] = calcDis(arma::vectorise(current), arma::vectorise(mats.row(i)));
	
	int ret = 0;
	double minE = dis[0];
	for (int i = 1; i < mats.n_rows; i++)
	{
		if (dis[i] < minE)
		{
			ret = i;
			minE = dis[i];
		}
	}

	return ret;
}

int main()
{
	// kmeans using iris dataset
#if TEST_KMEANS
	{
	arma::mat irisData;
	irisData.load("iris.txt", arma::raw_ascii);

	irisData.print("iris_data");
	}
#endif

	return 0;
}