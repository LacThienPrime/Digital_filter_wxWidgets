#pragma once

#include <wx/wx.h>

#include <cmath>
#include <tuple>
#include <vector>
#include <complex>
<<<<<<< HEAD
#include <string>

class FilterCalc : public wxWindow
{
public:
    FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, 
        int sample, std::vector<double> ac, std::vector<double> bc,  wxCriticalSection& cs);

    std::string title;
    double minX, maxX;
    std::vector<double> a;
    std::vector<double> b;

private:
    void OnPaint(wxPaintEvent& evt);

    int sample_freq;
    wxCriticalSection& valuesCs;
    std::tuple<int, double, double> CalSegment(double low, double high); 
=======

#include "mathplot.h"

class FilterCalc : public mpFXYVector
{
public:
    std::tuple<std::vector<double>, std::vector<double>> CalcTestSignal(int sample);

    void GetTestSignal(int sample);

    std::tuple<std::vector<double>, std::vector<double>> CalcFilteredSignal(std::tuple<std::vector<double>, std::vector<double>> test_signal,
        std::vector<double> a, std::vector<double> b);

    void GetFilteredSignal(std::tuple<std::vector<double>, std::vector<double>> test_signal,
        std::vector<double> a, std::vector<double> b);

    void CalcDFT(std::vector<double> time, std::vector<double> signal);
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
};

