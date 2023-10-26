#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START,
    ID_BTN_FIR_START
};

<<<<<<< HEAD
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_CANCELLED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_UPDATED, wxThreadEvent);

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
=======
MyFrame::MyFrame()
    : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));
    Connect(ID_BTN_FIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartFIRClicked));

<<<<<<< HEAD
    this->Bind(wxEVT_SORTINGTHREAD_UPDATED, &MyFrame::OnThreadUpdate, this);
    this->Bind(wxEVT_SORTINGTHREAD_COMPLETED, &MyFrame::OnThreadCompletion, this);
    this->Bind(wxEVT_SORTINGTHREAD_CANCELLED, &MyFrame::OnThreadCancel, this);

    CreateStatusBar();
    this->SetStatusText("(i) Thread Info");
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{  
    delete testSignal;
    delete dft;    
    
    GetInputValue();
    SelectResponse();

    testSignal = new FilterCalc(filterPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq, a, b, dataCs);
    filterSizer->Add(testSignal, 1, wxEXPAND, 1);
    testSignal->title = "Filter";

    dft = new FilterCalc(dftPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq, a, b, dataCs);
    dftSizer->Add(dft, 1, wxEXPAND, 1);
    dft->title = "DFT"; 

    if (!this->processing)
    {
        this->processing = true;

        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR || GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            this->SetStatusText("Could not create thread");

            this->processing = false;
        }
        else
        {
            this->SetStatusText("Calculating");
        }

        this->Layout();
    }
}

wxThread::ExitCode MyFrame::Entry()
{
    int n = sample_freq;

    for (int i = 0; i < n - 1; i++)
    {
        wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTINGTHREAD_UPDATED);
        e->SetPayload<double>(static_cast<double>(i) / static_cast<double>(n - 2));
        wxQueueEvent(this, e);

        if (wxThread::This()->TestDestroy())
        {
            wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTINGTHREAD_CANCELLED);
            e->SetString("Processing aborted.");
            wxQueueEvent(this, e);
            return nullptr;
        }

        wxCriticalSectionLocker lock(dataCs);
        this->SetStatusText("Processing");
    }

    wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTINGTHREAD_COMPLETED);
    e->SetString("Completed");
    wxQueueEvent(this, e);

    return nullptr;
}

void MyFrame::OnClose(wxCloseEvent& e)
{
    if (GetThread() && GetThread()->IsRunning())
    {
        GetThread()->Delete();
    }

    this->Destroy();
}

void MyFrame::OnThreadUpdate(wxThreadEvent& e)
{
    double progressFraction = e.GetPayload<double>();
}

void MyFrame::OnThreadCompletion(wxThreadEvent& e)
{
    this->SetStatusText(e.GetString());
    this->Layout();

    GetThread()->Wait();

    this->processing = false;
}

void MyFrame::OnThreadCancel(wxThreadEvent& e)
{
    this->SetStatusText(e.GetString());
    this->Layout();

    this->processing = false;
=======
    filterPlot = new mpWindow(this, wxID_ANY, wxPoint(300, 20), wxSize(550, 250), wxBORDER_DEFAULT);
    DFTPlot = new mpWindow(this, wxID_ANY, wxPoint(300, 300), wxSize(550, 250), wxBORDER_DEFAULT);

    timeAxis = new mpScaleX(wxT("Time (s)"), mpALIGN_BORDER_BOTTOM, false, mpX_NORMAL);
    amplitudeAxis = new mpScaleY(wxT("Amplitude (dB)"), mpALIGN_BORDER_LEFT, false);
    freqAxis = new mpScaleX(wxT("Freqency (Hz)"), mpALIGN_BORDER_BOTTOM, false, mpX_NORMAL);
    magnitudeAxis = new mpScaleY(wxT("Amplitude (dB)"), mpALIGN_BORDER_LEFT, false);

    testSignal = new FilterCalc();
    filteredSignal = new FilterCalc();

    signalDFT = new FilterCalc();
    filterDFT = new FilterCalc();

    fir = new FIRfilter();
    coef = new FIRfilter();

    CreateStatusBar();
}

void MyFrame::OnStartFIRClicked(wxCommandEvent& e)
{    
    filterPlot->DelAllLayers(0, 1);
    DFTPlot->DelAllLayers(0, 1);

    GetInputValue();

    fir->WIN_TYPE = (FIRfilter::WindowType)(m_choice_fir_type->GetSelection());

    if (m_radioBtn_lowpass->GetValue())
    {
        fir->FILT_TYPE = FIRfilter::FilterType::LowPass;
    }

    if (m_radioBtn_highpass->GetValue())
    {
        fir->FILT_TYPE = FIRfilter::FilterType::HighPass;
    }

    if (m_radioBtn_bandpass->GetValue())
    {
        fir->FILT_TYPE = FIRfilter::FilterType::BandPass;
    }

    if (m_radioBtn_bandstop->GetValue())
    {
        fir->FILT_TYPE = FIRfilter::FilterType::BandStop;
    }

    response = fir->ComputeResponses(sample_freq, filter_order, shift_sample, low_cutoff_freq, high_cutoff_freq);
    window = fir->ComputeWindow(filter_order);
    fir_coef = fir->ComputeWindowedResponses(sample_freq, response, window, filter_order);
    fir_time = std::get<0>(fir->CalcFIRFilter(fir_coef, yy));
    fir_signal = std::get<1>(fir->CalcFIRFilter(fir_coef, yy));

    wxPen drawingBluePen(*wxBLUE, 1, wxPENSTYLE_LONG_DASH);
    wxPen drawingRedPen(*wxRED, 1, wxPENSTYLE_LONG_DASH);

    testSignal->GetTestSignal(sample_freq);
    testSignal->SetPen(drawingBluePen);
    testSignal->SetContinuity(true);

    yy = std::get<1>(testSignal->CalcTestSignal(sample_freq));
    fir->GetFIRFilter(fir_coef, yy);
    fir->SetPen(drawingRedPen);
    fir->SetContinuity(true);

    filterPlot->AddLayer(timeAxis);
    filterPlot->AddLayer(amplitudeAxis);

    filterPlot->AddLayer(testSignal);
    filterPlot->AddLayer(fir);

    filterPlot->Fit(-0.1, 1.1, -3, 3);
    
    coef->GetCoefFIR(sample_freq, response, window, filter_order);
    coef->SetPen(drawingBluePen);
    coef->SetContinuity(true);

    DFTPlot->AddLayer(freqAxis);
    DFTPlot->AddLayer(magnitudeAxis);

    DFTPlot->AddLayer(coef);

    DFTPlot->Fit(-0.01, 0.17, -0.1, 0.25);

    int myint = fir_time.size();
    wxString mystring = wxString::Format(wxT("%i"), myint);
    //float myfloat = fir_coef[9];
    //wxString mystring = wxString::Format(wxT("%f"), myfloat);
    this->SetStatusText(mystring);
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{    
    filterPlot->DelAllLayers(0, 1);
    DFTPlot->DelAllLayers(0, 1);

    GetInputValue();
    SelectResponse();

    tt = std::get<0>(testSignal->CalcTestSignal(sample_freq));
    yy = std::get<1>(testSignal->CalcTestSignal(sample_freq));

    wxPen drawingBluePen(*wxBLUE, 1, wxPENSTYLE_LONG_DASH);
    wxPen drawingRedPen(*wxRED, 1, wxPENSTYLE_LONG_DASH);

    testSignal->GetTestSignal(sample_freq);
    testSignal->SetPen(drawingBluePen);
    testSignal->SetContinuity(true);

    filteredSignal->GetFilteredSignal(std::make_tuple(tt, yy), a, b);
    filteredSignal->SetPen(drawingRedPen);
    filteredSignal->SetContinuity(true);

    filterPlot->AddLayer(timeAxis);
    filterPlot->AddLayer(amplitudeAxis);

    filterPlot->AddLayer(testSignal);
    filterPlot->AddLayer(filteredSignal);

    filterPlot->Fit(-0.1, 1.1, -3, 3);

    //![calculate and plot DFT]
    yy_filtered = std::get<1>(filteredSignal->CalcFilteredSignal(std::make_tuple(tt, yy), a, b));

    signalDFT->CalcDFT(tt, yy);    
    signalDFT->SetPen(drawingBluePen);
    signalDFT->SetContinuity(true);

    filterDFT->CalcDFT(tt, yy_filtered);
    filterDFT->SetPen(drawingRedPen);
    filterDFT->SetContinuity(true);

    DFTPlot->AddLayer(freqAxis);
    DFTPlot->AddLayer(magnitudeAxis);

    DFTPlot->AddLayer(signalDFT);
    DFTPlot->AddLayer(filterDFT);

    DFTPlot->Fit(-10, 100, -0.05, 0.6);
    //! [calculate and plot DFT]
>>>>>>> 0a03b58ebc67b86781da17136f3e8130bbdc9136
}

void MyFrame::GetInputValue()
{
    sample_freq = atoi((m_textCtrl_sample->GetValue()).c_str());
    pass_freq = atoi((m_textCtrl_pass_freq->GetValue()).c_str());
    stop_freq = atoi((m_textCtrl_stop_freq->GetValue()).c_str());
    filter_order = atoi((m_textCtrl_order->GetValue()).c_str());
    shift_sample = atoi((m_textCtrl_shift_sample->GetValue()).c_str());
    low_cutoff_freq = atoi((m_textCtrl_low_cutoff->GetValue()).c_str());
    high_cutoff_freq = atoi((m_textCtrl_high_cutoff->GetValue()).c_str());
}

void MyFrame::SelectResponse()
{
    if (m_radioBtn_lowpass->GetValue())
    {
        Context context(new lowpass());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if (m_radioBtn_highpass->GetValue())
    {
        Context context(new highpass());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if (m_radioBtn_bandpass->GetValue())
    {
        Context context(new bandpass());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if (m_radioBtn_bandstop->GetValue())
    {
        Context context(new bandstop());
        auto coefficients = context.coefCalc(sample_freq, stop_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }
}
