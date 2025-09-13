//
//  TriLogic.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "TriLogic.hpp"

TriLogic::TriLogic(const wxString title, const wxPoint point, const wxSize size) : wxFrame(nullptr, wxID_ANY, title, point, size, wxCLOSE_BOX | wxMINIMIZE_BOX), grid(nullptr){
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
                                                                                                                                                            wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
        this->frameMFd->Bind(wxEVT_CLOSE_WINDOW, &TriLogic::DestroyFrame, this);
        this->frameMFd->SetBackgroundStyle(wxBG_STYLE_PAINT);
        
        this->grid = new GridDynamic(this->frameMFd, 8);
        this->grid->SetFuncUpdate(std::bind(&TriLogic::UpdateMatchSizeWindow, this));
        this->grid->DrawingGridDef(this->frameMFd->GetClientSize());
        
        this->frameMFd->Centre();
        this->frameMFd->Show();
        this->frameMFd->SetMaxSize(this->frameMFd->GetSize());
        this->frameMFd->SetMinSize(this->frameMFd->GetSize() / 1.9);
        this->Hide();
    
        if (grid->GetDataMap().count(0))
            SetGridCellSize(wxSize(grid->GetDataMap().at(0).at(0).dx, grid->GetDataMap().at(0).at(0).dy));
        
        this->vecbit_on.resize(3);
        std::vector<void (TriLogic::*)(wxCommandEvent&)> vec_func{
            &TriLogic::ReturnToMainWindow,
            &TriLogic::ReturnGameToStart,
            &TriLogic::ExitAllWindow
        };
        
        const char *arr_icon[] = { "Return", "turn", "exit" };
        wxSize size_icon[] = {{30, 30}, {45, 45}, {40, 40}};
        
        for (int i = 0; i < vecbit_on.size(); i++){
            this->vecbit_on[i] = new wxBitmapButton(this->frameMFd, wxID_ANY, wxImage(GetFoolDirPatch("resources", arr_icon[i], "png"), wxBITMAP_TYPE_PNG).Scale(size_icon[i]),
                                                    wxDefaultPosition,
                                                    wxSize(40, 40),
                                                    wxBORDER_NONE);
            this->vecbit_on[i]->Bind(wxEVT_BUTTON, vec_func[i], this);
        }
        
        this->vec_grid.resize(grid->GetDataMap().size());
        for (int i = 0; i < this->vec_grid.size(); i++){
            this->vec_grid[i].resize(grid->GetDataMap().size());
        }

        for (int i = 0; i < this->vec_grid.size(); i++){
            int code = i;
            
            for (int j = 0; j < this->vec_grid[i].size(); j++){
                this->vec_grid[i][j] = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap, wxPoint(grid->GetDataMap().at(code).at(j).x, grid->GetDataMap().at(code).at(j).y), wxSize(grid->GetDataMap().at(code).at(j).dx, grid->GetDataMap().at(code).at(j).dy), wxBORDER_NONE);
                this->vec_grid[i][j]->Bind(wxEVT_BUTTON, &TriLogic::SelectedBlock, this);
            }
        }
    }
}

void TriLogic::UpdateMatchSizeWindow(){
    if (!this->vec_grid.empty()){
        for (int i = 0; i < this->vec_grid.size(); i++){
            for (int j = 0; j < this->vec_grid[i].size(); j++){
                this->vec_grid[i][j]->SetPosition(wxPoint(grid->GetDataMap().at(i).at(j).x, grid->GetDataMap().at(i).at(j).y));
                this->vec_grid[i][j]->SetSize(wxSize(grid->GetDataMap().at(i).at(j).dx, grid->GetDataMap().at(i).at(j).dy));
            }
        }
    }
    
    if (!this->vecbit_on.empty()){
        for (int i = 0; i < this->vecbit_on.size(); i++){
            this->vecbit_on[i]->SetPosition(wxPoint(50 *i, this->frameMFd->GetSize().y -70));
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
        this->vec_grid.clear();
        this->vecbit_on.clear();
        this->Show();
    }
}

void TriLogic::ReturnGameToStart(wxCommandEvent&){
    if (!this->vec_grid.empty()){
        for (int i = 0; i <  this->vec_grid.size(); i++){
            for (int j = 0; j < this->vec_grid[i].size(); j++){
                this->vec_grid[i][j]->SetBitmap(wxNullBitmap);
            }
        }
    }
    this->whoseMove = 0;
}


void TriLogic::ExitAllWindow(wxCommandEvent&){
    this->Close();
}


void TriLogic::SelectedBlock(wxCommandEvent &ev){
    wxBitmapButton *button = (wxBitmapButton*)ev.GetEventObject();
    wxString ch[2] = {"cross", "zero"};
    
    if (!button->GetBitmap().IsOk()){
        wxImage img(GetFoolDirPatch("resources", ch[this->whoseMove], "png"), wxBITMAP_TYPE_PNG);
        img = img.Scale(GetGridCellSize());
        button->SetBitmap(img);
        this->whoseMove = !this->whoseMove;
    }
}

void TriLogic::SetGridCellSize(wxSize size){
    this->grid_cell_size = size;
}

const wxSize &TriLogic::GetGridCellSize() const{
    return this->grid_cell_size;
}
