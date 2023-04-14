/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <openGLPlotLive/plot/plot.h>

namespace PlotColors
{
	static constexpr glm::vec3 Green1{ 0.435f, 0.929f, 0.0f };
	static constexpr glm::vec3 Green2{ 0.412f, 0.878f, 0.0f };
	static constexpr glm::vec3 Green3{ 0.365f, 0.78f,  0.0f };
	static constexpr glm::vec3 Green4{ 0.294f, 0.631f, 0.0f };

	static constexpr glm::vec3 Yellow1{ 0.929f, 0.914f, 0.0f };
	static constexpr glm::vec3 Yellow2{ 0.878f, 0.867f, 0.0f };
	static constexpr glm::vec3 Yellow3{ 0.780f, 0.769f, 0.0f };
	static constexpr glm::vec3 Yellow4{ 0.631f, 0.620f, 0.0f };

	static constexpr glm::vec3 Red1{ 0.929f, 0.184f, 0.0f };
	static constexpr glm::vec3 Red2{ 0.878f, 0.176f, 0.0f };
	static constexpr glm::vec3 Red3{ 0.780f, 0.161f, 0.0f };
	static constexpr glm::vec3 Red4{ 0.631f, 0.125f, 0.0f };

	static constexpr glm::vec3 Cyan1{ 0.0f, 0.941f, 0.918f };
	static constexpr glm::vec3 Cyan2{ 0.0f, 0.878f, 0.851f };
	static constexpr glm::vec3 Cyan3{ 0.0f, 0.780f, 0.753f };
	static constexpr glm::vec3 Cyan4{ 0.0f, 0.631f, 0.607f };

	static constexpr glm::vec3 White{ 1.0f, 1.0f, 1.0f };

	static constexpr glm::vec3 color_scheme[17]{
		PlotColors::Green1, PlotColors::Green2,PlotColors::Green3,PlotColors::Green4,
		PlotColors::Yellow1, PlotColors::Yellow2,PlotColors::Yellow3,PlotColors::Yellow4,
		PlotColors::Red1, PlotColors::Red2,PlotColors::Red3,PlotColors::Red4,
		PlotColors::Cyan1, PlotColors::Cyan2,PlotColors::Cyan3,PlotColors::Cyan4,
		PlotColors::White
	};
}
