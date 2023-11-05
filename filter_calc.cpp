#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <thread>
#include <vector>
#include <complex>
#include <mutex>

#include "filter_calc.h"

wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_CANCELLED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_UPDATED, wxThreadEvent);

FilterCalc::FilterCalc(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_SORTINGTHREAD_UPDATED, &FilterCalc::OnThreadUpdate, this);
	this->Bind(wxEVT_SORTINGTHREAD_COMPLETED, &FilterCalc::OnThreadCompletion, this);
	this->Bind(wxEVT_SORTINGTHREAD_CANCELLED, &FilterCalc::OnThreadCancel, this);

	if (!this->processing)
	{
		this->processing = true;

		if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR || GetThread()->Run() != wxTHREAD_NO_ERROR)
		{
			this->processing = false;
		}
		else
		{

		}

		this->Layout();
	}
}

wxThread::ExitCode FilterCalc::Entry()
{
	wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTINGTHREAD_UPDATED);
	wxQueueEvent(this, e);

	if (wxThread::This()->TestDestroy())
	{
		wxThreadEvent* ev = new wxThreadEvent(wxEVT_SORTINGTHREAD_CANCELLED);
		wxQueueEvent(this, ev);
		return nullptr;
	}

	wxThreadEvent* eve = new wxThreadEvent(wxEVT_SORTINGTHREAD_COMPLETED);
	this->Bind(wxEVT_PAINT, &FilterCalc::OnPaintSignal, this);
	wxQueueEvent(this, eve);

	return nullptr;
}

void FilterCalc::OnThreadUpdate(wxThreadEvent& e)
{

}

void FilterCalc::OnThreadCompletion(wxThreadEvent& e)
{
	this->Layout();

	GetThread()->Wait();

	this->processing = false;
}

void FilterCalc::OnThreadCancel(wxThreadEvent& e)
{
	this->Layout();

	this->processing = false;
}

void FilterCalc::OnPaintSignal(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2.0,
			wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

		gc->SetFont(titleFont, *wxBLACK);

		double tw, th;
		gc->GetTextExtent(this->title, &tw, &th);

		const double titleTopBottomMinimumMargin = this->FromDIP(10);

		wxRect2DDouble fullArea{ 0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()) };

		const double marginX = fullArea.GetSize().GetWidth() / 15.0;
		const double marginTop = std::max(fullArea.GetSize().GetHeight() / 15.0, titleTopBottomMinimumMargin * 2.0 + 20.0);
		const double marginBottom = fullArea.GetSize().GetHeight() / 15.0;
		double labelsToChartAreaMargin = this->FromDIP(10);

		chartArea = fullArea;
		chartArea.Inset(marginX, marginTop, marginX + 10.0, marginBottom + 30.0);

		gc->DrawText(this->title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

		wxAffineMatrix2D normalizedToChartArea{};
		normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
		normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

		gc->SetPen(wxPen(wxColor(128, 128, 128)));
		gc->SetFont(*wxNORMAL_FONT, *wxBLACK);

		double lowValue = -3.0;
		double highValue = 3.0;

		int segmentCount = std::get<0>(CalSegment(lowValue, highValue));
		double rangeLow = std::get<1>(CalSegment(lowValue, highValue));
		double rangeHigh = std::get<2>(CalSegment(lowValue, highValue));

		double yValueSpan = rangeHigh - rangeLow;
		lowValue = rangeLow;
		highValue = rangeHigh;

		double yLinesCount = segmentCount + 1.0;
		double xValueSpan = 1.0;

		wxAffineMatrix2D normalizedToValue{};
		normalizedToValue.Translate(0.0, highValue);
		normalizedToValue.Scale(1, -1);
		normalizedToValue.Scale(xValueSpan, yValueSpan);

		wxAffineMatrix2D valueToNormalized = normalizedToValue;
		valueToNormalized.Invert();

		wxAffineMatrix2D valueToChartArea = normalizedToChartArea;
		valueToChartArea.Concat(valueToNormalized);
		
		for (int i = 0; i < 11; i++)
		{
			if (i == 0 || i == 10) {
				barColor = wxColour(0, 0, 0);
			} 
			else {
				barColor = wxColour(51, 204, 102);
			}
			gc->SetPen(barColor);
			
			double normalizedLineX = static_cast<double>(i) / 10;
			auto startPoint = normalizedToChartArea.TransformPoint({ normalizedLineX, 0 });
			auto endPoint = normalizedToChartArea.TransformPoint({ normalizedLineX, 1 });

			wxPoint2DDouble bottomHLinePoints[] = { startPoint, endPoint };

			gc->StrokeLines(2, bottomHLinePoints);

			double valueAtLineX = normalizedToValue.TransformPoint({ normalizedLineX, 0 }).m_x;

			auto text = wxString::Format("%.2f", valueAtLineX);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetBottom() - marginBottom);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, startPoint.m_x - th / 2.0, chartArea.GetBottom() + 6.0);
		}

		for (int j = 0; j < yLinesCount; j++)
		{
			if (j == 0 || j == 6) {
				barColor = wxColour(0, 0, 0);
			}
			else {
				barColor = wxColour(51, 204, 102);
			}
			gc->SetPen(barColor);

			double normalizedLineY = static_cast<double>(j) / (yLinesCount - 1);
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

		wxPoint2DDouble* testSignal = new wxPoint2DDouble[sample];
		wxPoint2DDouble* filteredSignal = new wxPoint2DDouble[sample];

		double dt, y;
		std::vector<double> t;
		std::vector<double> yy;
		std::vector<double> y_filtered(sample);

		for (int i = 0; i < sample; i++)
		{
			dt = static_cast<double>(i) / sample;
			t.push_back(dt);

			y = sin(2 * M_PI * 5.0 * dt) + 0.6 * sin(2 * M_PI * 50.0 * dt) + 0.8 * sin(2 * M_PI * 80.0 * dt);
			yy.push_back(y);

			testSignal[i] = valueToChartArea.TransformPoint({ dt, y });
		}

		for (int j = 0; j < sample; j++)
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

		for (int j = 0; j < sample; j++)
		{
			filteredSignal[j] = valueToChartArea.TransformPoint({ t[j], y_filtered[j] });
		}

		gc->SetPen(wxPen(*wxBLUE, 1));
		gc->StrokeLines(sample, testSignal);
		gc->SetPen(wxPen(*wxRED, 1));
		gc->StrokeLines(sample, filteredSignal);

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

	return std::make_tuple(6, origLow, origHigh);
}

void FilterCalc::UpdateValues(int s, std::vector<double> &ac, std::vector<double> &bc)
{
	sample = s;
	a = ac;
	b = bc;
}
