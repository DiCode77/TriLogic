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

constexpr const char *SETTINGS_FILE                 = "Settings.conf";
constexpr const char *CONFIG_INFO_VERSION           = "INFO/app_version";
constexpr const char *CONFIG_GAME_WINDOW_SIZE_GRID  = "GAME_WINDOW/size_grid";

class Config{
    wxFileConfig    _config;
    wxFileName      _file;
    bool            isStatus = false;
public:
    
    Config();
    void InitConfig(wxString, wxString);
    void SetParameter(wxString, long);
    void SetParameter(wxString, wxString);
    wxString GetStringParameter(wxString);
    long GetLongParameter(wxString);
    wxFileConfig *GetParameter();
    bool CheckKey(wxString);
    bool GetIsStatus();
    
private:
    const wxString GetFullPathForDir();
    void CreateConfFiles(wxString, wxString, wxString);
    bool CheckIsDir(wxString);
    bool CheckIsFile(wxString);
};

#endif /* SaveConfig_hpp */
