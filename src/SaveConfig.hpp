//
//  SaveConfig.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef SaveConfig_hpp
#define SaveConfig_hpp

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <filesystem>

constexpr const char *SETTINGS_FILE = "Settings.conf";

class Config{
    wxFileConfig    _config;
    wxFileName      _file;
    bool            isStatus = true;
public:
    
    Config();
    void InitConfig(wxString, wxString);
    
private:
    const wxString GetFullPathForDir();
    void CreateConfFiles(wxString, wxString, wxString);
    bool CheckIsDir(wxString);
    bool CheckIsFile(wxString);
};

#endif /* SaveConfig_hpp */
