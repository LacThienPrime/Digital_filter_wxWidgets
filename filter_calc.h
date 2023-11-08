#pragma once

#include <wx/wx.h>

#include <list>
#include <cmath>
#include <tuple>
#include <vector>
#include <complex>
#include <string>

class FilterCalc : public wxWindow
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
};
