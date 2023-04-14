/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include <fftw3.h>
#include <curve_fit.hpp>

#include "spectrum/spectrum.h"

spectrum::spectrum()
{
}

// todo: doxygen documentation
// todo: composite all remaining computation steps
// todo: use unique_ptrs everywhere insted of raw pointers
// todo: each computational step should init and free its own resources. currently, this is all hardcoded in spectrum class.
// todo: use adaptive polynomial filter for savgol, to minimalize distortion
spectrum::spectrum(size_t storage, size_t samples) : m_storage(storage), m_samples(samples)
{
	set_filter(filter_type::FFT);
	set_detection_method(detection_type::LOCAL_MAXIMA);
	set_segmentation_method(segment_type::LOCAL_MINIMA);

	m_wavelengths = new spectrum_unit_t[storage];
	m_indices = new spectrum_unit_t[storage];

	for (int i = 0; i < storage; i++)
	{
		m_indices[i] = i;
	}

	size_t m_freq_size = m_samples / 2 + 1;
	size_t m_derivation_size = m_samples - 1;

	for (int i = 0; i < spectrum_max_channels; i++)
	{
		spectrum_unit_t* m_intensities_alloc = fftwf_alloc_real(storage);
		m_intensities[i] = m_intensities_alloc;

		m_filtered_intensities[i] = new spectrum_unit_t[samples]{ 0 };

		derivation_t* m_derivation_alloc = new derivation_t[m_derivation_size];
		m_derivation[i] = std::span<derivation_t>(m_derivation_alloc, m_derivation_size);

		complex_t* m_freq_alloc = fftwf_alloc_complex(m_freq_size);
		m_freq[i] = std::span<complex_t>(m_freq_alloc, m_freq_size);
	}
}

spectrum::spectrum(size_t samples) : spectrum(samples, samples)
{
}

spectrum::~spectrum()
{
	delete[] m_wavelengths;
	delete[] m_indices;

	for (size_t i = 0; i < spectrum_max_channels; i++)
	{
		if (m_intensities[i] == m_filtered_intensities[i])
		{
			fftwf_free(m_intensities[i]);
		}
		else
		{
			fftwf_free(m_intensities[i]);
			delete[] m_filtered_intensities[i];
		}

		delete[] m_derivation[i].data();
		fftwf_free(m_freq[i].data());
	}
}

void spectrum::set_filter(filter_type type)
{
	if (type == filter_type::FFT)
		set_filter(fbgsense::filtering::filter_fft());
	else if (type == filter_type::SAVGOL)
		set_filter(fbgsense::filtering::filter_savgol());
}

void spectrum::set_filter(processor<fbgsense::filtering::filter>&& type)
{
	if (m_filter != nullptr && type.get_type() == m_filter->get_type())
		return;

	m_filter = type.create();
}

void spectrum::set_detection_method(detection_type type)
{
	if (type == detection_type::LOCAL_MAXIMA)
		set_detection_method(fbgsense::detection::detection_method_localmax());
}

void spectrum::set_detection_method(processor<fbgsense::detection::detection_method>&& type)
{
	if (m_detection_method != nullptr && type.get_type() == m_detection_method->get_type())
		return;

	m_detection_method = type.create();
}

void spectrum::set_segmentation_method(segment_type type)
{
	if (type == segment_type::LOCAL_MINIMA)
		set_detection_method(fbgsense::segmentation::segmentation_method_localmin());
	else if (type == segment_type::INFLECTION_POINTS)
		set_detection_method(fbgsense::segmentation::segmentation_method_inflection());
}

void spectrum::set_detection_method(processor<fbgsense::segmentation::segmentation_method>&& type)
{
	if (m_segmentation_method != nullptr && type.get_type() == m_segmentation_method->get_type())
		return;

	m_segmentation_method = type.create();
}

spectrum_state spectrum::get_spectrum_state(channel_t channel)
{
	size_t channel_index = to_index(channel);

	spectrum_state state
	{
		.channel = channel,
		.samples = m_samples,
		.wavelengths = m_wavelengths,
		.intensities = m_intensities[channel_index],
		.filtered_intensities = m_filtered_intensities[channel_index],
		.frequencies = m_freq[channel_index],
		.derivation = m_derivation[channel_index],
		.extremas = m_extremas[channel_index],
		.peaks = m_peaks[channel_index]
	};

	return state;
}

bool spectrum::is_noise(channel_t channel)
{
	size_t channel_index = to_index(channel);
	spectrum_unit_t effective_range = abs(m_extremas[channel_index].max - m_extremas[channel_index].min);

	return effective_range < 10;
}

void spectrum::recalculate(channel_t channel)
{
	size_t channel_index = to_index(channel);
	spectrum_unit_t* intensities = m_intensities[channel_index];
	spectrum_unit_t intensity = intensities[0];

	spectrum_unit_t min = intensity;
	spectrum_unit_t max = intensity;

	for (size_t i = 0; i < m_samples; i++)
	{
		intensity = intensities[i];

		if (intensity > max)
			max = intensity;

		if (intensity < min)
			min = intensity;
	}

	m_extremas[channel_index].min = min;
	m_extremas[channel_index].max = max;
}

void spectrum::filter(channel_t channel, std::initializer_list<parameter_t> params)
{
	spectrum_state state = get_spectrum_state(channel);

	m_filter->perform(state, params);
}

void spectrum::detect(channel_t channel, std::initializer_list<parameter_t> params)
{
	spectrum_state state = get_spectrum_state(channel);

	m_detection_method->perform(state, params);
}

std::vector<region_t> spectrum::segment(channel_t channel, std::initializer_list<parameter_t> params)
{
	spectrum_state state = get_spectrum_state(channel);

	return m_segmentation_method->perform(state, params);
}

bool spectrum::perform(channel_t channel, std::initializer_list<parameter_t> filteringParams, std::initializer_list<parameter_t> detectionParams, std::initializer_list<parameter_t> segmentationParams, fit_settings_t fitSettings)
{
	recalculate(channel);

	if (is_noise(channel)) // spectrum contains only noise, skip
		return false;

	filter(channel, filteringParams);
	detect(channel, detectionParams);

	if (fitSettings.enable)
	{
		std::vector<region_t> regions = segment(channel, segmentationParams);
		std::vector<peak> replacedPeaks;

		for (auto& region : regions)
		{
			peak* newPeak;

			// checks if the region contains just 1 peak. if that is the case,
			// we can copy over that peak, unless we are force fitting.
			if (region.peaks.size() == 1 && !fitSettings.force)
			{
				newPeak = &region.peaks[0];
			}
			else
			{
				peak fittedPeak = fit(channel, fitSettings, region);
				newPeak = &fittedPeak;
			}

			// calculate bounding box
			auto intensities = this->get_source_intensities(channel);
			auto wavelengths = this->get_wavelengths();
			spectrum_unit_t topY = newPeak->intensity();
			spectrum_unit_t bottomY = std::min(intensities[region.left], intensities[region.right]);
			spectrum_unit_t leftX = wavelengths[region.left];
			spectrum_unit_t rightX = wavelengths[region.right];

			bounds_t bounds
			{
				.valid = true,
				.bottomleft = { leftX, bottomY },
				.bottomright = { rightX, bottomY },
				.topleft = { leftX, topY },
				.topright = { rightX, topY }
			};

			newPeak->bounds(bounds);

			replacedPeaks.push_back(*newPeak);
		}

		// recalculate indexes
		int counter = 1;
		for (int i = 0; i < replacedPeaks.size(); i++)
		{
			replacedPeaks[i].index(counter++);
		}

		m_peaks[to_index(channel)] = replacedPeaks;
	}

	return true;
}

void spectrum::perform(std::initializer_list<parameter_t> filteringParams, std::initializer_list<parameter_t> detectionParams, std::initializer_list<parameter_t> segmentationParams, fit_settings_t fitSettings)
{
	for (channel_t i = 1; i <= spectrum_max_channels; i++)
	{
		perform(i, filteringParams, detectionParams, segmentationParams, fitSettings);
	}
}

void spectrum::print(std::ostream& s, std::string delimiter)
{
	int totalPeaks = 0;
	for (int channel_index = 0; channel_index < spectrum_max_channels; channel_index++)
	{
		totalPeaks += m_peaks[channel_index].size();
	}

	if (totalPeaks == 0)
	{
		s << "No peaks detected." << std::endl;
		return;
	}

	s << std::fixed;
	s.imbue(std::locale(std::cout.getloc(), new decimal_comma));

	s << "Count" << delimiter << std::right << std::setw(4) << std::to_string(totalPeaks) << std::endl;
	s << "Channel" << delimiter << "Index" << delimiter << "Position" << delimiter << "Wavelength" << delimiter << "Intensity" << delimiter << "Asymmetry" << std::endl;

	for (peak p : get_peaks())
	{
		s << std::right << std::setw(7) << int(p.channel()) << delimiter
			<< std::right << std::setw(5) << int(p.index()) << delimiter
			<< std::right << std::setw(8) << int(p.position()) << delimiter
			<< std::right << std::setw(10) << std::setprecision(4) << p.wavelength() << delimiter
			<< std::right << std::setw(9) << std::setprecision(2) << p.intensity() << delimiter
			<< std::right << std::setw(9) << std::setprecision(2) << p.asymmetry()
			<< std::endl;
	}
}

std::vector<peak> spectrum::get_peaks()
{
	std::vector<peak> flattened;

	for (int channel_index = 0; channel_index < spectrum_max_channels; channel_index++)
	{
		for (int i = 0; i < m_peaks[channel_index].size(); i++)
		{
			flattened.push_back(m_peaks[channel_index][i]);
		}
	}

	return flattened;
}

std::vector<peak> spectrum::get_peaks(channel_t channel)
{
	return m_peaks[to_index(channel)];
}

void spectrum::clean()
{
	for (int channel_index = 0; channel_index < spectrum_max_channels; channel_index++)
	{
		m_peaks[channel_index].clear();
	}
}

peak spectrum::fit(channel_t channel, fit_settings_t fitSettings, region_t region)
{
	size_t channel_index = to_index(channel);
	spectrum_unit_t* intensities = m_intensities[channel_index];
	std::span<derivation_t> derivation = m_derivation[channel_index];
	minmax_t extremas = m_extremas[channel_index];
	std::vector<peak> peaks = m_peaks[channel_index];

	switch (fitSettings.fitType)
	{
	case fit_type::LINEAR:
	{
		int differentiationStart = region.left;
		if (differentiationStart == 0)
			differentiationStart++;

		// create range
		std::vector<double> ySpan;
		for (int i = differentiationStart; i <= region.right; i++)
		{
			// differentiate unfiltered spectrum, then take that range
			ySpan.push_back(intensities[i] - intensities[i - 1]);
		}

		std::vector<double> xSpan(&m_indices[region.left], &m_indices[region.right] + 1);

		std::vector<double> r = curve_fit(linear, { 0, 1 }, xSpan, ySpan);
		// std::cout << "result: " << r[0] << '\t' << r[1] << '\t' << -r[0] / r[1] << '\n';

		double zeroCrossing = -r[0] / r[1];
		double approximatedWavelength = 1510 + zeroCrossing * 0.005; // todo: add function for this in spectrum.cpp

		size_t closestIndex = std::floor(zeroCrossing + 0.5);

		return peak(closestIndex, 0, channel, approximatedWavelength, intensities[closestIndex]);
	}
	case fit_type::GAUSSIAN:
	case fit_type::BIGAUSSIAN:
	{
		// take a range
		std::vector<double> ySpan(&intensities[region.left], &intensities[region.right] + 1);
		std::vector<double> xSpan(&m_indices[region.left], &m_indices[region.right] + 1);

		// normalize for fitting
		for (int z = 0; z < ySpan.size(); z++)
			ySpan[z] += abs(extremas.min);

		// find local argmax
		int localMaxIndex = argmax(ySpan);

		spectrum_unit_t amplitudeEstimate = 1;
		double sigmaEstimate = (region.right - region.left) / 2.355; // FWHM ratio
		double muEstimate = xSpan[localMaxIndex];

		if (fitSettings.equalize)
		{
			std::vector<double> xLeft(xSpan.begin(), xSpan.begin() + localMaxIndex);
			std::vector<double> yLeft(ySpan.begin(), ySpan.begin() + localMaxIndex);
			std::vector<double> xRight(xSpan.begin() + localMaxIndex, xSpan.end());
			std::vector<double> yRight(ySpan.begin() + localMaxIndex, ySpan.end());

			if (xLeft.size() != xRight.size())
			{
				std::vector<double> *smallerX, *biggerX, *smallerY, *biggerY;
				std::vector<double> tempX, tempY;

				if (xLeft.size() < xRight.size())
				{
					smallerX = &xLeft;
					smallerY = &yLeft;
					biggerX = &xRight;
					biggerY = &yRight;
				}
				else if (xLeft.size() > xRight.size())
				{
					smallerX = &xRight;
					smallerY = &yRight;
					biggerX = &xLeft;
					biggerY = &yLeft;
				}

				std::vector<double> equalspace = linspace((size_t)0, biggerX->size() - 1, biggerX->size() - smallerX->size());
				std::vector<int> excludeIndexes;

				// convert to integers
				for (double i : equalspace)
				{
					int index = floor(i + 0.5);
					excludeIndexes.push_back(index);
				}

				int lastIndex = 0;
				for (size_t i = 0; i < biggerX->size(); ++i)
				{
					if (lastIndex < excludeIndexes.size() && i == excludeIndexes[lastIndex])
					{
						++lastIndex;
						continue;
					}

					tempX.push_back((*biggerX)[i]);
					tempY.push_back((*biggerY)[i]);
				}

				if (xLeft.size() < xRight.size())
				{
					tempX.insert(tempX.begin(), (*smallerX).begin(), (*smallerX).end());
					tempY.insert(tempY.begin(), (*smallerY).begin(), (*smallerY).end());
				}
				else if (xLeft.size() > xRight.size())
				{
					tempX.insert(tempX.end(), (*smallerX).begin(), (*smallerX).end());
					tempY.insert(tempY.end(), (*smallerY).begin(), (*smallerY).end());
				}

				xSpan = tempX;
				ySpan = tempY;
			}
		}

		// the region is too small
		if (xSpan.size() < 3)
			return region.peaks[0];

		std::vector<double> r;

		if (fitSettings.fitType == fit_type::GAUSSIAN)
			r = curve_fit(gaussian, { amplitudeEstimate, muEstimate, sigmaEstimate }, xSpan, ySpan);
		else if (fitSettings.fitType == fit_type::BIGAUSSIAN)
			r = curve_fit(bigaussian, { amplitudeEstimate, muEstimate, sigmaEstimate, sigmaEstimate }, xSpan, ySpan);

		// std::cout << "result: " << r[0] << ' ' << r[1] << ' ' << r[2] << '\n';

		spectrum_unit_t approximatedWavelength = 1510 + r[1] * 0.005; // todo: add function for this in spectrum.cpp

		int closestIndex = std::floor(r[1] + 0.5);

		// this peak does not converge
		if (closestIndex < 0 || closestIndex > m_samples)
			return region.peaks[0];

		if (fitSettings.fitType == fit_type::GAUSSIAN)
			return peak(closestIndex, 0, channel, approximatedWavelength, intensities[closestIndex]);
		else if (fitSettings.fitType == fit_type::BIGAUSSIAN)
		{
			double asymmetryEstimate = std::abs(r[2] / r[3]);
			return peak(closestIndex, 0, channel, approximatedWavelength, intensities[closestIndex], asymmetryEstimate);
		}
	}
	}

	throw new std::exception("Invalid fitting method was used.");
}

spectrum_point_t spectrum::get_spectrum_point(channel_t channel, size_t index)
{
	size_t channel_index = to_index(channel);

	spectrum_point_t point{ m_wavelengths[index], m_filtered_intensities[channel_index][index] };

	return point;
}

channel_t spectrum::get_channel_count()
{
	return spectrum_max_channels;
}

size_t spectrum::get_samples_count()
{
	return m_samples;
}

size_t spectrum::get_storage_size()
{
	return m_storage;
}

minmax_t spectrum::get_wavelength_bounds()
{
	minmax_t bounds{ m_wavelengths[0], m_wavelengths[m_samples - 1] };

	return bounds;
}

spectrum_unit_t* spectrum::get_wavelengths()
{
	return m_wavelengths;
}

spectrum_unit_t* spectrum::get_source_intensities(channel_t channel)
{
	return m_intensities[to_index(channel)];
}

spectrum_unit_t* spectrum::get_filtered_intensities(channel_t channel)
{
	return m_filtered_intensities[to_index(channel)];
}
