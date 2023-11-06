#include "fir_filter.h"

std::vector<double> FIRfilter::ComputeResponses
(int sample, int order, int shift_sample, int cutoff_freq_low, int cutoff_freq_high)
{
    const int s = order;
    double sample_freq = 1.0 / sample;

    std::vector<double> impulseResponse(s);

    for (int n = 0; n < order; n++)
    {
        if (n != shift_sample)
        {
            switch (FILT_TYPE)
            {
            case FilterType::LowPass:
                impulseResponse[n] = sin(2.0 * M_PI * cutoff_freq_low * sample_freq * (n - shift_sample))
                    / (M_PI * sample_freq * (n - shift_sample));
                break;

            case FilterType::HighPass:
                impulseResponse[n] = (sin(M_PI * (n - shift_sample)) - sin(2.0 * M_PI * cutoff_freq_low
                    * sample_freq * (n - shift_sample))) / (M_PI * sample_freq * (n - shift_sample));
                break;

            case FilterType::BandPass:
                impulseResponse[n] = (sin(2.0 * M_PI * cutoff_freq_high * sample_freq * (n - shift_sample))
                    - sin(2.0 * M_PI * cutoff_freq_low * sample_freq * (n - shift_sample)))
                    / (M_PI * sample_freq * (n - shift_sample));
                break;

            case FilterType::BandStop:
                impulseResponse[n] = (sin(2.0 * M_PI * cutoff_freq_low * sample_freq * (n - shift_sample))
                    - sin(2.0 * M_PI * cutoff_freq_high * sample_freq * (n - shift_sample))
                    + sin(M_PI * (n - shift_sample))) / (M_PI * sample_freq * (n - shift_sample));
                break;
            }
        }
        else
        {
            switch (FILT_TYPE)
            {
            case FilterType::LowPass:
                impulseResponse[n] = 2.0 * cutoff_freq_low;
                break;

            case FilterType::HighPass:
                impulseResponse[n] = 1.0 / sample_freq - 2.0 * cutoff_freq_low;
                break;

            case FilterType::BandPass:
                impulseResponse[n] = 2.0 * cutoff_freq_high - 2.0 * cutoff_freq_low;
                break;

            case FilterType::BandStop:
                impulseResponse[n] = 2.0 * cutoff_freq_low - 2.0 * cutoff_freq_high + 1.0 / sample_freq;
                break;
            }
        }
    }

    return impulseResponse;
}


/*
*  https://en.wikipedia.org/wiki/Window_function#A_list_of_window_functions
*/
std::vector<double> FIRfilter::ComputeWindow(int order)
{
    std::vector<double> window(order);

    for (int n = 0; n < order; n++)
    {
        switch (WIN_TYPE)
        {
        case WindowType::Rectangular:
            window[n] = 1.0;
            break;

        case WindowType::Triangular:
            window[n] = 1.0 - std::abs((n - 0.5 * order) / (0.5 * order));
            break;

        case WindowType::Welch:
            window[n] = 1.0 - std::pow((n - 0.5 * order) / (0.5 * order), 2.0);
            break;

        case WindowType::Sine:
            window[n] = sin(M_PI * n / ((double)order));
            break;

        case WindowType::Hann:
            window[n] = 0.5 * (1.0 - cos(2.0 * M_PI * n / ((double)order)));
            break;

        case WindowType::Hamming:
            window[n] = (25.0 / 46.0) - (21.0 / 46.0) * cos(2.0 * M_PI * n / ((double)order));
            break;

        case WindowType::Blackman:
            window[n] = 0.42 - 0.5 * cos(2.0 * M_PI * n / ((double)order))
                + 0.08 * cos(4.0 * M_PI * n / ((double)order));
            break;

        case WindowType::Nuttall:
            window[n] = 0.355768 - 0.487396 * cos(2.0 * M_PI * n / ((double)order))
                + 0.144232 * cos(4.0 * M_PI * n / ((double)order))
                - 0.012604 * cos(6.0 * M_PI * n / ((double)order));
            break;

        case WindowType::BlackmanNuttall:
            window[n] = 0.3635819 - 0.4891775 * cos(2.0 * M_PI * n / ((double)order))
                + 0.1365995 * cos(4.0 * M_PI * n / ((double)order))
                - 0.0106411 * cos(6.0 * M_PI * n / ((double)order));
            break;

        case WindowType::BlackmanHarris:
            window[n] = 0.35875 - 0.48829 * cos(2.0 * M_PI * n / ((double)order))
                + 0.14128 * cos(4.0 * M_PI * n / ((double)order))
                - 0.01168 * cos(6.0 * M_PI * n / ((double)order));
            break;

        case WindowType::FlatTop:
            window[n] = 0.21557895 - 0.41663158 * cos(2.0 * M_PI * n / ((double)order))
                + 0.277263158 * cos(4.0 * M_PI * n / ((double)order))
                - 0.083578947 * cos(6.0 * M_PI * n / ((double)order))
                + 0.006947368 * cos(8.0 * M_PI * n / ((double)order));
            break;

        default:
            window[n] = 1.0;
            break;
        }
    }

    return window;
}

std::vector<double> FIRfilter::ComputeWindowedResponses
(int sample, std::vector<double> impulseResponse, std::vector<double> window, int order)
{
    std::vector<double> windowedImpulseResponse(order);

    for (int n = 0; n < order; n++)
    {
        windowedImpulseResponse[n] = impulseResponse[n] * window[n] / sample;
    }

    return windowedImpulseResponse;
}

void FIRfilter::GetCoefFIR(int sample, std::vector<double> impulseResponse, std::vector<double> window, int order)
{
    double value;

    std::vector<double> time;
    std::vector<double> coef = ComputeWindowedResponses(sample, impulseResponse, window, order);

    for (int i = 0; i < order; i++)
    {
        value = double(i) / sample;
        time.push_back(value);
    }
}

std::tuple<std::vector<double>, std::vector<double>> FIRfilter::CalcFIRFilter
(std::vector<double> coef, std::vector<double> input)
{
    int order = coef.size();
    int samp = input.size();

    double time;

    std::vector<double> tt;
    std::vector<double> fir_filtered(samp);

    for (int i = 0; i < samp; i++)
    {
        fir_filtered[i] = 0.0;

        for (int j = 0; j < order; j++)
        {
            if (i + j == samp)
            {
                break;
            }

            fir_filtered[i] += input[i + j] * coef[j];
        }
    }

    for (int i = 0; i < samp; i++)
    {
        time = double(i) / samp;
        tt.push_back(time);
    }

    return std::make_tuple(tt, fir_filtered);
}

void FIRfilter::GetFIRFilter(std::vector<double> coef, std::vector<double> input)
{
    std::vector<double> time = std::get<0>(CalcFIRFilter(coef, input));
    std::vector<double> fir_filtered_signal = std::get<1>(CalcFIRFilter(coef, input));
}