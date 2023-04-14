/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

inline
double gaussian(double x, double scale, double mu, double sigma)
{
	const double z = (x - mu) / sigma;
	return scale * std::exp(-0.5 * z * z);
}


inline
double linear(double x, double intercept, double slope)
{
	return slope * x + intercept;
}

inline
double bigaussian(double x, double scale, double mu, double sigma1, double sigma2)
{
	double currentSigma;

	if (x < mu)
		currentSigma = sigma1;
	else
		currentSigma = sigma2;

	const double z = (x - mu) / currentSigma;
	return scale * std::exp(-0.5 * z * z);
}
