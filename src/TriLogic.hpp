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

#include "GridDynamic.hpp"

class TriLogic : public wxFrame{
    GridDynamic *grid;
    std::vector<std::vector<wxBitmapButton*>> vec_grid;
    std::vector<wxBitmapButton*> vecbit_on;
    wxSize grid_cell_size;
    short whoseMove = 0;
    
public:
    TriLogic(const wxString title, const wxPoint point, const wxSize size);
private:
    wxPanel *panel = nullptr;
    wxFrame *frameMFd = nullptr;
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
    void UpdateMatchSizeWindow();
    void DestroyFrame(wxCloseEvent&);
    void ReturnToMainWindow(wxCommandEvent&);
    void ReturnGameToStart(wxCommandEvent&);
    void ExitAllWindow(wxCommandEvent&);
    
    void SelectedBlock(wxCommandEvent&);
    
    void SetGridCellSize(wxSize);
    const wxSize &GetGridCellSize() const;
};

#endif /* TriLogic_hpp */
