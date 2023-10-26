#include "app.h"

IMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MyFrame* mainFrame = new MyFrame();
    mainFrame->Show();

    return true;
}