#include "my_frame.h"

enum IDs
{
    ID_BTN_IIR_START,
    ID_BTN_FIR_START
};

wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_CANCELLED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_SORTINGTHREAD_UPDATED, wxThreadEvent);

MyFrame::MyFrame() : MyFrameUI(nullptr, wxID_ANY, "Digital Filter C++", wxDefaultPosition, wxSize(900, 650))
{
    Connect(ID_BTN_IIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartIIRClicked));
    Connect(ID_BTN_FIR_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnStartFIRClicked));

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

    dft = new FilterCalc(dftPanel, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(600, 300)), sample_freq, dataCs);
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

    //for (int i = 0; i < n; i++)
    //{
        wxThreadEvent* e = new wxThreadEvent(wxEVT_SORTINGTHREAD_UPDATED);
        //e->SetPayload<double>(static_cast<double>(i) / static_cast<double>(n - 2));
        wxQueueEvent(this, e);

        if (wxThread::This()->TestDestroy())
        {
            wxThreadEvent* ev = new wxThreadEvent(wxEVT_SORTINGTHREAD_CANCELLED);
            ev->SetString("Processing aborted.");
            wxQueueEvent(this, ev);
            return nullptr;
        }

        wxCriticalSectionLocker lock(dataCs);
        this->SetStatusText("Processing");
    //}

    wxThreadEvent* eve = new wxThreadEvent(wxEVT_SORTINGTHREAD_COMPLETED);
    eve->SetString("Completed");
    wxQueueEvent(this, eve);

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
    //double progressFraction = e.GetPayload<double>();
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
