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
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/radiobut.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/frame.h>

class MyFrameUI : public wxFrame
{
private:

protected:
	wxGridBagSizer* mainSizer;
	wxGridBagSizer* controlSizer;
	wxGridBagSizer* gbSizer3;
	wxStaticBoxSizer* sbSizer1;
	wxGridSizer* gSizer1;
	wxStaticBoxSizer* sbSizer11;
	wxGridSizer* gSizer8;
	wxStaticBoxSizer* sbSizer12;
	wxGridSizer* gSizer10;
	wxGridSizer* gSizer12;
	wxGridSizer* plotSizer;
	wxBoxSizer* filterSizer;
	wxBoxSizer* dftSizer;
	wxPanel* controlPanel;
	wxStaticText* m_staticText1;
	wxTextCtrl* m_textCtrl_order;
	wxStaticText* m_staticText2;
	wxTextCtrl* m_textCtrl_shift_sample;
	wxStaticText* m_staticText4;
	wxTextCtrl* m_textCtrl_low_cutoff;
	wxStaticText* m_staticText5;
	wxTextCtrl* m_textCtrl_high_cutoff;
	wxStaticText* m_staticText24;
	wxChoice* m_choice_fir_type;
	wxStaticText* m_staticText26;
	wxTextCtrl* m_textCtrl_sample;
	wxStaticText* m_staticText21;
	wxTextCtrl* m_textCtrl_pass_freq;
	wxStaticText* m_staticText22;
	wxTextCtrl* m_textCtrl_stop_freq;
	wxStaticText* m_staticText25;
	wxChoice* m_choice_iir_type;
	wxRadioButton* m_radioBtn_lowpass;
	wxRadioButton* m_radioBtn_highpass;
	wxRadioButton* m_radioBtn_bandpass;
	wxRadioButton* m_radioBtn_bandstop;
	wxButton* m_button_IIR_start;
	wxButton* m_button_FIR_start;
	wxPanel* filterPanel;
	wxPanel* dftPanel;

	// Virtual event handlers, override them in your derived class
	virtual void OnStartIIRClicked(wxCommandEvent& event) { event.Skip(); }
	virtual void OnStartFIRClicked(wxCommandEvent& event) { event.Skip(); }

public:
	MyFrameUI(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(900, 620), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MyFrameUI();
};

