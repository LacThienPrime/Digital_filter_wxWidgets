#include "app.h"

#include <wx/image.h>

IMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MyFrame* mainFrame = new MyFrame();
    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(wxT("1894488.bmp"), wxBITMAP_TYPE_BMP));
    mainFrame->SetIcon(icon);
    mainFrame->Show();

    return true;
}