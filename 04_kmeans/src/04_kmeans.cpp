#include <iostream>
#include <armadillo>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

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

void getNewCenters(arma::mat & centers, const arma::mat & data, int i, int epochs)
{
	int ele_n = data.n_cols;
	int n_row = centers.n_rows;
	int sampleN = data.n_rows;

	arma::mat newCenters(n_row, ele_n);
	newCenters.fill(0);

	vector<int> tags(sampleN, 0);
	vector<int> addCounter(n_row, 0);

	for (int i = 0; i < sampleN; i++)
	{
		int idx = findClosest(data.row(i), centers);
		tags[i] = idx;
	}

	for (int i = 0; i < n_row; i++)
	{
		for (int j = 0; j < sampleN; j++)
		{
			if (tags[j] == i)
			{
				newCenters.row(i) += data.row(j);
				addCounter[i]++;
			}
		}
	}

	for (int i = 0; i < n_row; i++)
	{
		newCenters.row(i) /= addCounter[i];
	}

	centers = newCenters;

	if (i == epochs)
	{
		for (auto & i : tags)
			printf("%d\n", i);

		ofstream ofs("iris_result_kmeans.txt", ios::out);
		for (int i = 0; i < sampleN; i++)
		{
			for (int j = 0; j < ele_n; j++)
				ofs << data(i, j) << " ";
			ofs << tags[i] << endl;
		}
	}
}

int main()
{
	// kmeans using iris dataset
#if TEST_KMEANS
	{
	// Load iris dataset
	arma::mat irisData;
	irisData.load("iris.txt", arma::raw_ascii);
	irisData.print("iris_data");

	// Generate random centers
	const int classN = 3;
	const int epochs = 10000;
	int samplesN = irisData.n_rows;
	int ele_n    = 4;

	arma::mat centers(classN, ele_n);
	std::srand(std::time(nullptr));
	for (int i = 0; i < classN; i++)
	{
		int randIdx      = std::rand() % samplesN;
		arma::rowvec row = irisData.row(randIdx);
		centers.row(i)   = row.cols(arma::span(0, ele_n - 1));
	}

	// Iterate to calculating samples' centers
	for (int i = 0; i <= epochs; i++)
	{
		getNewCenters(centers, irisData.cols(arma::span(0, 3)), i, epochs);
	}
	}
#endif

	return 0;
}