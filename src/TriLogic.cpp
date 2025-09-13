//
//  TriLogic.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "TriLogic.hpp"

TriLogic::TriLogic(const wxString title, const wxPoint point, const wxSize size) : wxFrame(nullptr, wxID_ANY, title, point, size, wxCLOSE_BOX | wxMINIMIZE_BOX){
    this->Centre();
    this->panel = new wxPanel(this, wxID_ANY);
    wxImage::AddHandler(new wxPNGHandler);
    
    this->button_1vs1         = new wxButton(this->panel, wxID_ANY, wxT("1 Vs 1"), wxDefaultPosition, wxDefaultSize);
    this->button_usVsus       = new wxButton(this->panel, wxID_ANY, wxT("1 Vs Bot"), wxDefaultPosition, wxDefaultSize);
    this->button_usVsai       = new wxButton(this->panel, wxID_ANY, wxT("1 Vs Ai"), wxDefaultPosition, wxDefaultSize);
    this->bbitmap_settings    = new wxBitmapButton(this->panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(80, 80), wxBORDER_NONE);
    this->bitmap_iconSettings = new wxBitmap(GetFoolDirPatch("resources", "settings", "png"), wxBITMAP_TYPE_PNG);
    wxBitmapBase::Rescale(*this->bitmap_iconSettings, this->bbitmap_settings->GetSize());
    
    wxPoint bb_xy = {
        (this->GetSize().x - this->button_1vs1->GetSize().x) / 2,
        (this->GetSize().y - this->button_1vs1->GetSize().y) / 2
    };
    
    this->button_1vs1->SetPosition(wxPoint(bb_xy.x, bb_xy.y -50));
    this->button_usVsus->SetPosition(wxPoint(bb_xy.x, bb_xy.y));
    this->button_usVsai->SetPosition(wxPoint(bb_xy.x, bb_xy.y +50));
    this->bbitmap_settings->SetPosition(wxPoint(bb_xy.x, bb_xy.y +120));
    this->bbitmap_settings->SetBitmap(*this->bitmap_iconSettings);
    
    this->button_1vs1->Bind(wxEVT_BUTTON, &TriLogic::ShowMatchField, this);
    this->button_usVsus->Bind(wxEVT_BUTTON, &TriLogic::ShowMatchField, this);
    this->button_usVsai->Bind(wxEVT_BUTTON, &TriLogic::ShowMatchField, this);
}

wxString TriLogic::GetFoolDirPatch(const char *folder, const char *name, const char *ext){
    wxFileName imagePath(wxStandardPaths::Get().GetExecutablePath());
    imagePath.AppendDir("..");
    imagePath.AppendDir(folder);
    imagePath.SetFullName(name);
    imagePath.SetExt(ext);
    return imagePath.GetFullPath();
}

void TriLogic::ShowMatchField(wxCommandEvent &ev){
    if (this->frameMFd == nullptr){
        this->frameMFd = new wxFrame(this->panel, wxID_ANY, wxString(" Start Games! ").append(((wxButton*)(ev.GetEventObject()))->GetLabel()), wxDefaultPosition, wxSize(700, 700),
                                                                                                                                                                  wxCLOSE_BOX | wxMINIMIZE_BOX);
        this->frameMFd->Bind(wxEVT_CLOSE_WINDOW, &TriLogic::DestroyFrame, this);
        GridDynamic *grid = new GridDynamic(this->frameMFd, 8);
        grid->DrawingGridDef(this->frameMFd->GetClientSize());
        
        this->frameMFd->Centre();
        this->frameMFd->Show(true);
       // this->frameMFd->SetBackgroundColour(wxColour(0, 0, 0, 150));
        this->Hide();
        
        wxImage img_return(GetFoolDirPatch("resources", "Return2", "png"), wxBITMAP_TYPE_PNG);
        img_return = img_return.Scale(50, 50);
        
        wxBitmapButton *sbitmap_return = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap, wxDefaultPosition, img_return.GetSize(), wxBORDER_NONE);
        sbitmap_return->Bind(wxEVT_BUTTON, &TriLogic::ReturnToMainWindow, this);
        sbitmap_return->SetBitmap(img_return);
        sbitmap_return->SetPosition(wxPoint(15, 605));
        
        
        wxImage img_turn(GetFoolDirPatch("resources", "turn2", "png"), wxBITMAP_TYPE_PNG);
        img_turn = img_turn.Scale(70, 70);
        
        wxBitmapButton *sbitmap_turn = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap, wxDefaultPosition, img_turn.GetSize(), wxBORDER_NONE);
        sbitmap_turn->Bind(wxEVT_BUTTON, &TriLogic::ReturnGameToStart, this);
        sbitmap_turn->SetBitmap(img_turn);
        sbitmap_turn->SetPosition(wxPoint(((this->frameMFd->GetSize().x /2) - img_turn.GetSize().x /2), sbitmap_return->GetPosition().y -8));
        
    
        wxImage img_exit(GetFoolDirPatch("resources", "exit1", "png"), wxBITMAP_TYPE_PNG);
        img_exit = img_exit.Scale(60, 60);
        
        wxBitmapButton *sbitmap_exit = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap, wxDefaultPosition, img_exit.GetSize(), wxBORDER_NONE);
        sbitmap_exit->Bind(wxEVT_BUTTON, &TriLogic::ExitAllWindow, this);
        sbitmap_exit->SetBitmap(img_exit);
        sbitmap_exit->SetPosition(wxPoint(((this->frameMFd->GetSize().x) - img_exit.GetSize().x -10), sbitmap_turn->GetPosition().y +3));
        
        if (grid->GetDataMap().count(0))
            SetGridCellSize(wxSize(grid->GetDataMap().at(0).at(0).dx, grid->GetDataMap().at(0).at(0).dy));
        
        
        std::vector<std::vector<wxBitmapButton*>> vec(grid->GetDataMap().size());
        
        for (int i = 0; i < vec.size(); i++){
            vec[i].resize(grid->GetDataMap().size());
        }

        for (int i = 0; i < vec.size(); i++){
            int code = i;
            
            for (int j = 0; j < vec[i].size(); j++){
                vec[i][j] = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap, wxPoint(grid->GetDataMap().at(code).at(j).x, grid->GetDataMap().at(code).at(j).y), wxSize(grid->GetDataMap().at(code).at(j).dx, grid->GetDataMap().at(code).at(j).dy), wxBORDER_NONE);
                vec[i][j]->Bind(wxEVT_BUTTON, &TriLogic::SelectedBlock, this);
            }
        }
    }
}

void TriLogic::DestroyFrame(wxCloseEvent &close){
    this->Close();
}


void TriLogic::ReturnToMainWindow(wxCommandEvent&){
    if (this->frameMFd != nullptr){
        this->frameMFd->Destroy();
        this->frameMFd = nullptr;
        this->Show();
    }
}

void TriLogic::ReturnGameToStart(wxCommandEvent&){
}


void TriLogic::ExitAllWindow(wxCommandEvent&){
    this->Close();
}


void TriLogic::SelectedBlock(wxCommandEvent &ev){
    puts("SelectedBlock");
    static short revert = 0;
    
    wxBitmapButton *button = (wxBitmapButton*)ev.GetEventObject();
    wxString ch[2] = {"cross2", "zero2"};
    
    if (!button->GetBitmap().IsOk()){
        wxImage img(GetFoolDirPatch("resources", ch[revert], "png"), wxBITMAP_TYPE_PNG);
        img = img.Scale(GetGridCellSize());
        
        button->SetBitmap(img);
        
        revert = !revert;
    }
}

void TriLogic::SetGridCellSize(wxSize size){
    this->grid_cell_size = size;
}

const wxSize &TriLogic::GetGridCellSize() const{
    return this->grid_cell_size;
}
