/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "filtering/filter_savgol.h"

namespace fbgsense::filtering
{

	void filter_savgol::perform(spectrum_state spect, std::initializer_list<parameter_t> params)
	{
		parameter_t windowSize = params.begin()[0];

		// This savgol implementation works only on odd window sizes
		if (windowSize % 2 == 0)
			++windowSize;

		parameter_t m = (int)((windowSize / 2.0) + 0.5) - 1; // m - the full window length is 2m+1
		parameter_t polynomialOrder = 3; // n
		parameter_t derivationOrder = 0; // s
		parameter_t timeInterval = 0; // t

		if (windowSize < 2) // we are not filtering anything
		{
			std::copy(spect.intensities, spect.intensities + spect.samples, spect.filtered_intensities);
			return;
		}

		auto savgolFilter = savgol::SavitzkyGolayFilter(m, timeInterval, polynomialOrder, derivationOrder);

		spectrum_unit_t* start = &spect.intensities[0];
		spectrum_unit_t* mid = &spect.filtered_intensities[m];
		size_t maxLength = spect.samples - windowSize;

		for (size_t i = 0; i <= maxLength; ++i)
		{
			std::span<spectrum_unit_t> window = std::span<spectrum_unit_t>(start, windowSize);
			*mid = savgolFilter.filter(window);

			++start;
			++mid;
		}

		// Savgol creates empty space around the edges. We will use the mirroring technique to fix this issue.
		for (size_t i = 0; i < m; ++i)
		{
			spect.filtered_intensities[i] = spect.filtered_intensities[(2 * m) - i];
		}

		for (size_t i = 1; i <= m; ++i)
		{
			spect.filtered_intensities[maxLength + m + i] = spect.filtered_intensities[maxLength + m - i];
		}
	}
}
