/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <wx/wx.h>
#include "plotColors.h"

class wxChannelCtrl : public wxPanel
{
	typedef std::function<void(int, bool)> checkedChangedFunc;

public:
	wxChannelCtrl(wxWindow* parent, wxWindowID id, int channel_number, checkedChangedFunc onChecked, checkedChangedFunc onRightClicked)
		: wxPanel(parent, id), m_onChecked(onChecked), m_channelNumber(channel_number), m_onRightClicked(onRightClicked)
	{
		this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

		wxBoxSizer* bSizerCH;
		bSizerCH = new wxBoxSizer(wxVERTICAL);

		m_checkBoxCH = new wxCheckBox(this, wxID_ANY, wxT("CH " + std::to_string(channel_number)), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		bSizerCH->Add(m_checkBoxCH, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM | wxRIGHT, this->FromDIP(5));

		m_textCtrlCH = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, this->FromDIP(wxSize(25, -1)), wxTE_CENTER | wxTE_READONLY);
		m_textCtrlCH->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

		bSizerCH->Add(m_textCtrlCH, 0, wxALIGN_CENTER_HORIZONTAL, this->FromDIP(5));

		m_checkBoxCH->Bind(wxEVT_CHECKBOX, [&](wxCommandEvent& event)
			{
				if (m_onChecked != nullptr)
					m_onChecked(m_channelNumber, GetChecked());
			});

		m_checkBoxCH->Bind(wxEVT_RIGHT_UP, [&](wxMouseEvent& event)
			{
				if (m_onRightClicked != nullptr)
					m_onRightClicked(m_channelNumber, GetChecked());
			});

		this->SetSizer(bSizerCH);
		this->Layout();
	}

	wxCheckBox* GetCheckBox()
	{
		return m_checkBoxCH;
	}

	wxTextCtrl* GetTextCtrl()
	{
		return m_textCtrlCH;
	}

	bool GetChecked()
	{
		return m_checkBoxCH->GetValue();
	}

	void SetChecked(bool value)
	{
		m_checkBoxCH->SetValue(value);
	}

	void SetPeakCount(int count)
	{
		m_textCtrlCH->SetValue(wxString(std::to_string(count)));
	}

	void SetColor(int color)
	{
		glm::vec3 rgb = PlotColors::color_scheme[color];
		m_checkBoxCH->SetForegroundColour(wxColour(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255));
	}

protected:
	wxCheckBox* m_checkBoxCH;
	wxTextCtrl* m_textCtrlCH;

	checkedChangedFunc m_onRightClicked;
	checkedChangedFunc m_onChecked;
	int m_channelNumber;
};
