#pragma once

#include <wx/wx.h>

#include <thread>

#include "coef_strategy.h"
#include "fir_filter.h"
#include "filter_calc.h"
#include "my_frame_UI.h"

class MyFrame : public MyFrameUI
{
public:
	MyFrame();
    ~MyFrame();

private:
    void OnClose(wxCloseEvent& e);
    void OnStartIIRClicked(wxCommandEvent& e);
    void OnStartFIRClicked(wxCommandEvent& e);
    void GetInputValue();
    void SelectResponse();
    
    FilterCalc* testSignal;
    FilterCalc* dftSignal;
    FIRfilter* fir;

    /*
    *  IIR parameters
    */
    int sample_freq;
    int pass_freq;
    int stop_freq;

    std::vector<double> a;
    std::vector<double> b;

    /*
    *  IIR parameters
    */
    int order;
    int shift_sample;
    int low_cutoff_freq;
    int high_cutoff_freq;

    std::vector<double> responses;
    std::vector<double> window;
    std::vector<double> window_responses;
};

