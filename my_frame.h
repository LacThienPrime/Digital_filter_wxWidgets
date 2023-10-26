#pragma once

#include <wx/wx.h>

#include "coef_strategy.h"
#include "fir_filter.h"
#include "filter_calc.h"
#include "my_frame_UI.h"

wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_CANCELLED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_UPDATED, wxThreadEvent);

class MyFrame : public MyFrameUI, public wxThreadHelper
{
public:
	MyFrame();
	//virtual ~MyFrame();

private:
    bool processing{ false };

    wxCriticalSection dataCs;

    virtual wxThread::ExitCode Entry();

    void OnThreadUpdate(wxThreadEvent&);
    void OnThreadCompletion(wxThreadEvent&);
    void OnThreadCancel(wxThreadEvent&);

    void OnClose(wxCloseEvent& e);
	void OnStartIIRClicked(wxCommandEvent& e);
    //void OnStartFIRClicked(wxCommandEvent& e);
    void GetInputValue();
    void SelectResponse();

    int sample_freq;
    int pass_freq;
    int stop_freq;
    int filter_order;
    int shift_sample;
    int low_cutoff_freq;
    int high_cutoff_freq;

    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> tt;
    std::vector<double> yy;
    std::vector<double> yy_filtered;

    std::vector<double> response;
    std::vector<double> window;
    std::vector<double> fir_coef;
    std::vector<double> fir_time;
    std::vector<double> fir_signal;
   
    FilterCalc* testSignal;
    FilterCalc* dft;
};

