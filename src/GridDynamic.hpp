//
//  GridDynamic.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef GridDynamic_hpp
#define GridDynamic_hpp

#include <wx/wx.h>
#include <wx/dcbuffer.h>

class GridDynamic : public wxPanel{
    int line_grid;
    
public:
    GridDynamic(wxWindow *th_w, int line = 0) : wxPanel(th_w), line_grid(line){
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &GridDynamic::DrawingGrid, this);
        
        std::cout << th_w->GetSize().x << std::endl;
        std::cout << th_w->GetSize().y << std::endl;
    }
    
    void gridLines(int lines){
        this->line_grid = lines;
    }
    
    void DrawingGrid(wxPaintEvent&){
        wxAutoBufferedPaintDC dcGrid(this);
        dcGrid.Clear();
        
        wxGraphicsContext *gc = wxGraphicsContext::Create(dcGrid);
        
        if (!gc){
            return;
        }
        else{
            const wxSize size   =  this->GetClientSize();
            const double width  = size.GetWidth();
            const double height = size.GetHeight();
            
            const double indents = std::min(width, height) * 0.1;
            
            const double left   = indents;
            const double right  = width - indents;
            const double top    = indents;
            const double bootom = height - indents;
            
            const double gridWidth  = right - left;
            const double gridHeight = bootom - top;
            
            const double thick = std::max(2.0, std::min(gridWidth, gridHeight) * 0.02);
            
            wxPen pen(wxColor(0, 0, 0), thick);
            pen.SetCap(wxCAP_ROUND);
            pen.SetJoin(wxJOIN_ROUND);
            gc->SetPen(pen);
            
            gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
            
            const double dx = gridWidth / this->line_grid;
            const double dy = gridHeight / this->line_grid;
            
            for (int i = 1; i < this->line_grid; i++){
                double x = left + i * dx, y = top + i * dy;
                
                gc->StrokeLine(x, top, x, bootom);
                gc->StrokeLine(left, y, right, y);
            }
            delete gc;
        }
    }
};

#endif /* GridDynamic_hpp */

