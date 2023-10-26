<<<<<<< HEAD
#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "filter_calc.h"

FilterCalc::FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int sample, std::vector<double> ac, std::vector<double> bc, wxCriticalSection& cs)
	: wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE), sample_freq(sample), a(ac), b(bc), valuesCs(cs)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_PAINT, &FilterCalc::OnPaint, this);
}

void FilterCalc::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2.0,
			wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

		gc->SetFont(titleFont, wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);

		double tw, th;
		gc->GetTextExtent(this->title, &tw, &th);

		const double titleTopBottomMinimumMargin = this->FromDIP(10);

		wxRect2DDouble fullArea{ 0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()) };

		const double marginX = fullArea.GetSize().GetWidth() / 15.0;
		const double marginTop = std::max(fullArea.GetSize().GetHeight() / 16.0, titleTopBottomMinimumMargin * 2.0 + 20.0);
		const double marginBottom = fullArea.GetSize().GetHeight() / 10.0;
		double labelsToChartAreaMargin = this->FromDIP(10);

		wxRect2DDouble chartArea = fullArea;
		chartArea.Inset(marginX, marginTop, marginX, marginBottom);

		gc->DrawText(this->title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

		wxAffineMatrix2D normalizedToChartArea{};
		normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
		normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

		double lowValue = -3.0;
		double highValue = 3.0;

		int segmentCount = std::get<0>(CalSegment(lowValue, highValue));
		double rangeLow = std::get<1>(CalSegment(lowValue, highValue));
		double rangeHigh = std::get<2>(CalSegment(lowValue, highValue));

		double yValueSpan = rangeHigh - rangeLow;
		lowValue = rangeLow;
		highValue = rangeHigh;

		double yLinesCount = segmentCount + 1;
		double xValueSpan = 1.0;

		wxAffineMatrix2D normalizedToValue{};
		normalizedToValue.Translate(minX, highValue);
		normalizedToValue.Scale(1, -1);
		normalizedToValue.Scale(xValueSpan, yValueSpan);

		gc->SetPen(wxPen(wxColor(128, 128, 128)));
		gc->SetFont(*wxNORMAL_FONT, wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);

		for (int i = 0; i < yLinesCount; i++)
		{
			double normalizedLineY = static_cast<double>(i) / (yLinesCount - 1);

			auto lineStartPoint = normalizedToChartArea.TransformPoint({ 0, normalizedLineY });
			auto lineEndPoint = normalizedToChartArea.TransformPoint({ 1, normalizedLineY });

			wxPoint2DDouble linePoints[] = { lineStartPoint, lineEndPoint };
			gc->StrokeLines(2, linePoints);

			double valueAtLineY = normalizedToValue.TransformPoint({ 0, normalizedLineY }).m_y;

			auto text = wxString::Format("%.2f", valueAtLineY);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() - labelsToChartAreaMargin);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, chartArea.GetLeft() - labelsToChartAreaMargin - tw, lineStartPoint.m_y - th / 2.0);
		}

		wxPoint2DDouble leftHLinePoints[] = {
			normalizedToChartArea.TransformPoint({0, 0}),
			normalizedToChartArea.TransformPoint({0, 1}) };

		wxPoint2DDouble rightHLinePoints[] = {
			normalizedToChartArea.TransformPoint({1, 0}),
			normalizedToChartArea.TransformPoint({1, 1}) };

		gc->StrokeLines(2, leftHLinePoints);
		gc->StrokeLines(2, rightHLinePoints);

		wxPoint2DDouble* testSignal = new wxPoint2DDouble[sample_freq];
		wxPoint2DDouble* filteredSignal = new wxPoint2DDouble[sample_freq];

		wxAffineMatrix2D valueToNormalized = normalizedToValue;
		valueToNormalized.Invert();
		wxAffineMatrix2D valueToChartArea = normalizedToChartArea;
		valueToChartArea.Concat(valueToNormalized);

		double dt, y;
		std::vector<double> t;
		std::vector<double> yy;
		std::vector<double> y_filtered(sample_freq);

		wxCriticalSectionLocker lock(valuesCs);

        for (int i = 0; i < sample_freq; i++)
        {
            dt = static_cast<double>(i) / sample_freq;
			t.push_back(dt);

			y = sin(2 * M_PI * 5.0 * dt) + 0.7 * sin(2 * M_PI * 30.0 * dt) + 0.6 * sin(2 * M_PI * 50.0 * dt) + 0.8 * sin(2 * M_PI * 80.0 * dt);
			yy.push_back(y);

			testSignal[i] = valueToChartArea.TransformPoint({ dt, y });
        }

		for (int j = 0; j < sample_freq; j++)
		{
			if (j < 3)
			{
				y_filtered[j] = 0.0;
			}
			else
			{
				y_filtered[j] = a[1] * y_filtered[j - 1] + a[2] * y_filtered[j - 2] + b[0] * yy[j] + b[1] * yy[j - 1] + b[2] * yy[j - 2];
			}
		
			filteredSignal[j] = valueToChartArea.TransformPoint({ t[j], y_filtered[j] });
		}

		gc->SetPen(wxPen(*wxBLUE, 1));
		gc->StrokeLines(sample_freq, testSignal);
		gc->SetPen(wxPen(*wxRED, 1));
		gc->StrokeLines(sample_freq, filteredSignal);

		delete[] testSignal;
		delete[] filteredSignal;
		delete gc;
	}
}

std::tuple<int, double, double> FilterCalc::CalSegment(double origLow, double origHigh)
{
	constexpr double rangeMults[] = { 0.2, 0.25, 0.5, 1.0, 2.0, 2.5, 5.0 };
	constexpr int maxSegments = 6;

	double magnitude = std::floor(std::log10(origHigh - origLow));

	for (auto r : rangeMults)
	{
		double stepSize = r * std::pow(10.0, magnitude);
		double low = std::floor(origLow / stepSize) * stepSize;
		double high = std::ceil(origHigh / stepSize) * stepSize;

		int segments = round((high - low) / stepSize);

		if (segments <= maxSegments)
		{
			return std::make_tuple(segments, low, high);
		}
	}

	return std::make_tuple(10, origLow, origHigh);
}
=======
#include "filter_calc.h"

//! [get values of test signal]
std::tuple<std::vector<double>, std::vector<double>> FilterCalc::CalcTestSignal(int sample)
{
    std::vector<double> tt, yy;

    double dt = 1.0 / sample;
    double y;

    for (double t = 0.0; t <= 1.0; t = t + dt)
    {
        y = sin(2 * M_PI * 5.0 * t) + 0.6 * sin(2 * M_PI * 50.0 * t) + 0.8 * sin(2 * M_PI * 80.0 * t);

        tt.push_back(t);
        yy.push_back(y);
    }

    return make_tuple(tt, yy);
}

void FilterCalc::GetTestSignal(int sample)
{
    std::vector<double> tt = std::get<0>(CalcTestSignal(sample));
    std::vector<double> yy = std::get<1>(CalcTestSignal(sample));

    this->SetData(tt, yy);
}
//! [get values of test signal]

//! [get values of filtered signal]
std::tuple<std::vector<double>, std::vector<double>> FilterCalc::CalcFilteredSignal
(
    std::tuple<std::vector<double>, std::vector<double>> test_signal, std::vector<double> a, std::vector<double> b
)
{
    std::vector<double> tt = std::get<0>(test_signal);
    std::vector<double> yy = std::get<1>(test_signal);

    std::vector<double> yy_filtered(yy.size(), 0.0);

    for (int i = 3; i < yy.size(); i++)
    {
        yy_filtered[i] = a[1] * yy_filtered[i - 1] + a[2] * yy_filtered[i - 2]
            + b[0] * yy[i] + b[1] * yy[i - 1] + b[2] * yy[i - 2];
    }

    return make_tuple(tt, yy_filtered);
}

void FilterCalc::GetFilteredSignal(std::tuple<std::vector<double>, std::vector<double>> test_signal, std::vector<double> a, std::vector<double> b)
{
    std::vector<double> tt = std::get<0>(test_signal);
    std::vector<double> yy = std::get<1>(test_signal);

    std::vector<double> yy_filtered(yy.size(), 0.0);

    yy_filtered = std::get<1>(CalcFilteredSignal(test_signal, a, b));

    this->SetData(tt, yy_filtered);
}
//! [get values of filtered signal]

//! [calculate DFT]
void FilterCalc::CalcDFT(std::vector<double> time, std::vector<double> signal)
{
    int N = signal.size();
    int K = N;

    std::complex<double> intSum;

    std::vector<std::complex<double>> output;
    output.reserve(K);

    for (int k = 0; k < K; k++)
    {
        intSum = std::complex<double>(0.0, 0.0);

        for (int n = 0; n < N; n++)
        {
            double realPart = cos((2 * M_PI / N) * k * n);
            double imagPart = sin((2 * M_PI / N) * k * n);

            std::complex<double> w(realPart, -imagPart);

            intSum += signal[n] * w;
        }

        output.push_back(intSum);
    }

    int j = time.size();

    std::vector<double> xx;
    std::vector<double> yy;

    for (int n = 0; n < j; n++)
    {
        xx.push_back(n);
    }

    for (auto& ii : output)
    {
        ii = ii / static_cast<double>(j);
        yy.push_back(std::abs(ii));
    }

    this->SetData(xx, yy);
}
//! [calculate DFT]
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
