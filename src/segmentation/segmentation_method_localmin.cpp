/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "segmentation/segmentation_method_localmin.h"

namespace fbgsense::segmentation
{
	std::vector<region_t> segmentation_method_localmin::perform(spectrum_state spect, std::initializer_list<parameter_t> params)
	{
		parameter_t relativeIntensity = params.begin()[0];

		std::vector<bool> excludedPeaks(spect.peaks.size(), false);
		std::vector<peak> suppressedPeaks;
		std::vector<region_t> regions;

		for (size_t i = 0; i < spect.peaks.size(); i++)
		{
			if (excludedPeaks[i])
				continue;

			size_t position = spect.peaks[i].position();
			size_t leftGuard{ 0 }, rightGuard{ spect.derivation.size() };

			spectrum_unit_t threshold = spect.extremas.max - relativeIntensity;

			// get left guard
			for (int z = position - 1; z >= 0; z--)
			{
				if (spect.derivation[z].extrema == extrema_type::MINIMUM && spect.intensities[z] <= threshold)
				{
					leftGuard = z;
					break;
				}
			}

			// get right guard
			for (int z = position + 1; z < spect.derivation.size(); z++)
			{
				if (spect.derivation[z].extrema == extrema_type::MINIMUM && spect.intensities[z] <= threshold)
				{
					rightGuard = z;
					break;
				}
			}

			// check if the range from A to B contains multiple peaks
			std::vector<peak> localPeaks;
			for (int c = 0; c < spect.peaks.size(); c++)
			{
				if (spect.peaks[c].position() >= leftGuard && spect.peaks[c].position() <= rightGuard)
					localPeaks.push_back(spect.peaks[c]);
			}

			region_t currentRegion = { (int)leftGuard, (int)rightGuard, localPeaks };
			regions.push_back(currentRegion);

			// if there are multiple peaks, they are sharing the same region, so we can skip them
			if (localPeaks.size() > 1)
			{
				for (int i = 0; i < localPeaks.size(); i++)
				{
					excludedPeaks[localPeaks[i].index() - 1] = true;
				}
			}
		}

		return regions;
	}
}
