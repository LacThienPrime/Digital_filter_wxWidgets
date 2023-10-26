#pragma once

#include "my_frame.h"

class App : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(App);

