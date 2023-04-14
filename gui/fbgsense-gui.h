/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múčka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <mutex>
#include <thread>
#include <filesystem>
#include <chrono>
#include <sstream>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/clipbrd.h>

#include <interrogator.h>
#include <interrogator/constants/sm125.h>
#include <spectrum/spectrum_file.h>

#include "fbgsense-gui.gen.h"
#include "widgets/wxChannelCtrl.h"
#include "widgets/wxOpenGLPlotLive.h"

class PeaksDialog : public PeaksDialogBase
{
protected:
	void OnKeyUp(wxKeyEvent& event) override;
public:
	PeaksDialog(wxWindow* parent);
};

class ChartDialog : public ChartDialogBase
{
	bool prevSingleChannelEnabled = false;
	wxChannelCtrl* m_channelControls[spectrum_max_channels] = { nullptr };
public:
	wxOpenGLPlotLive* m_plotControl;

	ChartDialog(wxWindow* parent);

	void SetChannelEnabled(channel_t channel, bool value);
	void SetChannelPeakCount(channel_t channel, int count);
	bool GetChannelEnabled(channel_t channel);
	bool SingleChannelEnabledChanged(bool& state, channel_t& channel);

	void OnChannelRightClick(int channel_number, bool checked);
	void OnChannelChecked(int channel_number, bool checked);
	void UpdateSpectrumView();

	void Reset();
};

// todo: reload default settings for parameters
class MainApp : public wxApp
{
	std::string m_ipAddress;
	uint16_t m_channels{ 0 };
	uint32_t m_relativeThreshold{ 0 };
	uint32_t m_frequencyLimit{ 0 };
	fit_settings_t m_fitSettings;
	filter_type m_selectedFilter;
	segment_type m_selectedSegmentType;

	uint32_t m_statLoad{ 0 }, m_statPerform{ 0 }, m_statTotal{ 0 };
	double m_samplingRate{ 0 };
	std::vector<std::shared_ptr<GLPL::ILine2D>> m_peakBoxes;
	std::vector<float*> m_peakBoxesData;

	std::unique_ptr<spectrum> m_loadedSpectrum;
	std::unique_ptr<interrogator> m_device;
	bool m_working{ false };
	bool m_logging{ false };
	std::filesystem::path m_logFilePath;
	std::thread* m_workingThread = nullptr;

	PeaksDialog* m_peaksDialog = nullptr;
	ChartDialog* m_chartDialog = nullptr;

	std::mutex m_workerMutex;
	std::condition_variable m_syncUiUpdate;
	bool m_updatedUi{ false };

	void Worker();
	void UpdateUI(spectrum* s);

public:
	virtual bool OnInit();

	void SetWorking(bool value);
	bool GetWorking();
	bool GetLogging();
	interrogator* GetDevice();

	void SetIpAddress(std::string ip);
	void SetChannels(uint16_t channels);
	void SetEnableSuppression(bool value);
	void SetForceSuppress(bool value);
	void SetEqualizeSuppress(bool value);
	void SetSuppressionType(fit_type value);
	void SetDetectionType(filter_type value);
	void SetRelativeThreshold(uint32_t value);
	void SetFrequencyLimit(uint32_t value);
	void SetSamplingRate(double value);
	void SetSegmentType(segment_type value);

	void SetDefaultOptions();

	void Stop();
	void ConnectAndStart();
	void LoadSpectrumAndStart();
	void StartLogging();
	void StopLogging();
	void InitPlot(spectrum* spectrum);
	void UpdateLoadedSpectrum();
	void UpdatePeakView(spectrum* spectrum);
	void UpdateSpectrumView(spectrum* spectrum);
	void UpdateSpectrumView(spectrum* spectrum, channel_t channel);
	void UpdateMainUI();
};

class MainWindow : public MainWindowBase
{
protected:
	void m_checkBoxSuppressOnCheckBox(wxCommandEvent& event) override;
	void m_checkBoxForceOnCheckBox(wxCommandEvent& event) override;
	void m_buttonConnectOnButtonClick(wxCommandEvent& event) override;
	void m_buttonLoadSpectrumOnButtonClick(wxCommandEvent& event) override;
	void m_radioBoxSuppressionAlgoOnRadioBox(wxCommandEvent& event) override;
	void m_radioBoxDetectionAlgoOnRadioBox(wxCommandEvent& event) override;
	void m_checkBoxEqualizeOnCheckBox(wxCommandEvent& event) override;
	void m_spinCtrlRelOnSpinCtrl(wxSpinEvent& event) override;
	void m_spinCtrlLimOnSpinCtrl(wxSpinEvent& event) override;
	void m_buttonStartLogOnButtonClick(wxCommandEvent& event) override;
	void m_choiceSegmentByOnChoice(wxCommandEvent& event) override;

	void OnClose(wxCloseEvent& event) override;
public:
	MainWindow();

	void OnConnect();
	void OnDisconnect();
	void OnLoggingStart();
	void OnLoggingStop();
};

DECLARE_APP(MainApp)
