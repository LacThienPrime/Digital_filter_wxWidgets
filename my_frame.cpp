#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START,
    ID_BTN_FIR_START,
};

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));
    Connect(ID_BTN_FIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartFIRClicked));

    m_radioBtn_lowpass->SetValue(true);

    testSignal = new FilterCalc(filterPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), true);
    filterSizer->Add(testSignal, 1, wxEXPAND, 1);
    testSignal->title = "IIR Filter";

    dftSignal = new FilterCalc(dftPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), false);
    dftSizer->Add(dftSignal, 1, wxEXPAND, 1);
    dftSignal->title = "FIR Filter";

    fir = new FIRfilter();

    this->Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
    
    GetInputValue();
    SelectResponse();

    testSignal->UpdateIIRValues(sample_freq, a, b);
    testSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintIIR, testSignal);

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

    responses = fir->ComputeResponses(sample_freq, order, shift_sample, low_cutoff_freq, high_cutoff_freq);
    window = fir->ComputeWindow(order);
    window_responses = fir->ComputeWindowedResponses(sample_freq, responses, window, order);

    dftSignal->UpdateFIRValues(sample_freq, order, window_responses);
    dftSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintFIR, dftSignal);
}

MyFrame::~MyFrame()
{
    delete fir;
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{     
    testSignal->Refresh();

    GetInputValue();
    SelectResponse();

    testSignal->UpdateIIRValues(sample_freq, a, b);

    std::thread signalThread([this]()
        {
            testSignal->Refresh();

            std::thread guiThread([this]()
                {
                    testSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintIIR, testSignal);
                });

            guiThread.detach();
        });

    signalThread.detach();
}

void MyFrame::OnStartFIRClicked(wxCommandEvent& e)
{
    dftSignal->Refresh();

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
    
    responses = fir->ComputeResponses(sample_freq, order, shift_sample, low_cutoff_freq, high_cutoff_freq);
    window = fir->ComputeWindow(order);
    window_responses = fir->ComputeWindowedResponses(sample_freq, responses, window, order);

    dftSignal->UpdateFIRValues(sample_freq, order, window_responses);
    dftSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintFIR, dftSignal);
}

void MyFrame::OnClose(wxCloseEvent& e)
{
    this->Destroy();
}

void MyFrame::GetInputValue()
{
    sample_freq = atoi((m_textCtrl_sample->GetValue()).c_str());
    pass_freq = atoi((m_textCtrl_pass_freq->GetValue()).c_str());
    stop_freq = atoi((m_textCtrl_stop_freq->GetValue()).c_str());
    order = atoi((m_textCtrl_order->GetValue()).c_str());
    shift_sample = atoi((m_textCtrl_shift_sample->GetValue()).c_str());
    low_cutoff_freq = atoi((m_textCtrl_low_cutoff->GetValue()).c_str());
    high_cutoff_freq = atoi((m_textCtrl_high_cutoff->GetValue()).c_str());
}

void MyFrame::SelectResponse()
{
    if (m_radioBtn_lowpass->GetValue())
    {
        std::unique_ptr<CoefStrategy> strategy = std::make_unique<lowpass>();
        Context context(strategy.get());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    else if (m_radioBtn_highpass->GetValue())
    {
        std::unique_ptr<CoefStrategy> strategy = std::make_unique<highpass>();
        Context context(strategy.get());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    else if (m_radioBtn_bandpass->GetValue())
    {
        std::unique_ptr<CoefStrategy> strategy = std::make_unique<bandpass>();
        Context context(strategy.get());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    else if (m_radioBtn_bandstop->GetValue())
    {
        std::unique_ptr<CoefStrategy> strategy = std::make_unique<bandstop>();
        Context context(strategy.get());
        auto coefficients = context.coefCalc(sample_freq, stop_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }
}
