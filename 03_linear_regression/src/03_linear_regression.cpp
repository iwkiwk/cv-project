#include <iostream>
#include <armadillo>
#include <cassert>
#include <cmath>

using namespace std;

#define TEST_LINEAR_REGRESSION    0
#define TEST_LOGISTIC_REGRESSION  1

typedef struct {
	arma::mat inputs;
	arma::vec targets;
	arma::vec params;
} SamplesInfo_t;

SamplesInfo_t generateTrainSamples(int samplesNum = 100, int paramsNum = 5, bool withNoise = false)
{
	assert(paramsNum > 1);

	arma::vec params = arma::randu<arma::vec>(paramsNum);
	params = params * 100;

	arma::mat inputs(paramsNum, samplesNum);
	arma::vec targets(samplesNum);

	for (int i = 0; i < samplesNum; i++)
	{
		arma::vec x   = arma::randu<arma::vec>(paramsNum);
		x(paramsNum - 1) = 1;
		arma::vec y;
		if (withNoise)
			y = params.t() * x + arma::randu<arma::vec>(1) / 10;
		else
			y = params.t() * x;
		inputs.col(i) = x;
		targets(i)    = y(0);
	}

	SamplesInfo_t ret;
	ret.inputs  = inputs;
	ret.targets = targets;
	ret.params  = params;

	return ret;
}

arma::vec predict(const arma::vec & params, const arma::mat & inputs)
{
	arma::mat t_ = params.t() * inputs;
	arma::vec ret = arma::vectorise(t_);
	return ret;
}

double lossFunc(const arma::vec & targets, const arma::mat & preds)
{
	arma::vec diffs = targets - preds;
	arma::vec ret = 0.5 * diffs.t() * diffs / diffs.n_elem;
	return ret(0);
}

arma::vec gradientDescent(const arma::mat & inputs, const arma::vec & targets, const arma::mat & etaM, int epochs = 1000)
{
	assert(epochs > 100);

	int samplesN = inputs.n_cols;

	arma::vec params(inputs.n_rows);
	params.fill(0);
	for (int i = 0; i <= epochs; i++)
	{
		arma::vec pred  = predict(params, inputs);
		arma::vec diffs = pred - targets;
		arma::vec t_    = arma::vectorise(diffs.t() * inputs.t());
		t_     /= samplesN;
		params -= etaM * t_;

		if (i % (int)(epochs/100) == 0)
		{
			double loss = lossFunc(targets, pred);
			printf("epoch %d, loss %lf\n", i, loss);
		}
	}

	return params;
}

double logisticLossFunc(const arma::vec & targets, const arma::vec & preds)
{
	assert(targets.n_rows == preds.n_rows);
	assert(targets.n_cols == 1);
	assert(preds.n_cols == 1);

	int samplesN = targets.n_rows;
	arma::vec loss = -1 * (targets.t() * arma::log10(preds) + (1 - targets.t()) * arma::log10(1 - preds)) / samplesN;
	return loss(0);
}

double logisticPredict(const arma::vec & params, const arma::vec & input)
{
	arma::vec t_ = params.t() * input;
	double ret = 1.0 / (1 + std::exp(-1.0*arma::as_scalar(t_)));
	return ret;
}

void logisticGD(const arma::mat & inputs, arma::vec & params, const arma::mat & etaM, int epochs, const char * resultsFile)
{
	assert(epochs > 100);

	int samplesN = inputs.n_rows;
	int paramsN  = inputs.n_cols;

	arma::vec targets = arma::vectorise(inputs.col(paramsN - 1));

	for (int i = 0; i <= epochs; i++)
	{
		arma::vec sum(arma::zeros<arma::vec>(paramsN));
		arma::vec preds(samplesN);
		for (int sampleIdx = 0; sampleIdx < samplesN; sampleIdx++)
		{
			arma::vec input(paramsN);
			input = arma::vectorise(inputs.row(sampleIdx));
			input(paramsN-1) = 1;
			double pred      = logisticPredict(params, input);
			arma::vec t_     = (pred - targets(sampleIdx)) * input;
			sum += t_;

			preds(sampleIdx) = pred;
		}
		sum /= samplesN;
		params -= etaM * sum;

		if (i % (int)(epochs/100) == 0)
		{
			double loss = logisticLossFunc(targets, preds);
			//targets.print("targets");
			//preds.print("preds");
			printf("epoch %d, loss %lf\n", i, loss);
		}

		if (i == epochs)
		{
			//targets.print("targets");
			preds = arma::round(preds);
			//preds.print("preds");

			arma::mat results(samplesN, 2);
			results.col(0) = targets;
			results.col(1) = preds;
			results.save(resultsFile, arma::raw_ascii);
		}
	}
}

int main()
{
#if TEST_LINEAR_REGRESSION
	{
	int samplesN = 1000;
	int paramsN = 3;
	int epochs = 100000;
	SamplesInfo_t samples = generateTrainSamples(samplesN, paramsN);

	arma::vec etas(paramsN);
	etas.fill(0.005);
	arma::mat etaM(paramsN, paramsN);
	etaM.fill(0);
	for (int i = 0; i < paramsN; i++)
		etaM(i, i) = etas[i];
	//etaM.print("etaM:");

	// Normal equations
	arma::vec paramsNormal = arma::trans(arma::inv(samples.inputs.t() * samples.inputs) * samples.inputs.t()) * samples.targets;

	// Gradient descent
	arma::vec paramsGD = gradientDescent(samples.inputs, samples.targets, etaM, epochs);

	//samples.inputs.print("inputs");
	//samples.targets.print("targets");
	samples.params.print("origin params");
	paramsNormal.print("normal result");
	paramsGD.print("GD params");
	}
#endif

	// Logistic regression using iris dataset
#if TEST_LOGISTIC_REGRESSION
	{
	arma::mat irisData;
	irisData.load("iris.txt", arma::raw_ascii);

	int paramsN = irisData.n_cols - 2;
	arma::vec params(paramsN);
	int epochs = 10000;

	arma::vec etas(paramsN);
	etas.fill(0.01);
	arma::mat etaM(paramsN, paramsN);
	etaM.fill(0);
	for (int i = 0; i < paramsN; i++)
		etaM(i, i) = etas[i];

	arma::mat dataM(irisData.n_rows, paramsN);

	params = arma::randu<arma::vec>(paramsN);
	dataM.cols(arma::span(0, 3)) = irisData.cols(arma::span(0, 3));
	dataM.col(4) = irisData.col(4);
	dataM = arma::shuffle(dataM); // shuffle data matrix
	logisticGD(dataM, params, etaM, epochs, "iris_results1.txt"); // 1st class

	params = arma::randu<arma::vec>(paramsN);
	dataM.cols(arma::span(0, 3)) = irisData.cols(arma::span(0, 3));
	dataM.col(4) = irisData.col(5);
	dataM = arma::shuffle(dataM);
	logisticGD(dataM, params, etaM, epochs, "iris_results2.txt"); // 2nd class

	params = arma::randu<arma::vec>(paramsN);
	dataM.cols(arma::span(0, 3)) = irisData.cols(arma::span(0, 3));
	dataM.col(4) = irisData.col(6);
	dataM = arma::shuffle(dataM);
	logisticGD(dataM, params, etaM, epochs, "iris_results3.txt"); // 3rd class
	}
#endif

	return 0;
}