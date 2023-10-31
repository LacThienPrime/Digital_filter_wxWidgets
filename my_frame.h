#pragma once

#include <wx/wx.h>

#include "coef_strategy.h"
#include "fir_filter.h"
#include "filter_calc.h"
#include "my_frame_UI.h"

wxDECLARE_EVENT(wxEVT_SORTING_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTING_CANCELLED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTING_UPDATED, wxThreadEvent);

class MyFrame : public MyFrameUI, public wxThreadHelper
{
public:
	MyFrame();
	//virtual ~MyFrame();

private:
    bool processing{ false };
    virtual wxThread::ExitCode Entry();

    void OnThreadUpdate(wxThreadEvent&);
    void OnThreadCompletion(wxThreadEvent&);
    void OnThreadCancel(wxThreadEvent&);

    void OnClose(wxCloseEvent& e);
	void OnStartIIRClicked(wxCommandEvent& e);
    void GetInputValue();
    void SelectResponse();

    int sample_freq;
    int pass_freq;
    int stop_freq;

    std::vector<double> a;
    std::vector<double> b;
   
    FilterCalc* testSignal;
};

