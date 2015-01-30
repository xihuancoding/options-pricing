#include "Binomial.h"
#include "Cdf.h"

#include <cassert>
#include <cfloat>


Binomial::Binomial()
{
}

Binomial::~Binomial()
{
}

double Binomial::callOptionValue(double _spotPrice, double strickPrice, double yearsToExpiry, double _riskFreeInterestRate, double _volatility, double _dividendYield) const {
	assert(_spotPrice >= 0.0);
	assert(strickPrice >= 0.0);
	if (yearsToExpiry < 0.0) yearsToExpiry = 0.0;
	assert(_volatility >= 0.0);

	double riskFreeInterestRate = _riskFreeInterestRate / 100.0;
	double volatility = _volatility / 100.0;
	double dividendYield = _dividendYield / 100.0;
	double spotPrice = _spotPrice * exp((riskFreeInterestRate - dividendYield) * yearsToExpiry);

	int n = 1000;
	double deltaT = yearsToExpiry / n;

	double up = exp(volatility * sqrt(deltaT));
	double down = 1.0 / up;

	double p0 = 0.5;
	double p1 = 1.0 - p0;

	//initial values at time T
	double* p = new double[n + 1];
	for (int i = 0; i <= n; ++i) {
		p[i] = spotPrice * pow(up, 2 * i - n) - strickPrice;
		if (p[i] < 0) p[i] = 0;
	}

	// move to earlier times
	for (int j = n - 1; j >= 0; --j) {
		for (int i = 0; i <= j; ++i) {
			p[i] = p0 * p[i] + p1 * p[i + 1];    // binomial value
			//double exercise = spotPrice * pow(up, 2 * i - j) - strickPrice;  // exercise value
			//if (p[i] < exercise) p[i] = exercise;
			//if (p[i] < 0) p[i] = 0;
		}
	}

	double ret = p[0];
	delete[] p;
	ret *= exp(-riskFreeInterestRate * yearsToExpiry);

	return ret;
}

double Binomial::putOptionValue(double _spotPrice, double strickPrice, double yearsToExpiry, double _riskFreeInterestRate, double _volatility, double _dividendYield) const {
	assert(_spotPrice >= 0.0);
	assert(strickPrice >= 0.0);
	if (yearsToExpiry < 0.0) yearsToExpiry = 0.0;
	assert(_volatility >= 0.0);

	double riskFreeInterestRate = _riskFreeInterestRate / 100.0;
	double volatility = _volatility / 100.0 / 2.0;
	double dividendYield = _dividendYield / 100.0;
	double spotPrice = _spotPrice * exp((riskFreeInterestRate - dividendYield) * yearsToExpiry);

	int n = 1000;
	double deltaT = yearsToExpiry / n;

	double up = exp(volatility * sqrt(deltaT));
	double down = 1.0 / up;

	double p0 = 0.5;
	double p1 = 1.0 - p0;

	//initial values at time T
	double* p = new double[n + 1];
	for (int i = 0; i <= n; ++i) {
		p[i] = strickPrice - spotPrice * pow(up, 2 * i - n);
		if (p[i] < 0) p[i] = 0;
	}

	// move to earlier times
	for (int j = n - 1; j >= 0; --j) {
		for (int i = 0; i <= j; ++i) {
			p[i] = p0 * p[i] + p1 * p[i + 1];    // binomial value
			//double exercise = strickPrice - spotPrice * pow(up, 2 * i - j);  // exercise value
			//if (p[i] < exercise) p[i] = exercise;
			//if (p[i] < 0) p[i] = 0;
		}
	}

	double ret = p[0];
	delete[] p;
	ret *= exp(-riskFreeInterestRate * yearsToExpiry);

	return ret;
}
