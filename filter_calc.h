#pragma once

#include <wx/wx.h>
#include <wx/list.h>

#include <cmath>
#include <tuple>
#include <vector>
#include <complex>
#include <string>

wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_CANCELLED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_SORTINGTHREAD_UPDATED, wxThreadEvent);

class FilterCalc : public wxWindow, public wxThreadHelper
{
public:
    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, bool IIR);

    int sample;
    int order;
    std::string title;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> coef;

    void OnPaintSignal(wxPaintEvent& evt, bool isIIR);
    void OnPaintIIR(wxPaintEvent& evt);
    void OnPaintFIR(wxPaintEvent& evt);
    void UpdateIIRValues(int s, std::vector<double>& ac, std::vector<double>& bc);
    void UpdateFIRValues(int s, int o, std::vector<double>& co);

private:    
    std::tuple<int, double, double> CalSegment(double low, double high);

    wxColour barColor;
    wxRect2DDouble chartArea;

    bool processing{ false };
    virtual wxThread::ExitCode Entry();
    void OnThreadUpdate(wxThreadEvent&);
    void OnThreadCompletion(wxThreadEvent&);
    void OnThreadCancel(wxThreadEvent&);
};
