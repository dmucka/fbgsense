/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "filtering/filter_fft.h"

namespace fbgsense::filtering
{
	void filter_fft::perform(spectrum_state spect, std::initializer_list<parameter_t> params)
	{
		parameter_t peakWidth = params.begin()[0];

		if (peakWidth == 0) // we are not filtering anything
		{
			std::copy(spect.intensities, spect.intensities + spect.samples, spect.filtered_intensities);
			return;
		}

		fftwf_plan forward = fftwf_plan_dft_r2c_1d(
			spect.samples,
			spect.intensities,
			spect.frequencies.data(),
			FFTW_ESTIMATE);

		// get frequency spectrum
		fftwf_execute(forward);

		const parameter_t maxFreq = (spect.samples / 2) / peakWidth;
		const parameter_t filteringWindow = 200;
		const parameter_t filterStop = maxFreq + filteringWindow / 2;

		// filter using Hanning window
		for (int i = maxFreq; i < filterStop && i < spect.frequencies.size(); i++)
		{
			spectrum_unit_t factor = 0.5 + 0.5 * cos((2 * std::numbers::pi / filteringWindow) * (i - maxFreq + 1));
			spect.frequencies[i][0] *= factor;
			spect.frequencies[i][1] *= factor;
		}

		for (int i = filterStop; i < spect.frequencies.size(); i++)
		{
			spect.frequencies[i][0] = 0;
			spect.frequencies[i][1] = 0;
		}

		fftwf_plan backward = fftwf_plan_dft_c2r_1d(
			spect.samples,
			spect.frequencies.data(),
			spect.filtered_intensities,
			FFTW_ESTIMATE);

		fftwf_execute(backward);

		// fix scaling
		for (size_t i = 0; i < spect.samples; i++)
		{
			spect.filtered_intensities[i] /= spect.samples;
		}

		fftwf_destroy_plan(forward);
		fftwf_destroy_plan(backward);
	}
}
