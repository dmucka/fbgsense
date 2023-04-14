///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "fbgsense-gui.gen.h"

///////////////////////////////////////////////////////////////////////////

MainWindowBase::MainWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints(this->FromDIP(wxSize( -1,-1 )), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxGridBagSizer* gbSizerMain;
	gbSizerMain = new wxGridBagSizer( 0, 0 );
	gbSizerMain->SetFlexibleDirection( wxBOTH );
	gbSizerMain->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	gbSizerMain->SetMinSize( this->FromDIP(wxSize( 240,470 )) );

	gbSizerMain->Add(0, this->FromDIP(15), wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	gbSizerConnect = new wxGridBagSizer( 0, 0 );
	gbSizerConnect->SetFlexibleDirection( wxBOTH );
	gbSizerConnect->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizerConnect;
	fgSizerConnect = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerConnect->SetFlexibleDirection( wxBOTH );
	fgSizerConnect->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextAddress = new wxStaticText( this, wxID_ANY, wxT("IP Address"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAddress->Wrap( -1 );
	fgSizerConnect->Add( m_staticTextAddress, 0, wxALL, this->FromDIP(5) );

	m_textCtrlAddress = new wxTextCtrl( this, wxID_ANY, wxT("10.88.1.50"), wxDefaultPosition, this->FromDIP(wxSize( 65,-1 )), 0 );
	fgSizerConnect->Add( m_textCtrlAddress, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, this->FromDIP(5) );

	m_staticTextChannels = new wxStaticText( this, wxID_ANY, wxT("Channels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextChannels->Wrap( -1 );
	fgSizerConnect->Add( m_staticTextChannels, 0, wxALL, this->FromDIP(5) );

	m_spinCtrlChannels = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, this->FromDIP(wxSize( 52,-1 )), wxALIGN_CENTER_HORIZONTAL|wxSP_ARROW_KEYS, 0, 16, 16 );
	fgSizerConnect->Add( m_spinCtrlChannels, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, this->FromDIP(5) );

	m_staticTextSampling = new wxStaticText( this, wxID_ANY, wxT("Sampling rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSampling->Wrap( -1 );
	fgSizerConnect->Add( m_staticTextSampling, 0, wxALIGN_CENTER|wxALL, this->FromDIP(5) );

	wxBoxSizer* bSizerSamplingRate;
	bSizerSamplingRate = new wxBoxSizer( wxHORIZONTAL );

	m_spinCtrlSamplingRate = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, this->FromDIP(wxSize( 52,-1 )), wxALIGN_CENTER_HORIZONTAL|wxSP_ARROW_KEYS, 0, 9999, 0.000000, 0.1 );
	m_spinCtrlSamplingRate->SetDigits( 1 );
	bSizerSamplingRate->Add( m_spinCtrlSamplingRate, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, this->FromDIP(5) );

	m_staticTextSecSamplingRate = new wxStaticText( this, wxID_ANY, wxT("s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSecSamplingRate->Wrap( -1 );
	bSizerSamplingRate->Add( m_staticTextSecSamplingRate, 0, wxALIGN_CENTER|wxALL, this->FromDIP(5) );


	fgSizerConnect->Add( bSizerSamplingRate, 1, wxEXPAND, this->FromDIP(5) );


	gbSizerConnect->Add( fgSizerConnect, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );


	gbSizerConnect->AddGrowableCol( 0 );

	gbSizerMain->Add( gbSizerConnect, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxEXPAND, this->FromDIP(5) );

	wxGridBagSizer* gbSizerSuppression;
	gbSizerSuppression = new wxGridBagSizer( 0, 0 );
	gbSizerSuppression->SetFlexibleDirection( wxBOTH );
	gbSizerSuppression->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	gbSizerSuppression->Add( 0, this->FromDIP(5), wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	m_checkBoxSuppress = new wxCheckBox( this, wxID_ANY, wxT("False Peaks Suppression    "), wxDefaultPosition, this->FromDIP(wxSize( -1,-1 )), wxALIGN_RIGHT );
	m_checkBoxSuppress->SetValue(true);
	gbSizerSuppression->Add( m_checkBoxSuppress, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxTOP, this->FromDIP(5) );

	m_staticlineSuppression = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizerSuppression->Add( m_staticlineSuppression, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, this->FromDIP(5) );

	wxBoxSizer* bSizerFitOptions;
	bSizerFitOptions = new wxBoxSizer(wxHORIZONTAL);

	m_checkBoxForce = new wxCheckBox(this, wxID_ANY, wxT("Force"), wxDefaultPosition, wxSize(-1, -1), 0);
	m_checkBoxForce->SetValue(true);
	bSizerFitOptions->Add(m_checkBoxForce, 0, wxALL, this->FromDIP(5));

	m_checkBoxEqualize = new wxCheckBox(this, wxID_ANY, wxT("Equalize"), wxDefaultPosition, wxDefaultSize, 0);
	m_checkBoxEqualize->SetValue(true);
	bSizerFitOptions->Add(m_checkBoxEqualize, 0, wxALL, this->FromDIP(5));


	gbSizerSuppression->Add(bSizerFitOptions, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER, 5);

	wxString m_radioBoxSuppressionAlgoChoices[] = { wxT("Bigaussian"), wxT("Gaussian"), wxT("Linear") };
	int m_radioBoxSuppressionAlgoNChoices = sizeof(m_radioBoxSuppressionAlgoChoices) / sizeof(wxString);
	m_radioBoxSuppressionAlgo = new wxRadioBox(this, wxID_ANY, wxT("Select Fit"), wxDefaultPosition, wxDefaultSize, m_radioBoxSuppressionAlgoNChoices, m_radioBoxSuppressionAlgoChoices, 1, wxRA_SPECIFY_ROWS);
	m_radioBoxSuppressionAlgo->SetSelection( 0 );
	m_radioBoxSuppressionAlgo->SetMaxSize(this->FromDIP(wxSize(220, -1)));
	gbSizerSuppression->Add( m_radioBoxSuppressionAlgo, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER, this->FromDIP(5) );

	wxFlexGridSizer* fgSizerSegmentation;
	fgSizerSegmentation = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerSegmentation->SetFlexibleDirection( wxBOTH );
	fgSizerSegmentation->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextSegmentBy = new wxStaticText( this, wxID_ANY, wxT("Segment by"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSegmentBy->Wrap( -1 );
	fgSizerSegmentation->Add( m_staticTextSegmentBy, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, this->FromDIP(5) );

	wxString m_choiceSegmentByChoices[] = { wxT("Local minima"), wxT("Inflection points") };
	int m_choiceSegmentByNChoices = sizeof( m_choiceSegmentByChoices ) / sizeof( wxString );
	m_choiceSegmentBy = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSegmentByNChoices, m_choiceSegmentByChoices, 0 );
	m_choiceSegmentBy->SetSelection( 0 );
	fgSizerSegmentation->Add( m_choiceSegmentBy, 0, wxALIGN_CENTER|wxLEFT, this->FromDIP(5) );


	gbSizerSuppression->Add( fgSizerSegmentation, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxEXPAND|wxTOP, this->FromDIP(5) );

	gbSizerSuppression->AddGrowableCol( 0 );

	gbSizerMain->Add( gbSizerSuppression, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	wxGridBagSizer* gbSizerDetection;
	gbSizerDetection = new wxGridBagSizer( 0, 0 );
	gbSizerDetection->SetFlexibleDirection( wxBOTH );
	gbSizerDetection->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	gbSizerDetection->Add( 0, 5, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(8) );

	m_staticTextDetection = new wxStaticText( this, wxID_ANY, wxT("Detection Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDetection->Wrap( -1 );
	gbSizerDetection->Add( m_staticTextDetection, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxTOP, this->FromDIP(5) );

	m_staticlineDetection = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizerDetection->Add( m_staticlineDetection, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, this->FromDIP(5) );

	wxString m_radioBoxDetectionAlgoChoices[] = { wxT("FFT"), wxT("Savgol") };
	int m_radioBoxDetectionAlgoNChoices = sizeof(m_radioBoxDetectionAlgoChoices) / sizeof(wxString);
	m_radioBoxDetectionAlgo = new wxRadioBox(this, wxID_ANY, wxT("Select Method"), wxDefaultPosition, wxDefaultSize, m_radioBoxDetectionAlgoNChoices, m_radioBoxDetectionAlgoChoices, 1, wxRA_SPECIFY_ROWS);
	m_radioBoxDetectionAlgo->SetSelection(0);
	gbSizerDetection->Add(m_radioBoxDetectionAlgo, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxBOTTOM, this->FromDIP(5));

	wxFlexGridSizer* fgSizerDetection;
	fgSizerDetection = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerDetection->SetFlexibleDirection( wxBOTH );
	fgSizerDetection->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextRel = new wxStaticText( this, wxID_ANY, wxT("Relative threshold"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRel->Wrap( -1 );
	fgSizerDetection->Add( m_staticTextRel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, this->FromDIP(5) );

	wxBoxSizer* bSizerRel;
	bSizerRel = new wxBoxSizer( wxHORIZONTAL );

	m_spinCtrlRel = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, this->FromDIP(wxSize( 52,-1 )), wxALIGN_CENTER_HORIZONTAL|wxSP_ARROW_KEYS, 0, 9999, 5 );
	bSizerRel->Add( m_spinCtrlRel, 0, wxALIGN_CENTER, this->FromDIP(5) );

	m_staticTextRelUnit = new wxStaticText( this, wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRelUnit->Wrap( -1 );
	bSizerRel->Add( m_staticTextRelUnit, 0, wxALIGN_CENTER|wxLEFT, this->FromDIP(5) );


	fgSizerDetection->Add( bSizerRel, 1, wxEXPAND|wxLEFT, this->FromDIP(5) );

	m_staticTextLim = new wxStaticText( this, wxID_ANY, wxT("Filtering parameter"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLim->Wrap( -1 );
	fgSizerDetection->Add( m_staticTextLim, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, this->FromDIP(5) );

	wxBoxSizer* bSizerLim;
	bSizerLim = new wxBoxSizer( wxHORIZONTAL );

	m_spinCtrlLim = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, this->FromDIP(wxSize( 52,-1 )), wxALIGN_CENTER_HORIZONTAL|wxSP_ARROW_KEYS, 0, 99999, 5 );
	bSizerLim->Add( m_spinCtrlLim, 0, wxALIGN_CENTER, this->FromDIP(5) );

	m_staticTextLimUnit = new wxStaticText( this, wxID_ANY, wxT("units"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLimUnit->Wrap( -1 );
	bSizerLim->Add( m_staticTextLimUnit, 0, wxALIGN_CENTER|wxLEFT, this->FromDIP(5) );

	fgSizerDetection->Add(bSizerLim, 1, wxEXPAND | wxLEFT, this->FromDIP(5));

	gbSizerDetection->Add( fgSizerDetection, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxEXPAND, this->FromDIP(5) );


	gbSizerDetection->AddGrowableCol( 0 );

	gbSizerMain->Add( gbSizerDetection, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	gbSizerStats = new wxGridBagSizer( 0, 0 );
	gbSizerStats->SetFlexibleDirection( wxVERTICAL );
	gbSizerStats->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextStats = new wxStaticText( this, wxID_ANY, wxT("Statistics"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextStats->Wrap( -1 );
	gbSizerStats->Add( m_staticTextStats, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxEXPAND|wxTOP, this->FromDIP(5) );

	m_staticlineStats = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizerStats->Add( m_staticlineStats, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, this->FromDIP(5) );

	wxGridSizer* gSizerStatLoad;
	gSizerStatLoad = new wxGridSizer( 0, 2, 0, 0 );

	m_staticTextLoad = new wxStaticText( this, wxID_ANY, wxT("Load:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLoad->Wrap( -1 );
	gSizerStatLoad->Add( m_staticTextLoad, 0, wxALIGN_RIGHT, this->FromDIP(5) );

	wxBoxSizer* bSizerLoadStat;
	bSizerLoadStat = new wxBoxSizer( wxHORIZONTAL );

	m_statLoad = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, this->FromDIP(wxSize( -1,-1 )), 0 );
	m_statLoad->Wrap( -1 );
	bSizerLoadStat->Add( m_statLoad, 0, wxLEFT, this->FromDIP(5) );

	m_staticTextMsLoad = new wxStaticText( this, wxID_ANY, wxT("ms"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMsLoad->Wrap( -1 );
	bSizerLoadStat->Add( m_staticTextMsLoad, 0, wxLEFT, this->FromDIP(5) );


	gSizerStatLoad->Add( bSizerLoadStat, 1, wxEXPAND, this->FromDIP(5) );


	gbSizerStats->Add( gSizerStatLoad, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxBOTTOM|wxEXPAND, this->FromDIP(2) );

	wxGridSizer* gSizerStatPerform;
	gSizerStatPerform = new wxGridSizer( 0, 2, 0, 0 );

	m_staticTextPerform = new wxStaticText( this, wxID_ANY, wxT("Perform:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPerform->Wrap( -1 );
	gSizerStatPerform->Add( m_staticTextPerform, 0, wxALIGN_RIGHT, this->FromDIP(5) );

	wxBoxSizer* bSizerPerformStat;
	bSizerPerformStat = new wxBoxSizer( wxHORIZONTAL );

	m_statPerform = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_statPerform->Wrap( -1 );
	bSizerPerformStat->Add( m_statPerform, 0, wxLEFT, this->FromDIP(5) );

	m_staticTextMsPerform = new wxStaticText( this, wxID_ANY, wxT("ms"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMsPerform->Wrap( -1 );
	bSizerPerformStat->Add( m_staticTextMsPerform, 0, wxLEFT, this->FromDIP(5) );


	gSizerStatPerform->Add( bSizerPerformStat, 1, wxEXPAND, this->FromDIP(5) );


	gbSizerStats->Add( gSizerStatPerform, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxBOTTOM|wxEXPAND, this->FromDIP(2) );

	wxGridSizer* gSizerStatTotal;
	gSizerStatTotal = new wxGridSizer( 0, 2, 0, 0 );

	m_staticTextTotal = new wxStaticText( this, wxID_ANY, wxT("Total:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTotal->Wrap( -1 );
	gSizerStatTotal->Add( m_staticTextTotal, 0, wxALIGN_RIGHT, this->FromDIP(5) );

	wxBoxSizer* bSizerTotalStat;
	bSizerTotalStat = new wxBoxSizer( wxHORIZONTAL );

	m_statTotal = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_statTotal->Wrap( -1 );
	bSizerTotalStat->Add( m_statTotal, 0, wxLEFT, this->FromDIP(5) );

	m_staticTextMsTotal = new wxStaticText( this, wxID_ANY, wxT("ms"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMsTotal->Wrap( -1 );
	bSizerTotalStat->Add( m_staticTextMsTotal, 0, wxLEFT, this->FromDIP(5) );


	gSizerStatTotal->Add( bSizerTotalStat, 1, wxEXPAND, this->FromDIP(5) );


	gbSizerStats->Add( gSizerStatTotal, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );


	gbSizerStats->AddGrowableCol( 0 );

	gbSizerMain->Add( gbSizerStats, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	wxGridBagSizer* gbSizerButtons;
	gbSizerButtons = new wxGridBagSizer( 0, 0 );
	gbSizerButtons->SetFlexibleDirection( wxVERTICAL );
	gbSizerButtons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	gbSizerButtons->Add( 0, this->FromDIP(5), wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 0, 2, 0, 0 );

	m_buttonConnect = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, this->FromDIP(wxSize( -1,30 )), 0 );
	gSizer6->Add( m_buttonConnect, 0, wxALL|wxEXPAND, this->FromDIP(5) );

	m_buttonStartLog = new wxButton( this, wxID_ANY, wxT("Start logging"), wxDefaultPosition, this->FromDIP(wxSize( -1,30 )), 0 );
	m_buttonStartLog->Enable( false );

	gSizer6->Add( m_buttonStartLog, 0, wxALL|wxEXPAND, this->FromDIP(5) );


	gbSizerButtons->Add( gSizer6, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );

	m_buttonLoadSpectrum = new wxButton( this, wxID_ANY, wxT("Load spectrum"), wxDefaultPosition, this->FromDIP(wxSize( -1,30 )), 0 );
	gbSizerButtons->Add( m_buttonLoadSpectrum, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, this->FromDIP(5) );


	gbSizerButtons->AddGrowableCol( 0 );

	gbSizerMain->Add( gbSizerButtons, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );


	gbSizerMain->Add( 0, this->FromDIP(5), wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, this->FromDIP(5) );


	gbSizerMain->AddGrowableCol( 0 );

	this->SetSizer( gbSizerMain );
	this->Layout();
	gbSizerMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindowBase::OnClose));
	m_checkBoxSuppress->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxSuppressOnCheckBox), NULL, this);
	m_checkBoxForce->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxForceOnCheckBox), NULL, this);
	m_checkBoxEqualize->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxEqualizeOnCheckBox), NULL, this);
	m_radioBoxSuppressionAlgo->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(MainWindowBase::m_radioBoxSuppressionAlgoOnRadioBox), NULL, this);
	m_radioBoxDetectionAlgo->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(MainWindowBase::m_radioBoxDetectionAlgoOnRadioBox), NULL, this);
	m_spinCtrlRel->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MainWindowBase::m_spinCtrlRelOnSpinCtrl), NULL, this);
	m_spinCtrlLim->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MainWindowBase::m_spinCtrlLimOnSpinCtrl), NULL, this);
	m_choiceSegmentBy->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(MainWindowBase::m_choiceSegmentByOnChoice), NULL, this);
	m_buttonConnect->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonConnectOnButtonClick), NULL, this);
	m_buttonStartLog->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonStartLogOnButtonClick), NULL, this);
	m_buttonLoadSpectrum->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonLoadSpectrumOnButtonClick), NULL, this);
}

MainWindowBase::~MainWindowBase()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindowBase::OnClose));
	m_checkBoxSuppress->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxSuppressOnCheckBox), NULL, this);
	m_checkBoxForce->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxForceOnCheckBox), NULL, this);
	m_checkBoxEqualize->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainWindowBase::m_checkBoxEqualizeOnCheckBox), NULL, this);
	m_radioBoxSuppressionAlgo->Disconnect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(MainWindowBase::m_radioBoxSuppressionAlgoOnRadioBox), NULL, this);
	m_radioBoxDetectionAlgo->Disconnect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(MainWindowBase::m_radioBoxDetectionAlgoOnRadioBox), NULL, this);
	m_spinCtrlRel->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MainWindowBase::m_spinCtrlRelOnSpinCtrl), NULL, this);
	m_spinCtrlLim->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MainWindowBase::m_spinCtrlLimOnSpinCtrl), NULL, this);
	m_choiceSegmentBy->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(MainWindowBase::m_choiceSegmentByOnChoice), NULL, this);
	m_buttonConnect->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonConnectOnButtonClick), NULL, this);
	m_buttonStartLog->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonStartLogOnButtonClick), NULL, this);
	m_buttonLoadSpectrum->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindowBase::m_buttonLoadSpectrumOnButtonClick), NULL, this);

}

PeaksDialogBase::PeaksDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints(this->FromDIP(wxSize( 420,-1 )), wxDefaultSize );

	wxGridSizer* gSizerPeaks;
	gSizerPeaks = new wxGridSizer( 0, 1, 0, 0 );

	m_peakDataView = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES|wxDV_MULTIPLE|wxDV_VERT_RULES );
	m_dataViewListColumnChannel = m_peakDataView->AppendTextColumn( wxT("Channel"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnIndex = m_peakDataView->AppendTextColumn( wxT("Index"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnPosition = m_peakDataView->AppendTextColumn( wxT("Position"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnWavelength = m_peakDataView->AppendTextColumn( wxT("Wavelength"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnIntensity = m_peakDataView->AppendTextColumn( wxT("Intensity"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnAsymmetry = m_peakDataView->AppendTextColumn(wxT("Asymmetry"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	gSizerPeaks->Add( m_peakDataView, 0, wxEXPAND, this->FromDIP(5) );


	this->SetSizer( gSizerPeaks );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CHAR_HOOK, wxKeyEventHandler( PeaksDialogBase::OnKeyUp ) );
}

PeaksDialogBase::~PeaksDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR_HOOK, wxKeyEventHandler( PeaksDialogBase::OnKeyUp ) );

}

ChartDialogBase::ChartDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints(this->FromDIP(wxSize( 1250,500 )), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	bSizerChart = new wxBoxSizer( wxVERTICAL );

	bSizerHeader = new wxBoxSizer( wxHORIZONTAL );


	bSizerChart->Add( bSizerHeader, 0, wxALIGN_CENTER, this->FromDIP(5) );


	this->SetSizer( bSizerChart );
	this->Layout();

	this->Centre( wxBOTH );
}

ChartDialogBase::~ChartDialogBase()
{
}
