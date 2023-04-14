/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "detection/detection_method_localmax.h"

namespace fbgsense::detection
{
	void detection_method_localmax::perform(spectrum_state spect, std::initializer_list<parameter_t> params)
	{
		parameter_t relativeIntensity = params.begin()[0];
		spectrum_unit_t max = spect.extremas.max;

		// initialize first element, so we can detect zero crossings while calculating forward differences, in one pass
		spect.derivation[0].value = spect.filtered_intensities[1] - spect.filtered_intensities[0];
		spect.derivation[0].extrema = extrema_type::NONE;

		size_t peakIndex = 1;

		for (int i = 1; i < spect.derivation.size(); i++)
		{
			// forward difference
			spect.derivation[i].value = spect.filtered_intensities[i + 1] - spect.filtered_intensities[i];

			// detect zero crossings
			if (spect.derivation[i].value <= 0 && spect.derivation[i - 1].value > 0 || // from + to - or 0
				spect.derivation[i].value < 0 && spect.derivation[i - 1].value == 0)   // from 0 to -
			{
				spect.derivation[i].extrema = extrema_type::MAXIMUM;

				if (abs(max - spect.intensities[i]) < relativeIntensity) // check distance from global maximum
				{
					// assert(i < m_samples);
					spect.peaks.push_back(peak(i, peakIndex++, spect.channel, spect.wavelengths[i], spect.intensities[i])); // append current peak
				}
			}
			else if (spect.derivation[i].value > 0 && spect.derivation[i - 1].value <= 0 || // from - or 0 to +
				spect.derivation[i].value == 0 && spect.derivation[i - 1].value < 0)   // from - to 0
			{
				spect.derivation[i].extrema = extrema_type::MINIMUM;
			}
			else
			{
				spect.derivation[i].extrema = extrema_type::NONE; // either +/+ or -/- or 0/0
			}
		}
	}
}
