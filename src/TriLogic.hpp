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

#include "GridDynamic.hpp"
#include "ColourPropertyGrid.hpp"

enum ID_START_BUTTON{
    NONE,
    ID_1VS1,
    ID_1VSBOT,
    ID_1VSAI
};

class TriLogic : public wxFrame{
    GridDynamic *grid;
    std::vector<std::vector<wxBitmapButton*>> vec_grid;
    std::vector<wxBitmapButton*> vecbit_on;
    wxSize grid_cell_size;
    short whoseMove = 0;
    ID_START_BUTTON ID_SELECT_BT_SG;
    
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
    wxString GetFoolDirPatch(const char*, const char*, const char*);
    
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
    
    void SetSettingsForGames(wxCommandEvent&);
    void SetSettingsProperty(wxPropertyGridEvent&);
    
    void DestroyFrameSettings(wxCloseEvent&);
    
};

#endif /* TriLogic_hpp */
