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
    this->bitmap_iconSettings = new wxBitmap(GetFullDirPath("resources", "settings", "png"), wxBITMAP_TYPE_PNG);
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
    
    this->button_usVsus->Disable();
    this->button_usVsai->Disable();
    
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* menu_ab = new wxMenu();
    menu_ab->Append(wxID_ABOUT, "About TriLogic");
    menu_ab->Append(ID_START_BUTTON::ID_1VS1,   wxString("Play: ").append(this->button_1vs1->GetLabel()));
    menu_ab->Append(ID_START_BUTTON::ID_1VSBOT, wxString("Play: ").append(this->button_usVsus->GetLabel()));
    menu_ab->Append(ID_START_BUTTON::ID_1VSAI,  wxString("Play: ").append(this->button_usVsai->GetLabel()));
    menuBar->Append(menu_ab, "&Mode");
    SetMenuBar(menuBar);
    
    this->Bind(wxEVT_MENU, &TriLogic::ShowAbout,      this, wxID_ABOUT);
    this->Bind(wxEVT_MENU, &TriLogic::ShowMatchField, this, ID_START_BUTTON::ID_1VS1);
    this->Bind(wxEVT_MENU, &TriLogic::ShowMatchField, this, ID_START_BUTTON::ID_1VSBOT);
    this->Bind(wxEVT_MENU, &TriLogic::ShowMatchField, this, ID_START_BUTTON::ID_1VSAI);
    
    menu_ab->Enable(ID_START_BUTTON::ID_1VSBOT, false);
    menu_ab->Enable(ID_START_BUTTON::ID_1VSAI,  false);
}

void TriLogic::ShowAbout(wxCommandEvent&){
    wxAboutDialogInfo info;
    info.SetName("TriLogic Game");
    info.SetVersion(PROG_VERSION);
    info.SetCopyright("(C) 2019 - 2025 DiCode77.");
    wxAboutBox(info);
}

wxString TriLogic::GetFullDirPath(const char *folder, const char *name, const char *ext){
    wxFileName imagePath(wxStandardPaths::Get().GetExecutablePath());
    imagePath.AppendDir("..");
    imagePath.AppendDir(folder);
    imagePath.SetFullName(name);
    imagePath.SetExt(ext);
    return imagePath.GetFullPath();
}

void TriLogic::ShowMatchField(wxCommandEvent &ev){
    if (this->frameMFd == nullptr){
        
        wxString label_txt;
        if (ev.GetEventType() == wxEVT_BUTTON)    label_txt = ((wxButton*)(ev.GetEventObject()))->GetLabel();
        else if (ev.GetEventType() == wxEVT_MENU) label_txt = GetMenuBar()->FindItem(ev.GetId())->GetItemLabelText();
        
        this->frameMFd = new wxFrame(this->panel, wxID_ANY, wxString(" Start Games! ").append(label_txt), wxDefaultPosition, wxSize(700, 700), wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
        this->frameMFd->Bind(wxEVT_CLOSE_WINDOW, &TriLogic::DestroyFrame, this);
        this->frameMFd->SetBackgroundStyle(wxBG_STYLE_PAINT);
        
        this->grid = new GridDynamic(this->frameMFd, GetPSettings().mesh_seze_grid);
        this->grid->SetFuncUpdate(std::bind(&TriLogic::UpdateMatchSizeWindow, this));
        this->grid->DrawingGridDef(this->frameMFd->GetClientSize());
        this->grid->SetColorIsGrid(GetPSettings().game_grid_colors);
        this->grid->SetColorIsWind(GetPSettings().game_window_colors);
        
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
            this->vecbit_on[i] = new wxBitmapButton(this->frameMFd, wxID_ANY, wxImage(GetFullDirPath("resources", arr_icon[i], "png"), wxBITMAP_TYPE_PNG).Scale(size_icon[i]),
                                                    wxDefaultPosition,
                                                    wxSize(40, 40),
                                                    wxBORDER_NONE);
            this->vecbit_on[i]->Bind(wxEVT_BUTTON, vec_func[i], this);
        }
        
        InitBitmapButtonGrid(&this->vec_grid, 0, static_cast<int>(this->grid->GetDataMap().size()));
        SetSelectIdButton(ev.GetId());
    }
}

// Several actions take place here.
// First, if the buttons in the cells are initialized for the first time, only their declaration occurs.
// Second, if the buttons already existed but the grid size has changed, then the old buttons must be destroyed and new ones initialized at the old coordinates and new ones.
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
        this->DestroyBitmapButtons(&this->vec_grid);
        this->vecbit_on.clear();
        this->whoseMove = 1;
        this->grid->Destroy();
        this->grid = nullptr;
        this->st_pm_grid.matrix.clear();
        this->Show();
        this->SetGameStatus(false);
    }
}

// For cleaning all fields and fields
void TriLogic::ReturnGameToStart(wxCommandEvent&){
    if (!this->vec_grid.empty()){
        for (int i = 0; i <  this->vec_grid.size(); i++){
            for (int j = 0; j < this->vec_grid[i].size(); j++){
                this->vec_grid[i][j]->SetBitmap(wxNullBitmap);
                this->vec_grid[i][j]->SetBackgroundColour(wxNullColour);
                
                if (!this->vec_grid[i][j]->IsEnabled()){
                    this->vec_grid[i][j]->Enable(true);
                }
            }
        }
    }
    this->st_pm_grid.matrix.clear();
    this->whoseMove = 1;
    this->SetGameStatus(false);
}


void TriLogic::ExitAllWindow(wxCommandEvent&){
    this->Close();
}

void TriLogic::GameModeStart(wxCommandEvent &ev){
    if (!IsTheGameOver()){
        switch (GetSelectIdButton()){
            case ID_START_BUTTON::ID_1VS1:
                SelectedBlock(ev);
                IdentifyActiveCells(&this->vec_grid, static_cast<wxBitmapButton*>(ev.GetEventObject()), &this->st_pm_grid, !this->whoseMove);
                CheckGameStatus(&this->st_pm_grid, static_cast<wxBitmapButton*>(ev.GetEventObject()));
                break;
            case ID_START_BUTTON::ID_1VSBOT:
                break;
            case ID_START_BUTTON::ID_1VSAI:
                break;
            default:
                break;
        }
    }
}


void TriLogic::SelectedBlock(wxCommandEvent &ev){
    wxBitmapButton *button = (wxBitmapButton*)ev.GetEventObject();
    wxString ch[2] = {"zero", "cross"};
    
    if (!button->GetBitmap().IsOk()){
        wxImage img(GetFullDirPath("resources", ch[this->whoseMove], "png"), wxBITMAP_TYPE_PNG);
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
        game_w->AppendChild(new wxEnumProperty("Mesh size", "Mesh_size_grid", arr_lab, arr_val, 3));
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

// To completely destroy and free memory in std::vector from wxBitmapButton
void TriLogic::DestroyBitmapButtons(std::vector<std::vector<wxBitmapButton*>> *v_vec){
    if (v_vec != nullptr && !v_vec->empty()){
        std::ranges::for_each(v_vec->begin(), v_vec->end(), [](std::vector<wxBitmapButton*> &vec){
            std::ranges::for_each(vec.begin(), vec.end(), [](wxBitmapButton *&b){
                if (b != nullptr){
                    b->Destroy();
                    b = nullptr;
                }
            });
        });
        v_vec->clear();
        v_vec->shrink_to_fit();
    }
}

// To configure, set user variables.
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
        if (mcp){
            GetPSettings().game_window_colors = mcp->GetColors();
            
            if (this->frameMFd != nullptr && this->grid != nullptr){
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

// Required to wait for an event to occur, to prevent false presses.
void TriLogic::SetActiveStateForBButton(std::vector<std::vector<wxBitmapButton*>> *vec_grid, const bool isStatus, const std::vector<wxBitmapButton*> *excep){
    if (vec_grid != nullptr && !vec_grid->empty()){
        std::ranges::for_each(vec_grid->begin(), vec_grid->end(), [isStatus, &excep, this](std::vector<wxBitmapButton*> &vec){
            std::ranges::for_each(vec.begin(), vec.end(), [isStatus, &excep, this](wxBitmapButton *&isButton){
                if (isButton != nullptr && excep != nullptr && !excep->empty()){
                    if (!SearchForActiveBButtons(excep, isButton)){
                        isButton->Enable(isStatus);
                    }
                }
                else if (isButton != nullptr){
                    isButton->Enable(isStatus);
                }
            });
        });
    }
}

bool TriLogic::SearchForActiveBButtons(const std::vector<wxBitmapButton*> *vec, const wxBitmapButton *in_button){
    if (vec != nullptr && !vec->empty() && in_button != nullptr){
        auto it = std::ranges::find_if(vec->begin(), vec->end(), [&in_button](wxBitmapButton *b){
            if (b == in_button){
                return true;
            }
        });
        return *it == in_button;
    }
    return false;
}

// Initialize and build a matrix according to the selected cells in the grid
void TriLogic::IdentifyActiveCells(std::vector<std::vector<wxBitmapButton*>> *vec_grid, wxBitmapButton *b_but, DATA_POS_MATRIX *st, int id){
    if (vec_grid != nullptr && !vec_grid->empty()){
        if (st->matrix.empty()){
            st->matrix.resize(vec_grid->size());
            
            std::ranges::for_each(st->matrix.begin(), st->matrix.end(), [&vec_grid](std::vector<int> &m_vec){
                m_vec.resize(vec_grid->size());
                std::ranges::for_each(m_vec.begin(), m_vec.end(), [](int &val){
                    val = -1;
                });
            });
        }
        
        for (int i = 0; i < vec_grid->size(); i++){
            for (int j = 0; j < (*vec_grid)[i].size(); j++){
                if ((*vec_grid)[i][j] == b_but){
                    st->matrix[i][j] = id;
                    st->last_pos = std::make_pair(i, j);
                }
            }
        }
    }
}

// Here, the selected cells are checked to determine the winner.
void TriLogic::CheckGameStatus(const DATA_POS_MATRIX *st, wxBitmapButton *button){
    int sum = 0;
    int points = GetTheLengthOfTheSequence(static_cast<int>(st->matrix.size()));
    const auto m_vec = st->matrix;
    const auto m_pir = st->last_pos;
    const int mp_last = m_vec[m_pir.first][m_pir.second];
    std::vector<wxBitmapButton*> v_button;
    
    // We determine horizontal matches.
    for (int i = 0; i < m_vec[m_pir.first].size(); i++){ //
        if (m_vec[m_pir.first][i] != mp_last){
            sum = 0;
            v_button.clear();
        }
        else{
            sum++;
            v_button.push_back(vec_grid[m_pir.first][i]);
        }
        
        if (sum == points){
            EventsAfterTheVictory(&v_button);
            return;
        }
    }
    
    // We determine vertical matches.
    sum = 0;
    v_button.clear();
    for (int i = 0; i < m_vec.size(); i++){ //
        if (m_vec[i][m_pir.second] != mp_last){
            sum = 0;
            v_button.clear();
        }
        else{
            sum++;
            v_button.push_back(vec_grid[i][m_pir.second]);
        }
        
        if (sum == points){
            EventsAfterTheVictory(&v_button);
            return;
        }
    }
    
    // We determine the matches in the matrix from left to right.
    sum = 0;
    v_button.clear();
    auto l_toR = DiagonalPushFromLeftToRight(m_pir.first, m_pir.second);
    for (int x = l_toR.first, y = l_toR.second; x < m_vec.size() && y < m_vec.size(); x++, y++){
        if (m_vec[x][y] != mp_last){
            sum = 0;
            v_button.clear();
        }
        else{
            sum++;
            v_button.push_back(vec_grid[x][y]);
        }
        
        if (sum == points){
            EventsAfterTheVictory(&v_button);
            return;
        }
    }
    
    // We determine the matches in the matrix from right to left.
    sum = 0;
    v_button.clear();
    auto r_toL = DiagonalPushFromRightToLeft(m_pir.first, m_pir.second, static_cast<int>(m_vec.size()));
    for (int x = r_toL.first, y = r_toL.second; x < m_vec.size() && y < m_vec.size(); x++, y--){
        if (m_vec[x][y] != mp_last){
            sum = 0;
            v_button.clear();
        }
        else{
            sum++;
            v_button.push_back(vec_grid[x][y]);
        }
        
        if (sum == points){
            EventsAfterTheVictory(&v_button);
            return;
        }
    }
}

// We determine the starting position for steps from left to right.
std::pair<int, int>TriLogic::DiagonalPushFromLeftToRight(int x, int y){
    while (x != 0 && y != 0){
        x--;
        y--;
    }
    return std::make_pair(x, y);
}

// We determine the starting position for steps from right to left.
std::pair<int, int>TriLogic::DiagonalPushFromRightToLeft(int x, int y, int max){
    while (x != 0 && y != (max -1)){
        x--;
        y++;
    }
    return std::make_pair(x, y);
}

// Set the color for selected cells.
void TriLogic::SetCellColor(std::vector<wxBitmapButton*> *b){
    for (int i = 0;i < b->size(); i++){
        (*b)[i]->SetBackgroundColour(wxColour(200, 220, 255));
    }
}

// We obtain the number of sequences for different fields.
int TriLogic::GetTheLengthOfTheSequence(int val){
    switch (val) {
        case 3:
        case 4:
            return 3;
            break;
        case 5:
        case 6:
            return 4;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            return 5;
        default:
            return 3;
            break;
    }
    return 3;
}

bool TriLogic::IsTheGameOver(){
    return this->gameStatus;
}

void TriLogic::SetGameStatus(bool isStatus){
    this->gameStatus = isStatus;
}

void TriLogic::EventsAfterTheVictory(std::vector<wxBitmapButton*> *vec_b){
    if (vec_b != nullptr && !vec_b->empty()){
        SetCellColor(vec_b);
        SetActiveStateForBButton(&this->vec_grid, false, vec_b);
        vec_b->clear();
        SetGameStatus(true);
    }
    
    wxMessageDialog message(this->frameMFd, "You have won!\n\n Start a new game ?", "TriLogic", wxYES_NO | wxICON_QUESTION);
    int id = message.ShowModal();

    if (id == wxID_YES) {
        wxCommandEvent ev_cell(wxEVT_BUTTON, NULL);
        ReturnGameToStart(ev_cell);
    }
    else if (id == wxID_NO) {
    }
}
