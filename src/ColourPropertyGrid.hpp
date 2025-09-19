//
//  ColourPropertyGrid.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef ColourPropertyGrid_hpp
#define ColourPropertyGrid_hpp

#include <wx/frame.h>
#include <wx/colour.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>
#include <wx/odcombo.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include <wx/propgrid/advprops.h>

class MyColourProperty : public wxColourProperty{
    wxFrame *_frame;
    wxColour _colors;
public:
    MyColourProperty(wxFrame *frame = nullptr, const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, const wxColour &color = *wxWHITE);
    wxColour GetColors();
    
protected:
    virtual bool OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event) override;
    
private:
    bool MyQueryColourFromUser(wxVariant &variant);
    void SetRGBColors(const wxColour colors);
};

#endif /* ColourPropertyGrid_hpp */
