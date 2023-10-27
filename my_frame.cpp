#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START
};

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));

    CreateStatusBar();
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{
    delete testSignal;
    delete dft;

    GetInputValue();
    SelectResponse();

    testSignal = new FilterCalc(filterPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq, a, b);
    filterSizer->Add(testSignal, 1, wxEXPAND, 1);
    testSignal->title = "Filter";

    dft = new FilterCalc(dftPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq);
    dftSizer->Add(dft, 1, wxEXPAND, 1);
    dft->title = "DFT";
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
