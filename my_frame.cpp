#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START
};

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));

    m_radioBtn_lowpass->SetValue(true);

    testSignal = new FilterCalc(filterPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)));
    filterSizer->Add(testSignal, 1, wxEXPAND, 1);
    testSignal->title = "Filter";

    dftSignal = new FilterCalc(dftPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)));
    dftSizer->Add(dftSignal, 1, wxEXPAND, 1);
    dftSignal->title = "DFT";

    this->Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{     
    testSignal->Refresh();
    dftSignal->Refresh();

    sample_freq = atoi((m_textCtrl_sample->GetValue()).c_str());
    pass_freq = atoi((m_textCtrl_pass_freq->GetValue()).c_str());
    stop_freq = atoi((m_textCtrl_stop_freq->GetValue()).c_str());

    SelectResponse();

    testSignal->UpdateValues(sample_freq, a, b);
    dftSignal->UpdateValues(sample_freq, a, b);

    testSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintSignal, testSignal);
    dftSignal->Bind(wxEVT_PAINT, &FilterCalc::OnPaintSignal, dftSignal);
}

void MyFrame::OnClose(wxCloseEvent& e)
{
    this->Destroy();
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
