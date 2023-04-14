/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <numbers>
#include <vector>
#include <span>
#include <array>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <format>

#include <fstream>
#include <exception>

#include "spectrum_types.h"
#include "spectrum_state.h"
#include "io/decimal_comma.h"
#include "peak.h"
#include "fitting.h"
#include "linspace.h"
#include "argmax.h"

#include "filtering/filter.h"
#include "filtering/filter_fft.h"
#include "filtering/filter_savgol.h"

#include "detection/detection_method.h"
#include "detection/detection_method_localmax.h"

#include "segmentation/segmentation_method.h"
#include "segmentation/segmentation_method_localmin.h"
#include "segmentation/segmentation_method_inflection.h"

constexpr channel_t spectrum_max_channels = 16;

/**
 * @brief Data structure providing access to the implemented method.
*/
class spectrum
{
protected:
	size_t m_samples{ 0 };
	size_t m_storage{ 0 };

	spectrum_unit_t* m_indices;
	spectrum_unit_t* m_wavelengths;
	spectrum_unit_t* m_intensities[spectrum_max_channels];

	spectrum_unit_t* m_filtered_intensities[spectrum_max_channels];
	std::span<complex_t> m_freq[spectrum_max_channels];
	std::span<derivation_t> m_derivation[spectrum_max_channels];

	minmax_t m_extremas[spectrum_max_channels];
	std::vector<peak> m_peaks[spectrum_max_channels];

	std::unique_ptr<fbgsense::filtering::filter> m_filter;
	std::unique_ptr<fbgsense::detection::detection_method> m_detection_method;
	std::unique_ptr<fbgsense::segmentation::segmentation_method> m_segmentation_method;
public:
	inline static constexpr size_t to_index(channel_t channel)
	{
		return channel - 1;
	}

	spectrum();
	spectrum(size_t storage, size_t samples);
	spectrum(size_t samples);
	~spectrum();

	/**
	 * @brief Set the filter that will remove noise from the spectrum.
	 * The default is filter_type::FFT.
	*/
	void set_filter(filter_type type);

	/**
	 * @brief Set custom filtering processor.
	*/
	void set_filter(processor<fbgsense::filtering::filter>&& type);

	/**
	 * @brief Set the detection method that will find potential peaks.
	 * The default method is detection_type::LOCAL_MAXIMA.
	*/
	void set_detection_method(detection_type type);

	/**
	* @brief Set custom detection method processor.
	*/
	void set_detection_method(processor<fbgsense::detection::detection_method>&& type);

	/**
	 * @brief Set the method that will segment the spectrum into peak regions.
	 * The default method is segment_type::LOCAL_MINIMA.
	*/
	void set_segmentation_method(segment_type type);

	/**
	* @brief Set custom segmentation method processor.
	*/
	void set_detection_method(processor<fbgsense::segmentation::segmentation_method>&& type);

	/**
	 * @brief Get wavelength array. This array is shared across all intensities in the spectrum.
	*/
	spectrum_unit_t* get_wavelengths();

	/**
	 * @brief Get source intensities array for the specified channel.
	*/
	spectrum_unit_t* get_source_intensities(channel_t channel);

	/**
	 * @brief Get filtered intensities array for the specified channel.
	*/
	spectrum_unit_t* get_filtered_intensities(channel_t channel);

	/**
	 * @brief Get internal data structures for the given channel.
	*/
	spectrum_state get_spectrum_state(channel_t channel);

	/**
	 * @brief Get size of physical storage array.
	*/
	size_t get_storage_size();

	/**
	 * @brief Get number of samples.
	*/
	size_t get_samples_count();

	/**
	 * @brief Get number of channels.
	*/
	static channel_t get_channel_count();

	/**
	 * @brief Get lowest and highest wavelength value.
	*/
	minmax_t get_wavelength_bounds();

	/**
	 * @brief Checks if the signal intensity is within reasonable range.
	*/
	bool is_noise(channel_t channel);

	/**
	 * @brief Find global extremas.
	*/
	void recalculate(channel_t channel);

	/**
	 * @brief Filtering process.
	*/
	void filter(channel_t channel, std::initializer_list<parameter_t> params);

	/**
	 * @brief Peak detection process.
	*/
	void detect(channel_t channel, std::initializer_list<parameter_t> params);

	/**
	 * @brief Find peak regions of interest (ROI).
	*/
	std::vector<region_t> segment(channel_t channel, std::initializer_list<parameter_t> params);

	/**
	 * @brief Fit a function on the specified dataset using least squares technique.
	*/
	peak fit(channel_t channel, fit_settings_t fitSettings, region_t region);

	/**
	 * @brief Perform every step of calculation on the specific channel.
	 * @return true when all steps are completed, false if the spectrum contains only noise (steps have been skipped)
	*/
	bool perform(channel_t channel, std::initializer_list<parameter_t> filteringParams, std::initializer_list<parameter_t> detectionParams, std::initializer_list<parameter_t> segmentationParams, fit_settings_t fitSettings);

	/**
	 * @brief Perform every step of calculation for all channels.
	*/
	void perform(std::initializer_list<parameter_t> filteringParams, std::initializer_list<parameter_t> detectionParams, std::initializer_list<parameter_t> segmentationParams, fit_settings_t fitSettings);

	/**
	 * @brief Clear found peaks.
	*/
	void clean();

	/**
	 * @brief Print peaks to the specified stream.
	*/
	void print(std::ostream& s, std::string delimiter = "\t");

	/**
	 * @brief Get flattened list of peaks.
	*/
	std::vector<peak> get_peaks();

	/**
	 * @brief Get peaks of specific channel.
	*/
	std::vector<peak> get_peaks(channel_t channel);

	/**
	 * @brief Get a single point from spectrum channel in form of a tuple.
	*/
	spectrum_point_t get_spectrum_point(channel_t channel, size_t index);
};
