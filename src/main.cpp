#include <iostream>
#include <wx/wx.h>

class MyApp : public wxApp{
public:
    bool OnInit() override{
        wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Window", wxDefaultPosition, wxSize(700, 800));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);