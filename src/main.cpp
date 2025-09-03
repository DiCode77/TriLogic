//
//  main.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "main.hpp"

bool MyApp::OnInit() {
    TriLogic *prog = new TriLogic(wxT("TriLogic Game"), wxDefaultPosition, wxSize(400, 500));
    prog->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
