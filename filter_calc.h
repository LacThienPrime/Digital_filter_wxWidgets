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
        int sample, std::vector<double> ac, std::vector<double> bc);

    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
        int sample);
    
    std::string title;
    double minX, maxX;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> data;

private:
    void OnPaintSignal(wxPaintEvent& evt);
    void OnPaintDFT(wxPaintEvent& evt);
    void OnPaint(wxPaintEvent& evt, bool isDFT);

    int sample_freq;

    std::tuple<int, double, double> CalSegment(double low, double high); 
};
