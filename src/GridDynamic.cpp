//
//  GridDynamic.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "GridDynamic.hpp"

GridDynamic::GridDynamic(wxWindow *th_w, int line = 0) : wxPanel(th_w), line_grid(line), myFunc(nullptr), color_grid(wxColour(0, 0, 0)){
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &GridDynamic::DrawingGrid, this);
}

// I set the size of the grid n * n separately
void GridDynamic::gridLines(int lines){
    this->line_grid = lines;
}

// The main method of constructing a grid and calculating the coordinates of each cell
void GridDynamic::DrawingGridDef(wxSize mySize){
    // Automatic double buffering
    wxAutoBufferedPaintDC dcGrid(this);
    dcGrid.Clear();
    
    wxGraphicsContext *gc = wxGraphicsContext::Create(dcGrid);
    
    if (!gc){
        return;
    }
    else{
        const wxSize size   =  /*((mySize == wxSize(0, 0)) ? this->GetClientSize() : mySize);*/ this->GetParent()->GetClientSize(); // Return the overall dimensions of the window
        const double width  = size.GetWidth();
        const double height = size.GetHeight();
        
        const double indents = std::min(width, height) * 0.1; // here I specify the size of the entire grid
        
        // here are the margins, left, right, top, and bottom
        const double left   = indents;
        const double right  = width - indents;
        const double top    = indents;
        const double bootom = height - indents;
        
        const double gridWidth  = right - left;
        const double gridHeight = bootom - top;
        
        const double thick = std::max(2.0, std::min(gridWidth, gridHeight) * 0.02); // mesh thickness
        
        // This is where the grid itself is constructed.
        wxPen pen(wxColor(GetColorGrid()), thick);
        pen.SetCap(wxCAP_ROUND);
        pen.SetJoin(wxJOIN_ROUND);
        gc->SetPen(pen);
        
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        
        const double dx = gridWidth / this->line_grid;
        const double dy = gridHeight / this->line_grid;
        
        for (int i = 0; i < this->line_grid; i++){
            double x = left + (i * dx), y = top + (i * dy);
            
            if (i > 0){
                gc->StrokeLine(x, top, x, bootom);
                gc->StrokeLine(left, y, right, y);
            }
        }

        // thick is the thickness of the grid line in pixels, I divide it by two to reduce the indentation in the cell
        const double thick_drop = thick / 2;
        
        // Here I store the coordinates of each cell in the grid.
        this->un_map.clear();
        for (int i = 0; i < this->line_grid; i++){
            this->un_map.insert({i, {}});
            
            for (int j = 0; j < this->line_grid; j++){
                double x = left + j * dx, y = top + i * dy;
                
                // I check whether such a key has already been created, and use this key to store the new cell coordinates.
                if (this->un_map.count(i))
                    this->un_map[i].push_back({x + thick_drop, y + thick_drop, dx - thick, dy - thick});
            }
        }
        
        if (this->myFunc != nullptr)
            this->myFunc();
        
        delete gc;
    }
}

// I return the map, with all the coordinates of the cells
const std::unordered_map<int, std::vector<elem_posit>> &GridDynamic::GetDataMap() const{
    return this->un_map;
}

// The event, call, was placed in a separate method so that it could be called separately without creating the event itself.
void GridDynamic::DrawingGrid(wxPaintEvent&){
    DrawingGridDef();
}

// Return the specified grid color
wxColor GridDynamic::GetColorGrid(){
    return this->color_grid;
}

// I am transferring my function for calling here
void GridDynamic::SetFuncUpdate(std::function<void()> func){
    this->myFunc = func;
}

// I set the color for the brush manually
void GridDynamic::SetColorIsGrid(wxColor color){
    this->color_grid = color;
}
