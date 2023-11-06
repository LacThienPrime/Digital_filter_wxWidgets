#pragma once

#include <wx/wx.h>

#include "coef_strategy.h"
#include "fir_filter.h"
#include "filter_calc.h"
#include "my_frame_UI.h"

class MyFrame : public MyFrameUI
{
public:
	MyFrame();

private:
    void OnClose(wxCloseEvent& e);
    void OnStartIIRClicked(wxCommandEvent& e);
    void SelectResponse();

    void DefaultPlot();

    int sample_freq;
    int pass_freq;
    int stop_freq;

    std::vector<double> a;
    std::vector<double> b;
   
    FilterCalc* testSignal;
    FilterCalc* dftSignal;
};

