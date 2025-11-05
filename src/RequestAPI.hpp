//
//  RequestAPI.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef RequestAPI_hpp
#define RequestAPI_hpp

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <string>
#include <wx/wx.h>

constexpr const char *PROMPT = R"(
We are playing tic-tac-toe.
I will send you the game status data,
and you will make your move.
You need to specify the coordinates of the field where you will make your move; the answer must be in Json format. 
Example answer 
{
   "cell_x": "1"
   "cell_y": "3"
}  
Here is the field: )";

constexpr const char *HTTPS = "https://";
constexpr const char *HTTP0 = "http://";
constexpr const char *APIGN = "/api/generate";

constexpr int OLLAMA_NOT_RESPONDING        = 0;
constexpr int OLLAMA_SUCCESS               = 200;
constexpr int OLLAMA_BAD_REQUEST           = 400;
constexpr int OLLAMA_NOT_FOUND             = 404;
constexpr int OLLAMA_TOO_MANY_REQUEST      = 429;
constexpr int OLLAMA_INTERNAL_SERVER_ERROR = 500;
constexpr int OLLAMA_BAD_GATEWAY           = 502;

class OllamaAPI{
    std::string host;
    long        port;
    bool        https;
    std::string model;
    std::string host_url;
    
    nlohmann::json json_prompt;
    nlohmann::json json_cpr;
public:
    OllamaAPI(){}
    void InitConnect(std::string, long, bool, std::string);
    void SetDataGridForHttp(const std::vector<std::vector<wxString>>&, const bool, const long);
    void SetParametersModel(std::string);
    long SendMessage(cpr::Response&);
};

#endif /* RequestAPI_hpp */
