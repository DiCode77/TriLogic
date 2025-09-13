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
#include <functional>

typedef struct elem_posit{
    const double x;
    const double y;
    const double dx;
    const double dy;
} elem_posit;

class GridDynamic : public wxPanel{
    std::function<void()> myFunc;
    
    int line_grid;
    std::unordered_map<int, std::vector<elem_posit>> un_map;
public:
    
    GridDynamic(wxWindow*, int);
    
    void gridLines(int);
    void DrawingGridDef(wxSize mySize = wxSize(0, 0));
    const std::unordered_map<int, std::vector<elem_posit>> &GetDataMap() const;
    
private:
    void DrawingGrid(wxPaintEvent&);
    
public:
    void SetFuncUpdate(std::function<void()> func);
};

#endif /* GridDynamic_hpp */

