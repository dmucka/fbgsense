///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/radiobox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/frame.h>
#include <wx/dataview.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainWindowBase
///////////////////////////////////////////////////////////////////////////////
class MainWindowBase : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticTextAddress;
		wxStaticText* m_staticTextChannels;
		wxStaticText* m_staticTextSampling;
		wxStaticText* m_staticTextSecSamplingRate;
		wxStaticLine* m_staticlineSuppression;
		wxStaticText* m_staticTextDetection;
		wxStaticLine* m_staticlineDetection;
		wxStaticText* m_staticTextRel;
		wxStaticText* m_staticTextRelUnit;
		wxStaticText* m_staticTextLim;
		wxStaticText* m_staticTextLimUnit;
		wxStaticText* m_staticTextSegmentBy;
		wxStaticLine* m_staticlineStats;
		wxStaticText* m_staticTextLoad;
		wxStaticText* m_staticTextMsLoad;
		wxStaticText* m_staticTextPerform;
		wxStaticText* m_staticTextMsPerform;
		wxStaticText* m_staticTextTotal;
		wxStaticText* m_staticTextMsTotal;
		wxButton* m_buttonConnect;
		wxButton* m_buttonStartLog;
		wxButton* m_buttonLoadSpectrum;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void m_checkBoxSuppressOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxForceOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxEqualizeOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_radioBoxSuppressionAlgoOnRadioBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_radioBoxDetectionAlgoOnRadioBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_spinCtrlRelOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_spinCtrlLimOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_choiceSegmentByOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonConnectOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonStartLogOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonLoadSpectrumOnButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxGridBagSizer* gbSizerConnect;
		wxTextCtrl* m_textCtrlAddress;
		wxSpinCtrl* m_spinCtrlChannels;
		wxSpinCtrlDouble* m_spinCtrlSamplingRate;
		wxCheckBox* m_checkBoxSuppress;
		wxCheckBox* m_checkBoxForce;
		wxCheckBox* m_checkBoxEqualize;
		wxRadioBox* m_radioBoxSuppressionAlgo;
		wxRadioBox* m_radioBoxDetectionAlgo;
		wxSpinCtrl* m_spinCtrlRel;
		wxSpinCtrl* m_spinCtrlLim;
		wxChoice* m_choiceSegmentBy;
		wxGridBagSizer* gbSizerStats;
		wxStaticText* m_staticTextStats;
		wxStaticText* m_statLoad;
		wxStaticText* m_statPerform;
		wxStaticText* m_statTotal;

		MainWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("fbgsense-gui"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~MainWindowBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PeaksDialogBase
///////////////////////////////////////////////////////////////////////////////
class PeaksDialogBase : public wxDialog
{
	private:

	protected:
		wxDataViewColumn* m_dataViewListColumnChannel;
		wxDataViewColumn* m_dataViewListColumnIndex;
		wxDataViewColumn* m_dataViewListColumnPosition;
		wxDataViewColumn* m_dataViewListColumnWavelength;
		wxDataViewColumn* m_dataViewListColumnIntensity;
		wxDataViewColumn* m_dataViewListColumnAsymmetry;

		// Virtual event handlers, override them in your derived class
		virtual void OnKeyUp( wxKeyEvent& event ) { event.Skip(); }


	public:
		wxDataViewListCtrl* m_peakDataView;

		PeaksDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Peak Table"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,420 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );

		~PeaksDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ChartDialogBase
///////////////////////////////////////////////////////////////////////////////
class ChartDialogBase : public wxDialog
{
	private:

	protected:
		wxBoxSizer* bSizerChart;
		wxBoxSizer* bSizerHeader;

	public:

		ChartDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Spectrum View"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1250,500 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );

		~ChartDialogBase();

};

