#include <wx/wx.h>
<<<<<<< HEAD

=======
#include <chrono>
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
#include <math.h>
#include <cmath>

#include "coef_strategy.h"

Context::Context(CoefStrategy* strategy) : strategy(strategy) {}

<<<<<<< HEAD
std::tuple<std::vector<double>, std::vector<double>> Context::coefCalc(int sample_freq, int cutoff_freq)
=======
std::tuple<std::vector<double>, std::vector<double>> Context::coefCalc(int sample_freq, int cutoff_freq) 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
    return strategy->coefCalc(sample_freq, cutoff_freq);
}

std::tuple<std::vector<double>, std::vector<double>> lowpass::coefCalc(int sample_freq, int cutoff_freq)
{
    std::vector<double> a(3);
    std::vector<double> b(3);

    double omega0 = 2 * M_PI * cutoff_freq;
    double dt = 1.0 / sample_freq;
    double alpha = omega0 * dt;

    double alphaSq = alpha * alpha;
    double beta[] = { 1, sqrt(2), 1 };
    double D = alphaSq * beta[0] + 2 * alpha * beta[1] + 4 * beta[2];

    b[0] = alphaSq / D;
    b[1] = 2 * b[0];
    b[2] = b[0];

    a[1] = -(2 * alphaSq * beta[0] - 8 * beta[2]) / D;
    a[2] = -(beta[0] * alphaSq - 2 * beta[1] * alpha + 4 * beta[2]) / D;

    return std::make_tuple(a, b);
}

<<<<<<< HEAD
std::tuple<std::vector<double>, std::vector<double>> highpass::coefCalc(int sample_freq, int cutoff_freq)
=======
std::tuple<std::vector<double>, std::vector<double>> highpass::coefCalc(int sample_freq, int cutoff_freq) 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
    std::vector<double> a(3);
    std::vector<double> b(3);

    double omega0 = 2 * M_PI * cutoff_freq;
    double dt = 1.0 / sample_freq;

    double dtSq = dt * dt;
    double c[] = { omega0 * omega0, sqrt(2) * omega0, 1 };
    double E = c[0] * dtSq + 2 * c[1] * dt + 4 * c[2];

    b[0] = 4.0 / E;
    b[1] = -8.0 / E;
    b[2] = 4.0 / E;

    a[1] = -(2 * c[0] * dtSq - 8 * c[2]) / E;
    a[2] = -(c[0] * dtSq - 2 * c[1] * dt + 4 * c[2]) / E;

    return std::make_tuple(a, b);
}

<<<<<<< HEAD
std::tuple<std::vector<double>, std::vector<double>> bandpass::coefCalc(int sample_freq, int cutoff_freq)
=======
std::tuple<std::vector<double>, std::vector<double>> bandpass::coefCalc(int sample_freq, int cutoff_freq) 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
    std::vector<double> a(3);
    std::vector<double> b(3);

    int band_width = 6;

    double omega0 = 2 * M_PI * cutoff_freq;
    double dt = 1.0 / sample_freq;
    double alpha = omega0 * dt;

    double domega = 2 * M_PI * band_width;
    double Q = omega0 / domega;
    double G = alpha * alpha + 2 * alpha / Q + 4;

    b[0] = 2 * alpha / (Q * G);
    b[1] = 0;
    b[2] = -(b[0]);

    a[1] = -(2 * pow(alpha, 2) - 8) / G;
    a[2] = -(pow(alpha, 2) - 2 * alpha / Q + 4) / G;

    return std::make_tuple(a, b);
}

<<<<<<< HEAD
std::tuple<std::vector<double>, std::vector<double>> bandstop::coefCalc(int sample_freq, int cutoff_freq)
=======
std::tuple<std::vector<double>, std::vector<double>> bandstop::coefCalc(int sample_freq, int cutoff_freq) 
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
    std::vector<double> a(3);
    std::vector<double> b(3);

    int band_width = 6;

    double omega0 = 2 * M_PI * cutoff_freq;
    double dt = 1.0 / sample_freq;
    double alpha = omega0 * dt;

    double domega = 2 * M_PI * band_width;
    double Q = omega0 / domega;
    double G = alpha * alpha + 2 * alpha / Q + 4;

    b[0] = (alpha * alpha + 4) / G;
    b[1] = (2 * alpha * alpha - 8) / G;
    b[2] = b[0];

    a[1] = -(2 * alpha * alpha - 8) / G;
    a[2] = -(alpha * alpha - 2 * alpha / Q + 4) / G;

    return std::make_tuple(a, b);
}