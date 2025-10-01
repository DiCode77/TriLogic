//
//  TriLogic.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef TriLogic_hpp
#define TriLogic_hpp

#include <iostream>
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/bitmap.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/aboutdlg.h>

#include "GridDynamic.hpp"
#include "ColourPropertyGrid.hpp"

constexpr const char *PROG_VERSION = "0.0.2";

enum ID_START_BUTTON{
    NONE,
    ID_1VS1,
    ID_1VSBOT,
    ID_1VSAI
};

typedef struct SET_SETTINGS{
    wxColour main_window_colors = {0, 0, 0};
    wxColour game_window_colors = {0, 0, 0};
    wxColour game_grid_colors   = {0, 0, 0};
    int      mesh_seze_grid     = 3;
} SET_SETTINGS;

class TriLogic : public wxFrame{
    GridDynamic *grid;
    std::vector<std::vector<wxBitmapButton*>> vec_grid;
    std::vector<wxBitmapButton*> vecbit_on;
    wxSize grid_cell_size;
    short whoseMove = 0;
    ID_START_BUTTON ID_SELECT_BT_SG;
    SET_SETTINGS p_settings;
    
public:
    TriLogic(const wxString title, const wxPoint point, const wxSize size);
private:
    wxPanel *panel = nullptr;
    wxFrame *frameMFd = nullptr;
    wxFrame *frame_settings = nullptr;
private:
    wxButton *button_1vs1;
    wxButton *button_usVsus;
    wxButton *button_usVsai;
    wxBitmapButton *bbitmap_settings;
    wxBitmap *bitmap_iconSettings;
    
private:
    wxString GetFullDirPath(const char*, const char*, const char*);
    
private:
    void ShowMatchField(wxCommandEvent&);
    void InitBitmapButtonGrid(std::vector<std::vector<wxBitmapButton*>> *grid, int, int);
    void UpdateMatchSizeWindow();
    void DestroyFrame(wxCloseEvent&);
    void ReturnToMainWindow(wxCommandEvent&);
    void ReturnGameToStart(wxCommandEvent&);
    void ExitAllWindow(wxCommandEvent&);
    
    void GameModeStart(wxCommandEvent&);
    void SelectedBlock(wxCommandEvent&);
    
    void SetGridCellSize(wxSize);
    const wxSize &GetGridCellSize() const;
    void SetSelectIdButton(int);
    ID_START_BUTTON GetSelectIdButton();
    SET_SETTINGS &GetPSettings();
    
    void SetSettingsForGames(wxCommandEvent&);
    void SetSettingsProperty(wxPropertyGridEvent&);
    
    void DestroyFrameSettings(wxCloseEvent&);
    
    void ShowAbout(wxCommandEvent&);
};

#endif /* TriLogic_hpp */
