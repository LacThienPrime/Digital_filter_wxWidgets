#pragma once

#include <wx/wx.h>

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
    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, 
        int sample, std::vector<double> ac, std::vector<double> bc);

    std::string title;
    double minX, maxX;
    std::vector<double> a;
    std::vector<double> b;
    
    int sample_freq;
    
    void OnPaintSignal(wxPaintEvent& evt);

private:
    std::tuple<int, double, double> CalSegment(double low, double high);

    bool processing{ false };
    wxCriticalSection dataCs;
    virtual wxThread::ExitCode Entry();

    void OnThreadUpdate(wxThreadEvent&);
    void OnThreadCompletion(wxThreadEvent&);
    void OnThreadCancel(wxThreadEvent&);
};
