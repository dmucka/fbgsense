/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múčka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "fbgsense-gui.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	MainWindow* MainWin = new MainWindow();
	MainWin->Show();

	MainWin->SetFocus();

	m_peaksDialog = new PeaksDialog(MainWin);
	m_chartDialog = new ChartDialog(MainWin);

	SetTopWindow(MainWin);

	SetDefaultOptions();

	return true;
}

void MainApp::UpdatePeakView(spectrum* spectrum)
{
	m_peaksDialog->m_peakDataView->DeleteAllItems();

	for (channel_t ch = 1; ch <= spectrum->get_channel_count(); ++ch)
	{
		auto peaks = spectrum->get_peaks(ch);
		m_chartDialog->SetChannelPeakCount(ch, peaks.size());

		for (peak& p : peaks)
		{
			wxVector<wxVariant> data;

			data.push_back(wxVariant(std::to_string(p.channel())));
			data.push_back(wxVariant(std::to_string(p.index())));
			data.push_back(wxVariant(std::to_string(p.position())));

			std::ostringstream sWavelength, sIntensity, sAsymmetry;

			sWavelength.imbue(std::locale(std::cout.getloc(), new decimal_comma));
			sIntensity.imbue(std::locale(std::cout.getloc(), new decimal_comma));
			sAsymmetry.imbue(std::locale(std::cout.getloc(), new decimal_comma));

			sWavelength << std::fixed << std::setprecision(4) << p.wavelength();
			sIntensity << std::fixed << std::setprecision(2) << p.intensity();
			sAsymmetry << std::fixed << std::setprecision(2) << p.asymmetry();

			data.push_back(wxVariant(sWavelength.str()));
			data.push_back(wxVariant(sIntensity.str()));
			data.push_back(wxVariant(sAsymmetry.str()));

			m_peaksDialog->m_peakDataView->AppendItem(data);
		}
	}

	m_peaksDialog->Refresh();
}

void MainApp::UpdateSpectrumView(spectrum* spectrum, channel_t channel)
{
	size_t channel_index = spectrum::to_index(channel);

	if (/*spectrum->is_noise(channel) || */!m_chartDialog->GetChannelEnabled(channel))
	{
		m_chartDialog->m_plotControl->ClearSeries(channel_index);
		return;
	}

	m_chartDialog->m_plotControl->UpdateSeries(channel_index);

	// cleanup old bounding boxes
	for (auto& d : m_peakBoxesData)
		delete[] d;

	m_peakBoxesData.clear();

	for (auto& line : m_peakBoxes)
	{
		std::dynamic_pointer_cast<GLPL::Line2DReadOnly>(line)->clearBuffer();
		line->setTemporary(true);
	}

	m_peakBoxes.clear();

	bool singleState;
	channel_t singleChannel;
	bool singleChanged = m_chartDialog->SingleChannelEnabledChanged(singleState, singleChannel);
	bool isNoise = spectrum->is_noise(singleChannel);
	if (singleChanged || singleState)
	{
		unsigned int filteredSpectrumSeriesIndex = m_chartDialog->m_plotControl->GetAdditionalSeriesIndex(0);
		auto filteredSpectrumSeries = m_chartDialog->m_plotControl->GetSeries(filteredSpectrumSeriesIndex);
		if (singleState && !spectrum->is_noise(singleChannel))
		{
			// show and update filtered spectrum
			m_chartDialog->m_plotControl->SetSeriesSource(filteredSpectrumSeriesIndex,
				spectrum->get_wavelengths(),
				spectrum->get_filtered_intensities(singleChannel),
				spectrum->get_samples_count());

			m_chartDialog->m_plotControl->UpdateSeries(filteredSpectrumSeriesIndex);
			filteredSpectrumSeries->setVisible(true);

			// add bounding boxes
			for (peak& p : spectrum->get_peaks(singleChannel))
			{
				const int size = 2;
				if (p.bounds().valid)
				{
					auto bottomLineX = new float[] { p.bounds().bottomleft.x, p.bounds().bottomright.x };
					auto bottomLineY = new float[] { p.bounds().bottomleft.y, p.bounds().bottomright.y };

					auto topLineX = new float[] { p.bounds().topleft.x, p.bounds().topright.x };
					auto topLineY = new float[] { p.bounds().topleft.y, p.bounds().topright.y };

					auto leftLineX = new float[] { p.bounds().bottomleft.x, p.bounds().bottomleft.x };
					auto leftLineY = new float[] { p.bounds().bottomleft.y, p.bounds().topleft.y };

					auto rightLineX = new float[] { p.bounds().bottomright.x, p.bounds().bottomright.x };
					auto rightLineY = new float[] { p.bounds().bottomright.y, p.bounds().topright.y };

					auto bottomLine = m_chartDialog->m_plotControl->AddTemporarySeries(bottomLineX, bottomLineY, size);
					auto topLine = m_chartDialog->m_plotControl->AddTemporarySeries(topLineX, topLineY, size);
					auto leftLine = m_chartDialog->m_plotControl->AddTemporarySeries(leftLineX, leftLineY, size);
					auto rightLine = m_chartDialog->m_plotControl->AddTemporarySeries(rightLineX, rightLineY, size);

					m_peakBoxes.insert(m_peakBoxes.end(), {bottomLine, topLine, leftLine, rightLine});
					m_peakBoxesData.insert(m_peakBoxesData.end(), {bottomLineX, bottomLineY, topLineX, topLineY, leftLineX, leftLineY, rightLineX, rightLineY});
				}

				auto peakCrossX = new float[] { p.wavelength(), p.wavelength() };
				auto peakCrossY = new float[] { p.intensity() - 2, p.intensity() + 2 };
				auto peakCross = m_chartDialog->m_plotControl->AddTemporarySeries(peakCrossX, peakCrossY, size);
				m_peakBoxes.push_back(peakCross);
				m_peakBoxesData.insert(m_peakBoxesData.end(), {peakCrossX, peakCrossY});
			}
		}
		else
		{
			// hide filtered spectrum
			filteredSpectrumSeries->setVisible(false);
		}
	}
}

void MainApp::UpdateSpectrumView(spectrum* spectrum)
{
	for (channel_t ch = 1; ch <= spectrum->get_channel_count(); ++ch)
	{
		UpdateSpectrumView(spectrum, ch);
	}

	m_chartDialog->m_plotControl->Refresh();
}

void MainApp::Worker()
{
	bool firstTime = true;
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	try
	{
		while (GetWorking())
		{
			auto begin_total = std::chrono::high_resolution_clock::now();

			m_device->recv_spectrums();

			auto end_load = std::chrono::high_resolution_clock::now();

			spectrum_sm125* s = m_device->spectrums();
			s->set_filter(m_selectedFilter);
			s->set_segmentation_method(m_selectedSegmentType);
			s->perform({ (parameter_t)m_frequencyLimit }, { (parameter_t)m_relativeThreshold }, { (parameter_t)m_relativeThreshold }, m_fitSettings);

			auto end_perform = std::chrono::high_resolution_clock::now();

			m_statLoad = std::chrono::duration_cast<std::chrono::milliseconds>(end_load - begin_total).count();
			m_statPerform = std::chrono::duration_cast<std::chrono::milliseconds>(end_perform - end_load).count();
			m_statTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end_perform - begin_total).count();

			if (firstTime)
			{
				firstTime = false;

				for (channel_t ch = 1; ch <= s->get_channel_count(); ++ch)
				{
					auto peaks = s->get_peaks(ch);
					if (peaks.size() != 0)
						MainWin->GetEventHandler()->CallAfter(std::bind(&ChartDialog::SetChannelEnabled, m_chartDialog, ch, true));
					else
						MainWin->GetEventHandler()->CallAfter(std::bind(&ChartDialog::SetChannelEnabled, m_chartDialog, ch, false));
				}
			}

			if (GetLogging())
			{
				// try to open file
				std::ofstream logFile;
				logFile.open(m_logFilePath, std::ios::out | std::ios::app);

				if (!logFile.is_open())
				{
					wxMessageBox("Could not write to log file.", "Error", wxOK | wxICON_ERROR);
					continue;
				}

				logFile << std::fixed;
				logFile.imbue(std::locale(std::cout.getloc(), new decimal_comma));

				// https://stackoverflow.com/questions/24686846/get-current-time-in-milliseconds-or-hhmmssmmm-format
				// get current time
				auto now = std::chrono::system_clock::now();

				// get number of milliseconds for the current second
				// (remainder after division into seconds)
				auto ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 100000;

				// convert to std::time_t in order to convert to std::tm (broken time)
				auto timer = std::chrono::system_clock::to_time_t(now);

				// convert to broken time
				auto bt = *std::localtime(&timer);

				// write timestamp in format dd.MM.yyyy HH:mm:ss.fffff
				logFile << std::put_time(&bt, "%d.%m.%Y %H:%M:%S") << "." << std::setfill('0') << std::setw(5) << ms.count() << "\t";


				// write peak counts
				for (channel_t ch = 1; ch <= spectrum::get_channel_count(); ++ch)
				{
					auto peaks = s->get_peaks(ch);
					logFile << std::to_string(peaks.size()) << "\t";
				}

				// write data
				for (channel_t ch = 1; ch <= spectrum::get_channel_count(); ++ch)
				{
					auto peaks = s->get_peaks(ch);

					for (peak& p : peaks)
					{
						// write wavelengths
						logFile << std::setprecision(5) << p.wavelength() << "\t";

						// write asymmetry
						logFile << std::setprecision(2) << p.asymmetry() << "\t";
					}
				}

				logFile << std::endl;

				logFile.close();
			}

			if (GetWorking())
			{
				m_updatedUi = false;

				MainWin->GetEventHandler()->CallAfter(std::bind(&MainApp::UpdateUI, this, s));

				std::unique_lock<std::mutex> lck(m_workerMutex);

				if (!m_updatedUi)
				{
					m_syncUiUpdate.wait(lck);
				}

				auto end_total = std::chrono::high_resolution_clock::now();
				double totalSeconds = std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::seconds>(end_total - begin_total)).count();
				if (totalSeconds < m_samplingRate)
				{
					wxMilliSleep((m_samplingRate - totalSeconds) * 1000);
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		wxMessageBox(ex.what(), "Error", wxOK | wxICON_ERROR);
		MainWin->GetEventHandler()->CallAfter(std::bind(&MainApp::Stop, this));
	}
}

void MainApp::UpdateMainUI()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	MainWin->m_statLoad->SetLabel(std::to_string(m_statLoad));
	MainWin->m_statPerform->SetLabel(std::to_string(m_statPerform));
	MainWin->m_statTotal->SetLabel(std::to_string(m_statTotal));
	MainWin->gbSizerStats->Layout();
}

void MainApp::UpdateUI(spectrum* s)
{
	{
		std::lock_guard<std::mutex> lck(m_workerMutex);

		UpdatePeakView(s);
		UpdateSpectrumView(s);
		UpdateMainUI();

		s->clean();

		m_updatedUi = true;
	}

	m_syncUiUpdate.notify_all();
}

void MainApp::SetDefaultOptions()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	SetIpAddress(MainWin->m_textCtrlAddress->GetValue().ToStdString());
	SetChannels(MainWin->m_spinCtrlChannels->GetValue());
	SetEnableSuppression(MainWin->m_checkBoxSuppress->GetValue());
	SetForceSuppress(MainWin->m_checkBoxForce->GetValue());
	SetEqualizeSuppress(MainWin->m_checkBoxEqualize->GetValue());
	SetSuppressionType(static_cast<fit_type>(MainWin->m_radioBoxSuppressionAlgo->GetSelection()));
	SetDetectionType(static_cast<filter_type>(MainWin->m_radioBoxDetectionAlgo->GetSelection()));
	SetRelativeThreshold(MainWin->m_spinCtrlRel->GetValue());
	SetFrequencyLimit(MainWin->m_spinCtrlLim->GetValue());
	SetSamplingRate(MainWin->m_spinCtrlSamplingRate->GetValue());
	SetSegmentType(static_cast<segment_type>(MainWin->m_choiceSegmentBy->GetSelection()));
}

void MainApp::Stop()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	// check if we are logging
	if (GetLogging())
	{
		StopLogging();

		// user still wants to log
		if (GetLogging())
			return;
	}

	// signal thread to stop
	SetWorking(false);
	m_workingThread->join();

	m_device = nullptr;

	m_peaksDialog->Close();
	m_chartDialog->Close();

	MainWin->OnDisconnect();
}

void MainApp::ConnectAndStart()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	try
	{
		SetWorking(true);
		wxBeginBusyCursor();

		if (m_loadedSpectrum != nullptr)
			m_loadedSpectrum = nullptr;

		m_device = std::unique_ptr<interrogator>(new interrogator(m_ipAddress, sm125_port));
		m_device->set_channels(m_channels);

		MainWin->OnConnect();

		// start work thread on background
		m_workingThread = new std::thread(&MainApp::Worker, this);

		// show peak list
		m_peaksDialog->Show();

		// show visualizer
		m_chartDialog->Reset();
		m_chartDialog->Show();
		InitPlot(m_device->spectrums());

		wxEndBusyCursor();
	}
	catch (std::exception& ex)
	{
		SetWorking(false);
		wxEndBusyCursor();
		MainWin->OnDisconnect();
		wxMessageBox(ex.what(), "Error", wxOK | wxICON_ERROR);
	}
}

void MainApp::LoadSpectrumAndStart()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	wxFileDialog openFileDialog(MainWin, "Open spectrum file", "", "", "CSV files|*.txt;*.csv", wxFD_OPEN);

	// on cancel
	if (openFileDialog.ShowModal() != wxID_OK)
		return;

	std::filesystem::path spectrumFilePath(openFileDialog.GetPath().ToStdWstring());

	if (!std::filesystem::exists(spectrumFilePath) || std::filesystem::is_directory(spectrumFilePath))
	{
		wxMessageBox("Specified file does not exist.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	try
	{
		SetWorking(true);
		wxBeginBusyCursor();

		m_chartDialog->Reset();

		auto start_load = std::chrono::high_resolution_clock::now();

		spectrum_file* s = new spectrum_file(sm125_samples, sm125_samples - 1);
		s->load(spectrumFilePath);

		// todo: cleanup in close event
		m_loadedSpectrum = std::unique_ptr<spectrum>(s);

		auto end_load = std::chrono::high_resolution_clock::now();

		m_statLoad = std::chrono::duration_cast<std::chrono::milliseconds>(end_load - start_load).count();

		UpdateLoadedSpectrum();

		wxEndBusyCursor();
	}
	catch (std::exception& ex)
	{
		SetWorking(false);
		wxEndBusyCursor();
		wxMessageBox(ex.what(), "Error", wxOK | wxICON_ERROR);
	}
}

void MainApp::StartLogging()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	wxFileDialog saveFileDialog(MainWin, "Save log file", "", "", "Text files|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	// on cancel
	if (saveFileDialog.ShowModal() != wxID_OK)
		return;

	m_logFilePath = std::filesystem::path(saveFileDialog.GetPath().ToStdWstring());

	// try to open file
	std::ofstream logFile;
	logFile.open(m_logFilePath, std::ios::out | std::ios::trunc);

	if (!logFile.is_open())
	{
		wxMessageBox("Could not write to log file.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	// write header
	logFile << "Timestamp\t";
	for (int i = 1; i <= spectrum::get_channel_count(); ++i)
		logFile << std::to_string(i) << "\t";

	logFile << std::endl;

	logFile.close();

	// start logging
	m_logging = true;
	MainWin->OnLoggingStart();
}

void MainApp::StopLogging()
{
	MainWindow* MainWin = static_cast<MainWindow*>(GetMainTopWindow());

	// ask user if its intentional
	int result = wxMessageBox("Are you sure you want to stop logging?", "Logging", wxYES_NO | wxNO_DEFAULT);
	if (result != wxYES)
		return;

	m_logFilePath = "";
	m_logging = false;

	MainWin->OnLoggingStop();
}

void MainApp::InitPlot(spectrum* s)
{
	for (channel_t ch = 1; ch <= s->get_channel_count(); ++ch)
		m_chartDialog->m_plotControl->SetSeriesSource(
			s->to_index(ch),
			s->get_wavelengths(),
			s->get_source_intensities(ch),
			s->get_samples_count());
}

void MainApp::UpdateLoadedSpectrum()
{
	if (m_loadedSpectrum == nullptr)
		return;

	SetWorking(true);

	auto start_perform = std::chrono::high_resolution_clock::now();

	m_loadedSpectrum->set_filter(m_selectedFilter);
	m_loadedSpectrum->set_segmentation_method(m_selectedSegmentType);
	m_loadedSpectrum->perform(1, { (parameter_t)m_frequencyLimit }, { (parameter_t)m_relativeThreshold }, { (parameter_t)m_relativeThreshold }, m_fitSettings);

	auto end_perform = std::chrono::high_resolution_clock::now();

	m_statPerform = std::chrono::duration_cast<std::chrono::milliseconds>(end_perform - start_perform).count();
	m_statTotal = m_statLoad + m_statPerform;

	// show peak list
	UpdatePeakView(m_loadedSpectrum.get());
	m_peaksDialog->Show();

	// show visualizer
	m_chartDialog->SetChannelEnabled(1, true);
	m_chartDialog->Show();
	InitPlot(m_loadedSpectrum.get());
	UpdateSpectrumView(m_loadedSpectrum.get(), 1);
	m_chartDialog->Refresh();

	m_loadedSpectrum->clean();

	// update main UI
	UpdateMainUI();

	SetWorking(false);
}

#pragma region MainWindow

MainWindow::MainWindow() : MainWindowBase((wxFrame*)NULL)
{
}

void MainWindow::OnConnect()
{
	m_spinCtrlSamplingRate->Disable();
	m_buttonLoadSpectrum->Disable();
	m_textCtrlAddress->Disable();
	m_spinCtrlChannels->Disable();
	m_buttonStartLog->Enable();

	m_buttonConnect->SetLabel("Disconnect");
}

void MainWindow::OnDisconnect()
{
	m_spinCtrlSamplingRate->Enable();
	m_buttonLoadSpectrum->Enable();
	m_textCtrlAddress->Enable();
	m_spinCtrlChannels->Enable();
	m_buttonStartLog->Disable();

	m_buttonConnect->SetLabel("Connect");
}

void MainWindow::OnLoggingStart()
{
	m_buttonStartLog->SetLabel("Stop logging");
}

void MainWindow::OnLoggingStop()
{
	m_buttonStartLog->SetLabel("Start logging");
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	if (wxGetApp().GetWorking())
		wxGetApp().Stop();

	event.Skip();
}

#pragma endregion

#pragma region Buttons

void MainWindow::m_buttonConnectOnButtonClick(wxCommandEvent& event)
{
	if (wxGetApp().GetWorking())
	{
		wxGetApp().Stop();
	}
	else
	{
		wxGetApp().SetChannels(m_spinCtrlChannels->GetValue());
		wxGetApp().SetIpAddress(m_textCtrlAddress->GetValue().ToStdString());
		wxGetApp().SetSamplingRate(m_spinCtrlSamplingRate->GetValue());
		wxGetApp().ConnectAndStart();
	}
}

void MainWindow::m_buttonLoadSpectrumOnButtonClick(wxCommandEvent& event)
{
	wxGetApp().LoadSpectrumAndStart();
}

void MainWindow::m_buttonStartLogOnButtonClick(wxCommandEvent& event)
{
	if (wxGetApp().GetLogging())
	{
		wxGetApp().StopLogging();
	}
	else
	{
		wxGetApp().StartLogging();
	}
}

#pragma endregion

#pragma region Setters

void MainApp::SetIpAddress(std::string ip)
{
	m_ipAddress = ip;
}

void MainApp::SetChannels(uint16_t channels)
{
	m_channels = channels;
}

void MainApp::SetEnableSuppression(bool value)
{
	m_fitSettings.enable = value;
}

void MainApp::SetForceSuppress(bool value)
{
	m_fitSettings.force = value;
}

void MainApp::SetEqualizeSuppress(bool value)
{
	m_fitSettings.equalize = value;
}

void MainApp::SetSuppressionType(fit_type value)
{
	m_fitSettings.fitType = value;
}

void MainApp::SetDetectionType(filter_type value)
{
	m_selectedFilter = value;
}

void MainApp::SetRelativeThreshold(uint32_t value)
{
	m_relativeThreshold = value;
}

void MainApp::SetFrequencyLimit(uint32_t value)
{
	m_frequencyLimit = value;
}

void MainApp::SetWorking(bool value)
{
	m_working = value;
}

void MainApp::SetSamplingRate(double value)
{
	m_samplingRate = value;
}

void MainApp::SetSegmentType(segment_type value)
{
	m_selectedSegmentType = value;
}

#pragma endregion

#pragma region Getters

bool MainApp::GetWorking()
{
	return m_working;
}

bool MainApp::GetLogging()
{
	return m_logging;
}

interrogator* MainApp::GetDevice()
{
	return m_device.get();
}

#pragma endregion

#pragma region Event Handlers

void MainWindow::m_checkBoxSuppressOnCheckBox(wxCommandEvent& event)
{
	if (m_checkBoxSuppress->GetValue())
	{
		m_checkBoxForce->Enable();
		m_checkBoxEqualize->Enable();
		m_radioBoxSuppressionAlgo->Enable();
		m_choiceSegmentBy->Enable();
	}
	else
	{
		m_checkBoxForce->Disable();
		m_checkBoxEqualize->Disable();
		m_radioBoxSuppressionAlgo->Disable();
		m_choiceSegmentBy->Disable();
	}

	wxGetApp().SetEnableSuppression(m_checkBoxSuppress->GetValue());
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_checkBoxForceOnCheckBox(wxCommandEvent& event)
{
	wxGetApp().SetForceSuppress(m_checkBoxForce->GetValue());
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_checkBoxEqualizeOnCheckBox(wxCommandEvent& event)
{
	wxGetApp().SetEqualizeSuppress(m_checkBoxEqualize->GetValue());
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_radioBoxSuppressionAlgoOnRadioBox(wxCommandEvent& event)
{
	wxGetApp().SetSuppressionType(static_cast<fit_type>(m_radioBoxSuppressionAlgo->GetSelection()));
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_radioBoxDetectionAlgoOnRadioBox(wxCommandEvent& event)
{
	wxGetApp().SetDetectionType(static_cast<filter_type>(m_radioBoxDetectionAlgo->GetSelection()));
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_spinCtrlRelOnSpinCtrl(wxSpinEvent& event)
{
	wxGetApp().SetRelativeThreshold(m_spinCtrlRel->GetValue());
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_spinCtrlLimOnSpinCtrl(wxSpinEvent& event)
{
	wxGetApp().SetFrequencyLimit(m_spinCtrlLim->GetValue());
	wxGetApp().UpdateLoadedSpectrum();
}

void MainWindow::m_choiceSegmentByOnChoice(wxCommandEvent& event)
{
	wxGetApp().SetSegmentType(static_cast<segment_type>(m_choiceSegmentBy->GetSelection()));
	wxGetApp().UpdateLoadedSpectrum();
}

#pragma endregion

#pragma region PeaksDialog

void PeaksDialog::OnKeyUp(wxKeyEvent& event)
{
	// handle ctrl+c to copy values from table
	if (event.ControlDown() && event.GetKeyCode() == 'C')
	{
		std::ostringstream s;

		if (m_peakDataView->GetSelectedItemsCount() == 0) // if nothing is selected, copy whole table
		{
			for (int row = 0; row < m_peakDataView->GetStore()->GetItemCount(); ++row)
			{
				for (int col = 0; col < m_peakDataView->GetColumnCount(); ++col)
				{
					if (col != 0)
						s << '\t';

					wxVariant val;
					m_peakDataView->GetStore()->GetValueByRow(val, row, col);
					s << val.GetString();
				}

				s << std::endl;
			}
		}
		else // copy selected rows only
		{
			wxDataViewItemArray selections;
			m_peakDataView->GetSelections(selections);

			for (wxDataViewItem const& row : selections)
			{
				for (int col = 0; col < m_peakDataView->GetColumnCount(); ++col)
				{
					if (col != 0)
						s << '\t';

					wxVariant val;
					m_peakDataView->GetStore()->GetValue(val, row, col);
					s << val.GetString();
				}

				s << std::endl;
			}
		}

		if (wxTheClipboard->Open())
		{
			wxTheClipboard->SetData(new wxTextDataObject(s.str()));
			wxTheClipboard->Close();
		}
	}
}

PeaksDialog::PeaksDialog(wxWindow* parent) : PeaksDialogBase(parent)
{
}

#pragma endregion

#pragma region ChartDialog

ChartDialog::ChartDialog(wxWindow* parent) : ChartDialogBase(parent)
{
	//this->SetBackgroundColour(wxTheColourDatabase->Find("BLACK"));

	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);

	wxGLAttributes vAttrs;
	// Defaults should be accepted
	vAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	if (!wxGLCanvas::IsDisplaySupported(vAttrs))
	{
		wxMessageBox("Visual attributes for OpenGL are not accepted.\nThe app will exit now.",
			"Error with OpenGL", wxOK | wxICON_ERROR);
		wxGetApp().Exit();
	}

	m_plotControl = new wxOpenGLPlotLive(panel, vAttrs);

	panelSizer->Add(m_plotControl, 1, wxEXPAND);

	panel->SetSizer(panelSizer);

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(panel, 1, wxEXPAND);

	bSizerChart->Add(topSizer, 1, wxEXPAND);

	// prepare N channels
	for (int i = 0; i < spectrum::get_channel_count(); ++i)
	{
		wxChannelCtrl* m_channelCtrl = new wxChannelCtrl(this, wxID_ANY, i + 1,
			std::bind(&ChartDialog::OnChannelChecked, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&ChartDialog::OnChannelRightClick, this, std::placeholders::_1, std::placeholders::_2));
		//m_channelCtrl->SetBackgroundColour(wxTheColourDatabase->Find("BLACK"));
		m_channelCtrl->SetColor(i);
		bSizerHeader->Add(m_channelCtrl, 0, wxALL, this->FromDIP(10));

		m_channelControls[i] = m_channelCtrl;
	}

	// 16 channels
	m_plotControl->QueueSeries(spectrum::get_channel_count());

	// filtered spectrum
	m_plotControl->QueueSeries(1);

	this->Layout();
}

void ChartDialog::SetChannelEnabled(channel_t channel, bool value)
{
	m_channelControls[spectrum::to_index(channel)]->SetChecked(value);
	m_plotControl->ShowSeries(spectrum::to_index(channel), value);
}

void ChartDialog::SetChannelPeakCount(channel_t channel, int count)
{
	m_channelControls[spectrum::to_index(channel)]->SetPeakCount(count);
}

bool ChartDialog::GetChannelEnabled(channel_t channel)
{
	return m_channelControls[spectrum::to_index(channel)]->GetChecked();
}

bool ChartDialog::SingleChannelEnabledChanged(bool& state, channel_t& channel)
{
	int enabledChannels = 0;
	channel_t lastEnabledChannel = 0;

	for (int i = 0; i < spectrum::get_channel_count(); ++i)
	{
		if (m_channelControls[i]->GetChecked())
		{
			++enabledChannels;
			lastEnabledChannel = i + 1;
		}
	}

	state = enabledChannels == 1;
	channel = lastEnabledChannel;

	if (state != prevSingleChannelEnabled)
	{
		prevSingleChannelEnabled = state;
		return true;
	}

	return false;
}

void ChartDialog::Reset()
{
	for (channel_t ch = 1; ch <= spectrum::get_channel_count(); ++ch)
	{
		SetChannelPeakCount(ch, 0);
		SetChannelEnabled(ch, false);
	}

	prevSingleChannelEnabled = false;
	m_plotControl->Reset();
	Refresh();
}

void ChartDialog::OnChannelRightClick(int channel_number, bool checked)
{
	for (channel_t ch = 1; ch <= spectrum::get_channel_count(); ++ch)
	{
		SetChannelEnabled(ch, false);
	}

	SetChannelEnabled(channel_number, true);

	UpdateSpectrumView();
}

void ChartDialog::OnChannelChecked(int channel_number, bool checked)
{
	UpdateSpectrumView();
}

void ChartDialog::UpdateSpectrumView()
{
	if (wxGetApp().GetWorking())
		wxGetApp().UpdateSpectrumView(wxGetApp().GetDevice()->spectrums());
}

#pragma endregion
