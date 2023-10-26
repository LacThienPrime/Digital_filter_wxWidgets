#pragma once

#include <wx/wx.h>

#include <vector>
#include <thread>
#include <tuple>
#include <vector>
#include <thread>
#include <chrono>
#include <complex>
#include <stdint.h>
#include <math.h>
#include <cmath>

class FIRfilter : public wxWindow
{
public:
    enum class WindowType
    {
        Rectangular,
        Triangular,
        Welch,
        Sine,
        Hann,
        Hamming,
        Blackman,
        Nuttall,
        BlackmanNuttall,
        BlackmanHarris,
        FlatTop
    };

    enum class FilterType
    {
        LowPass,
        HighPass,
        BandPass,
        BandStop
    };

    WindowType WIN_TYPE;
    FilterType FILT_TYPE;

    std::vector<double> ComputeResponses(int sample, int order, int shift_sample, int cutoff_freq_low, int cutoff_freq_high);
    std::vector<double> ComputeWindow(int order);
    std::vector<double> ComputeWindowedResponses(int sample, std::vector<double> impulseResponse, std::vector<double> window, int order);

    std::tuple<std::vector<double>, std::vector<double>> CalcFIRFilter(std::vector<double> coef, std::vector<double> input);

    void GetCoefFIR(int sample, std::vector<double> impulseResponse, std::vector<double> window, int order);
    void GetFIRFilter(std::vector<double> coef, std::vector<double> input);
};