//
//  ColourPropertyGrid.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "ColourPropertyGrid.hpp"


MyColourProperty::MyColourProperty(wxFrame *frame, const wxString &label, const wxString &name, const wxColour &color) : wxColourProperty(label, name, color){
    this->_frame = frame;
}

wxColour MyColourProperty::GetColors(){
    return this->_colors;
}

 bool MyColourProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event){
    bool askColour = false;
    
    if (propgrid->IsMainButtonEvent(event)){
        askColour = true;
    }
    else if (event.GetEventType() == wxEVT_COMBOBOX){
        wxOwnerDrawnComboBox* cb = static_cast<wxOwnerDrawnComboBox*>(propgrid->GetEditorControl());
        
        if (cb){
            int index = cb->GetSelection();
            
            if ( index == GetCustomColourIndex()){
                askColour = true;
            }
        }
    }
    
    if (askColour && !propgrid->WasValueChangedInEvent()){
        wxVariant variant;
        
        if (MyQueryColourFromUser(variant)){
            return false;
        }
    }
    return false;
}


bool MyColourProperty::MyQueryColourFromUser(wxVariant& variant){
    bool res = false;
    
    wxColourPropertyValue val = GetVal();
    
    wxColourData data;
    data.SetChooseFull(true);
    data.SetColour(*wxWHITE);
    
    wxColourDialog dialog(this->_frame, &data);
    if (dialog.ShowModal() == wxID_OK) {
        wxColourData retData = dialog.GetColourData();
        wxColour col = retData.GetColour();
        
        if (col.IsOk() && this->_frame != nullptr) {
            this->_frame->Refresh();
            this->_frame->Update();
        }
        
        val.m_colour = retData.GetColour();
        variant = DoTranslateVal(val);
        
        SetValueInEvent(variant);
        SetRGBColors(col);
        res = true;
    }
    return res;
}

void MyColourProperty::SetRGBColors(const wxColour colors)  {
    this->_colors = colors;
}
