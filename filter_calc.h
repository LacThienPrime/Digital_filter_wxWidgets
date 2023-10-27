#pragma once

#include <wx/wx.h>

#include <cmath>
#include <tuple>
#include <vector>
#include <complex>
#include <string>

class FilterCalc : public wxWindow
{
public:
    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, 
        int sample, std::vector<double> ac, std::vector<double> bc,  wxCriticalSection& cs);

    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
        int sample, wxCriticalSection& cs);
    
    std::string title;
    double minX, maxX;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> data;

    std::tuple<std::vector<double>, std::vector<double>> SignalCalc(int sample);
    std::tuple<std::vector<double>, std::vector<double>> DFTcalc(std::vector<double> signal);

private:
    void OnPaintSignal(wxPaintEvent& evt);
    void OnPaintDFT(wxPaintEvent& evt);

    int sample_freq;
    wxCriticalSection& valuesCs;
    std::tuple<int, double, double> CalSegment(double low, double high); 
};
