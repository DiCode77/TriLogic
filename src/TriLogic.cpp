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
    
    this->button_1vs1         = new wxButton(this->panel, ID_START_BUTTON::ID_1VS1, wxT("1 Vs 1"), wxDefaultPosition, wxDefaultSize);
    this->button_usVsus       = new wxButton(this->panel, ID_START_BUTTON::ID_1VSBOT, wxT("1 Vs Bot"), wxDefaultPosition, wxDefaultSize);
    this->button_usVsai       = new wxButton(this->panel, ID_START_BUTTON::ID_1VSAI, wxT("1 Vs Ai"), wxDefaultPosition, wxDefaultSize);
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
    this->bbitmap_settings->Bind(wxEVT_BUTTON, &TriLogic::SetSettingsForGames, this);
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
        
        this->grid = new GridDynamic(this->frameMFd, GetPSettings().mesh_seze_grid);
        this->grid->SetFuncUpdate(std::bind(&TriLogic::UpdateMatchSizeWindow, this));
        this->grid->DrawingGridDef(this->frameMFd->GetClientSize());
        this->grid->SetColorIsGrid(GetPSettings().game_grid_colors);
        
        this->frameMFd->Centre();
        this->frameMFd->Show();
        this->frameMFd->SetMaxSize(this->frameMFd->GetSize());
        this->frameMFd->SetMinSize(this->frameMFd->GetSize() / 1.9);
        this->Hide();
    
        if (this->grid->GetDataMap().count(0))
            SetGridCellSize(wxSize(this->grid->GetDataMap().at(0).at(0).dx, this->grid->GetDataMap().at(0).at(0).dy));
        
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
        
        InitBitmapButtonGrid(&this->vec_grid, 0, static_cast<int>(this->grid->GetDataMap().size()));
        SetSelectIdButton(ev.GetId());
    }
}

void TriLogic::InitBitmapButtonGrid(std::vector<std::vector<wxBitmapButton*>> *m_grid, int start, int end){
    if (m_grid != nullptr && start != end){
        wxCommandEvent ev_cell(wxEVT_BUTTON, NULL);
        ReturnGameToStart(ev_cell);
        
        if (m_grid->size() < end){
            m_grid->resize(end);
            
            for (int i = 0; i < m_grid->size(); i++){
                (*m_grid)[i].resize(end);
            }
        }
        else{
            if (start >= end){
                for (size_t i = 0; i < m_grid->size() && start; i++){
                    for (int j = 0; j < (*m_grid)[i].size(); j++){
                        if (j < end){
                            continue;
                        }
                        else{
                            if (i >= end){
                                break;
                            }
                            else{
                                (*m_grid)[i][j]->Destroy();
                                (*m_grid)[i][j] = nullptr;
                            }
                        }
                    }
                    
                    if (i >= end){
                        for (int l = 0; l < (*m_grid)[i].size(); l++){
                            (*m_grid)[i][l]->Destroy();
                            (*m_grid)[i][l] = nullptr;
                        }
                    }
                    
                    if (i < end){
                        (*m_grid)[i].resize(end);
                    }
                }
                
                m_grid->resize(end);
                return;
            }
        }
        
        for (int i = 0; i < end; i++){
            for (int j = 0; j < end; j++){
                if ((*m_grid)[i][j] != nullptr){
                    continue;
                }
                
                wxBitmapButton *btn = new wxBitmapButton(this->frameMFd, wxID_ANY, wxNullBitmap,
                                                         wxPoint(this->grid->GetDataMap().at(i).at(j).x, this->grid->GetDataMap().at(i).at(j).y),
                                                         wxSize(this->grid->GetDataMap().at(i).at(j).dx, this->grid->GetDataMap().at(i).at(j).dy), wxBORDER_NONE);
                btn->Bind(wxEVT_BUTTON, &TriLogic::GameModeStart, this);
                (*m_grid)[i][j] = btn;
            }
        }
    }
}

// Here we update the location of the elements and their size.
void TriLogic::UpdateMatchSizeWindow(){
    if (!this->vec_grid.empty()){
        InitBitmapButtonGrid(&this->vec_grid, static_cast<int>(this->vec_grid.size()), this->grid->GetGridLines());
        SetGridCellSize(wxSize(this->grid->GetDataMap().at(0).at(0).dx, this->grid->GetDataMap().at(0).at(0).dy));
        
        for (int i = 0; i < this->vec_grid.size(); i++){
            for (int j = 0; j < this->vec_grid[i].size(); j++){
                this->vec_grid[i][j]->SetPosition(wxPoint(this->grid->GetDataMap().at(i).at(j).x, this->grid->GetDataMap().at(i).at(j).y));
                this->vec_grid[i][j]->SetSize(wxSize(this->grid->GetDataMap().at(i).at(j).dx, this->grid->GetDataMap().at(i).at(j).dy));
            }
        }
    }
    
    // updating the location of navigation icons.
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
        this->whoseMove = 0;
        this->grid->Destroy();
        this->grid = nullptr;
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

void TriLogic::GameModeStart(wxCommandEvent &ev){
    switch (GetSelectIdButton()){
        case ID_START_BUTTON::ID_1VS1:
            SelectedBlock(ev);
            break;
        case ID_START_BUTTON::ID_1VSBOT:
            break;
        case ID_START_BUTTON::ID_1VSAI:
            break;
        default:
            break;
    }
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

void TriLogic::SetSelectIdButton(int id){
    switch (id) {
        case ID_START_BUTTON::ID_1VS1:
            this->ID_SELECT_BT_SG = ID_START_BUTTON::ID_1VS1;
            break;
        case ID_START_BUTTON::ID_1VSBOT:
            this->ID_SELECT_BT_SG = ID_START_BUTTON::ID_1VSBOT;
            break;
        case ID_START_BUTTON::ID_1VSAI:
            this->ID_SELECT_BT_SG = ID_START_BUTTON::ID_1VSAI;
            break;
        default:
            this->ID_SELECT_BT_SG = ID_START_BUTTON::NONE;
            break;
    }
}

ID_START_BUTTON TriLogic::GetSelectIdButton(){
    return this->ID_SELECT_BT_SG;
}

SET_SETTINGS &TriLogic::GetPSettings(){
    return this->p_settings;
}


void TriLogic::SetSettingsForGames(wxCommandEvent&){
    if (this->frame_settings == nullptr){
        this->frame_settings = new wxFrame(this, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxSize(600, 450));
        this->frame_settings->Bind(wxEVT_CLOSE_WINDOW, &TriLogic::DestroyFrameSettings, this);
        
        wxPropertyGrid* pg = new wxPropertyGrid(frame_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
        pg->Bind(wxEVT_PG_CHANGED, &TriLogic::SetSettingsProperty, this);
        
        wxPGProperty *title_name = pg->Append(new wxPropertyCategory(this->GetTitle()));
        
        wxPGProperty *main_w = title_name->AppendChild(new wxPropertyCategory("Main Window"));
        main_w->AppendChild(new MyColourProperty(this, "Color", "Main_color_window"));
        
        wxArrayString arr_lab;
        wxArrayInt arr_val;
        
        for (int i = 3; i <= 10; i++){
            arr_lab.Add(std::to_string(i) + "x" + std::to_string(i));
            arr_val.Add(i);
        }
        
        wxPGProperty *game_w = title_name->AppendChild(new wxPropertyCategory("Game Window"));
        game_w->AppendChild(new wxEnumProperty("Mesh size", "Mesh_size_grid", arr_lab, arr_val, 1));
        game_w->AppendChild(new MyColourProperty(this, "Color Grid", "Game_grid_color"));
        game_w->AppendChild(new MyColourProperty(this, "Color Window", "Game_color_window"));
        
        pg->CollapseAll();
        title_name->SetExpanded(true);
        main_w->SetExpanded(false);
        game_w->SetExpanded(false);
        
        
        frame_settings->Show();
    }
}

void TriLogic::DestroyFrameSettings(wxCloseEvent&){
    if (this->frame_settings != nullptr){
        this->frame_settings->Destroy();
        this->frame_settings = nullptr;
    }
}


void TriLogic::SetSettingsProperty(wxPropertyGridEvent &event){
    if (event.GetPropertyName() == "Main_color_window"){
        MyColourProperty *mcp = dynamic_cast<MyColourProperty*>(event.GetProperty());
        if (mcp){
            this->SetBackgroundColour(mcp->GetColors());
            this->Update();
            this->Refresh();
        }
    }
    else if (event.GetPropertyName() == "Game_grid_color"){
        MyColourProperty *mcp = dynamic_cast<MyColourProperty*>(event.GetProperty());
        if (mcp){
            GetPSettings().game_grid_colors = mcp->GetColors();
            
            if (this->grid != nullptr){
                this->grid->SetColorIsGrid(GetPSettings().game_grid_colors);
                this->grid->Update();
                this->grid->Refresh();
            }
        }
    }
    else if (event.GetPropertyName() == "Game_color_window"){
        MyColourProperty *mcp = dynamic_cast<MyColourProperty*>(event.GetProperty());
        if (mcp && this->frameMFd != nullptr){
            GetPSettings().game_window_colors = mcp->GetColors();
            
            if (this->grid != nullptr){
                this->grid->SetBackgroundColour(GetPSettings().game_window_colors);
                this->grid->Update();
                this->grid->Refresh();
            }
        }
    }
    else if (event.GetPropertyName() == "Mesh_size_grid"){
        long val = event.GetValue().GetLong();
        if (val >= 3){
            GetPSettings().mesh_seze_grid = static_cast<int>(val);
            
            if (this->grid){
                this->grid->SetGridLines(GetPSettings().mesh_seze_grid);
                this->grid->Update();
                this->grid->Refresh();
            }
        }
    }
}
