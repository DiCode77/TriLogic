//
//  SaveConfig.cpp
//  TriLogic
//
//  Created by DiCode77.
//

#include "SaveConfig.hpp"
#include "TriLogic.hpp"

Config::Config() : _config(wxEmptyString, wxEmptyString, wxString(GetFullPathForDir()).append("/").append(SETTINGS_FILE)){};

void Config::InitConfig(wxString appName, wxString appVendor){
    CreateConfFiles(GetFullPathForDir(), appName, appVendor);
}

const wxString Config::GetFullPathForDir(){
    return wxStandardPaths::Get().GetUserDataDir();
}

void Config::CreateConfFiles(wxString path, wxString appName, wxString appVendor){
    if (!CheckIsDir(path)){
        this->_file.Mkdir(path, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        this->isStatus = CheckIsDir(path) ? true : false;
    }
    
    if (this->isStatus){
        this->_config.SetAppName(appName);
        this->_config.SetVendorName(appVendor);
        
        this->_config.Write(wxString("APP_VERSION"), PROG_VERSION);
        this->_config.Flush();
        
        this->isStatus = CheckIsFile(wxString(path).append("/").append(SETTINGS_FILE)) ? true : false;
    }
}

bool Config::CheckIsDir(wxString path){
    return std::filesystem::exists(path.wc_str());
}

bool Config::CheckIsFile(wxString path){
    return std::filesystem::exists(path.wc_str()) && std::filesystem::is_regular_file(path.wc_str());
}
