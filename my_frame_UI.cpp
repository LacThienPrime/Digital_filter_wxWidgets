#include "my_frame_UI.h"

MyFrameUI::MyFrameUI(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxSize(900, 620), wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	mainSizer = new wxGridBagSizer(0, 0);
	mainSizer->SetFlexibleDirection(wxHORIZONTAL);
	mainSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);

	mainSizer->SetMinSize(wxSize(890, 610));

	controlSizer = new wxGridBagSizer(0, 0);
	controlSizer->SetFlexibleDirection(wxBOTH);
	controlSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	controlSizer->SetMinSize(wxSize(270, -1));
	controlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(270, 520), wxTAB_TRAVERSAL);
	controlPanel->SetMinSize(wxSize(270, 520));

	gbSizer3 = new wxGridBagSizer(0, 0);
	gbSizer3->SetFlexibleDirection(wxVERTICAL);
	gbSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(controlPanel, wxID_ANY, wxT("FIR SPECIFY FREQUENCY")), wxVERTICAL);

	gSizer1 = new wxGridSizer(5, 2, 0, 0);

	m_staticText1 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Order"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	gSizer1->Add(m_staticText1, 0, wxALL, 5);

	m_textCtrl_order = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxT("64"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer1->Add(m_textCtrl_order, 0, wxALL, 5);

	m_staticText2 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Shift sample"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	gSizer1->Add(m_staticText2, 0, wxALL, 5);

	m_textCtrl_shift_sample = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxT("32"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer1->Add(m_textCtrl_shift_sample, 0, wxALL, 5);

	m_staticText4 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Low freq cut-off"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	gSizer1->Add(m_staticText4, 0, wxALL, 5);

	m_textCtrl_low_cutoff = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer1->Add(m_textCtrl_low_cutoff, 0, wxALL, 5);

	m_staticText5 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("High freq cut-off"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	gSizer1->Add(m_staticText5, 0, wxALL, 5);

	m_textCtrl_high_cutoff = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer1->Add(m_textCtrl_high_cutoff, 0, wxALL, 5);

	m_staticText24 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Filter type"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText24->Wrap(-1);
	gSizer1->Add(m_staticText24, 0, wxALL, 5);

	wxString m_choice_fir_typeChoices[] = { wxT("Rectangular"), wxT("Triangular"), wxT("Welch"), wxT("Sine"), wxT("Hann"), wxT("Hamming"), wxT("Blackman"), wxT("Nuttall"), wxT("BlackmanNuttall"), wxT("BlackmanHarris"), wxT("FlatTop") };
	int m_choice_fir_typeNChoices = sizeof(m_choice_fir_typeChoices) / sizeof(wxString);
	m_choice_fir_type = new wxChoice(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(110, -1), m_choice_fir_typeNChoices, m_choice_fir_typeChoices, 0);
	m_choice_fir_type->SetSelection(0);
	gSizer1->Add(m_choice_fir_type, 0, wxALL, 5);


	sbSizer1->Add(gSizer1, 1, wxEXPAND, 5);


	gbSizer3->Add(sbSizer1, wxGBPosition(0, 0), wxGBSpan(6, 2), wxALIGN_CENTER | wxALL, 5);

	sbSizer11 = new wxStaticBoxSizer(new wxStaticBox(controlPanel, wxID_ANY, wxT("IIR SPECIFY FREQUENCY")), wxVERTICAL);

	gSizer8 = new wxGridSizer(4, 2, 0, 0);

	m_staticText26 = new wxStaticText(sbSizer11->GetStaticBox(), wxID_ANY, wxT("Sample freq"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText26->Wrap(-1);
	gSizer8->Add(m_staticText26, 0, wxALL, 5);

	m_textCtrl_sample = new wxTextCtrl(sbSizer11->GetStaticBox(), wxID_ANY, wxT("1000"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer8->Add(m_textCtrl_sample, 0, wxALL, 5);

	m_staticText21 = new wxStaticText(sbSizer11->GetStaticBox(), wxID_ANY, wxT("Pass freq"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21->Wrap(-1);
	gSizer8->Add(m_staticText21, 0, wxALL, 5);

	m_textCtrl_pass_freq = new wxTextCtrl(sbSizer11->GetStaticBox(), wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer8->Add(m_textCtrl_pass_freq, 0, wxALL, 5);

	m_staticText22 = new wxStaticText(sbSizer11->GetStaticBox(), wxID_ANY, wxT("Stop freq"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText22->Wrap(-1);
	gSizer8->Add(m_staticText22, 0, wxALL, 5);

	m_textCtrl_stop_freq = new wxTextCtrl(sbSizer11->GetStaticBox(), wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer8->Add(m_textCtrl_stop_freq, 0, wxALL, 5);

	m_staticText25 = new wxStaticText(sbSizer11->GetStaticBox(), wxID_ANY, wxT("Filter type"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText25->Wrap(-1);
	gSizer8->Add(m_staticText25, 0, wxALL, 5);

	wxString m_choice_iir_typeChoices[] = { wxT("Butterworth"), wxT("Chebyshev"), wxT("Elliptic") };
	int m_choice_iir_typeNChoices = sizeof(m_choice_iir_typeChoices) / sizeof(wxString);
	m_choice_iir_type = new wxChoice(sbSizer11->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(110, -1), m_choice_iir_typeNChoices, m_choice_iir_typeChoices, 0);
	m_choice_iir_type->SetSelection(0);
	gSizer8->Add(m_choice_iir_type, 0, wxALL, 5);


	sbSizer11->Add(gSizer8, 1, wxEXPAND, 5);


	gbSizer3->Add(sbSizer11, wxGBPosition(7, 0), wxGBSpan(6, 2), wxALIGN_CENTER | wxALL, 5);

	sbSizer12 = new wxStaticBoxSizer(new wxStaticBox(controlPanel, wxID_ANY, wxT("RESPONSE TYPE")), wxVERTICAL);

	gSizer10 = new wxGridSizer(2, 2, 10, 60);

	m_radioBtn_lowpass = new wxRadioButton(sbSizer12->GetStaticBox(), wxID_ANY, wxT("Low-pass"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer10->Add(m_radioBtn_lowpass, 0, wxALL, 5);

	m_radioBtn_highpass = new wxRadioButton(sbSizer12->GetStaticBox(), wxID_ANY, wxT("High-pass"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer10->Add(m_radioBtn_highpass, 0, wxALIGN_CENTER | wxALL, 5);

	m_radioBtn_bandpass = new wxRadioButton(sbSizer12->GetStaticBox(), wxID_ANY, wxT("Band-pass"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer10->Add(m_radioBtn_bandpass, 0, wxALIGN_CENTER | wxALL, 5);

	m_radioBtn_bandstop = new wxRadioButton(sbSizer12->GetStaticBox(), wxID_ANY, wxT("Band-stop"), wxDefaultPosition, wxDefaultSize, 0);
	gSizer10->Add(m_radioBtn_bandstop, 0, wxALL, 5);


	sbSizer12->Add(gSizer10, 1, wxALIGN_CENTER | wxALL, 5);


	gbSizer3->Add(sbSizer12, wxGBPosition(14, 0), wxGBSpan(6, 2), wxALIGN_LEFT | wxALL, 5);


	controlPanel->SetSizer(gbSizer3);
	controlPanel->Layout();
	controlSizer->Add(controlPanel, wxGBPosition(0, 0), wxGBSpan(10, 2), wxEXPAND | wxALL, 5);

	gSizer12 = new wxGridSizer(1, 2, 10, 20);

	gSizer12->SetMinSize(wxSize(100, 30));
	m_button_IIR_start = new wxButton(this, wxID_ANY, wxT("START IIR"), wxDefaultPosition, wxSize(80, 40), 0);
	gSizer12->Add(m_button_IIR_start, 0, wxALIGN_CENTER | wxALL, 5);

	m_button_FIR_start = new wxButton(this, wxID_ANY, wxT("START FIR"), wxDefaultPosition, wxSize(80, 40), 0);
	gSizer12->Add(m_button_FIR_start, 0, wxALL, 5);


	controlSizer->Add(gSizer12, wxGBPosition(10, 0), wxGBSpan(1, 2), wxALIGN_CENTER, 5);


	controlSizer->AddGrowableRow(0);
	controlSizer->AddGrowableRow(1);
	controlSizer->AddGrowableRow(2);
	controlSizer->AddGrowableRow(3);
	controlSizer->AddGrowableRow(4);
	controlSizer->AddGrowableRow(5);
	controlSizer->AddGrowableRow(6);
	controlSizer->AddGrowableRow(7);
	controlSizer->AddGrowableRow(8);
	controlSizer->AddGrowableRow(9);

	mainSizer->Add(controlSizer, wxGBPosition(0, 0), wxGBSpan(4, 2), wxALL | wxEXPAND, 5);

	plotSizer = new wxGridSizer(2, 1, 20, 0);

	plotSizer->SetMinSize(wxSize(500, 250));
	filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	filterPanel->SetMinSize(wxSize(500, 250));
	filterPanel->SetMaxSize(wxSize(500, 250));

	filterSizer = new wxBoxSizer(wxHORIZONTAL);

	filterSizer->SetMinSize(wxSize(500, 250));

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();
	filterSizer->Fit(filterPanel);
	plotSizer->Add(filterPanel, 1, wxEXPAND | wxALL, 5);

	dftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	dftPanel->SetMinSize(wxSize(500, 250));
	dftPanel->SetMaxSize(wxSize(500, 250));

	dftSizer = new wxBoxSizer(wxHORIZONTAL);

	dftSizer->SetMinSize(wxSize(500, 250));

	dftPanel->SetSizer(dftSizer);
	dftPanel->Layout();
	dftSizer->Fit(dftPanel);
	plotSizer->Add(dftPanel, 1, wxEXPAND | wxALL, 5);


	mainSizer->Add(plotSizer, wxGBPosition(0, 2), wxGBSpan(4, 6), wxEXPAND, 5);


	mainSizer->AddGrowableCol(2);
	mainSizer->AddGrowableCol(3);
	mainSizer->AddGrowableCol(4);
	mainSizer->AddGrowableCol(5);
	mainSizer->AddGrowableCol(6);
	mainSizer->AddGrowableCol(7);
	mainSizer->AddGrowableRow(0);
	mainSizer->AddGrowableRow(1);
	mainSizer->AddGrowableRow(2);
	mainSizer->AddGrowableRow(3);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_button_IIR_start->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameUI::OnStartIIRClicked), NULL, this);
	m_button_FIR_start->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameUI::OnStartFIRClicked), NULL, this);
}

MyFrameUI::~MyFrameUI()
{
	// Disconnect Events
	m_button_IIR_start->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameUI::OnStartIIRClicked), NULL, this);
	m_button_FIR_start->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameUI::OnStartFIRClicked), NULL, this);

}
