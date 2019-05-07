#include <iostream>
#include <armadillo>
#include <cassert>

using namespace std;

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

double lossFunc(const arma::vec & targets, const arma::mat & pred)
{
	arma::vec diffs = targets - pred;
	arma::vec ret = 0.5 * diffs.t() * diffs / diffs.n_elem;
	return ret(0);
}

arma::vec gradientDescent(const arma::mat & inputs, const arma::vec & targets, const arma::mat & etaM, int epochs = 1000)
{
	assert(epochs > 100);

	int samplesN = inputs.n_cols;

	arma::vec params(inputs.n_rows);
	params.fill(0);
	for (int i = 0; i < epochs; i++)
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

int main()
{
	int samplesN = 1000;
	int paramsN = 3;
	int epochs = 100000;
	SamplesInfo_t samples = generateTrainSamples(samplesN, paramsN);

	arma::vec etas(paramsN);
	etas.fill(0.001);
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

	return 0;
}