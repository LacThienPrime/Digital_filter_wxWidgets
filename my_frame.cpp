#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START
};

wxDEFINE_EVENT(wxEVT_SORTING_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTING_CANCELLED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTING_UPDATED, wxThreadEvent);

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));

    this->Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);

    this->Bind(wxEVT_SORTING_UPDATED, &MyFrame::OnThreadUpdate, this);
    this->Bind(wxEVT_SORTING_COMPLETED, &MyFrame::OnThreadCompletion, this);
    this->Bind(wxEVT_SORTING_CANCELLED, &MyFrame::OnThreadCancel, this);

    CreateStatusBar();
}

void MyFrame::OnStartIIRClicked(wxCommandEvent& e)
{
    delete testSignal;

    GetInputValue();
    SelectResponse();
6
    if (!this->processing)
    {
        this->processing = true;

        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR ||
            GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            this->SetStatusText("Could not create thread.");

            this->processing = false;
        }
        else
        {
            this->SetStatusText("Sorting ");
        }

        this->Layout();
    }
}

wxThread::ExitCode MyFrame::Entry()
{
    wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTING_UPDATED);
    wxQueueEvent(this, e);

    if (wxThread::This()->TestDestroy())
    {
        wxThreadEvent* ev = new wxThreadEvent(wxEVT_SORTING_CANCELLED);
        wxQueueEvent(this, ev);
        return nullptr;
    }

    wxThreadEvent* eve = new wxThreadEvent(wxEVT_SORTING_COMPLETED);

    testSignal = new FilterCalc(filterPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq, a, b);
    filterSizer->Add(testSignal, 1, wxEXPAND, 1);
    testSignal->title = "Filter";

    this->SetStatusText("Completed");

    wxQueueEvent(this, eve);

    return nullptr;
}

void MyFrame::OnClose(wxCloseEvent& e)
{
    this->Destroy();
}

void MyFrame::OnThreadUpdate(wxThreadEvent& e)
{

}

void MyFrame::OnThreadCompletion(wxThreadEvent& e)
{
    this->Layout();

    GetThread()->Wait();

    this->processing = false;
}

void MyFrame::OnThreadCancel(wxThreadEvent& e)
{
    this->Layout();

    this->processing = false;
}

void MyFrame::GetInputValue()
{
    sample_freq = atoi((m_textCtrl_sample->GetValue()).c_str());
    pass_freq = atoi((m_textCtrl_pass_freq->GetValue()).c_str());
    stop_freq = atoi((m_textCtrl_stop_freq->GetValue()).c_str());
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

    else if (m_radioBtn_highpass->GetValue())
    {
        Context context(new highpass());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    else if (m_radioBtn_bandpass->GetValue())
    {
        Context context(new bandpass());
        auto coefficients = context.coefCalc(sample_freq, pass_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    else if (m_radioBtn_bandstop->GetValue())
    {
        Context context(new bandstop());
        auto coefficients = context.coefCalc(sample_freq, stop_freq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }
}
