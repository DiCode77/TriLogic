#include "RequestAPI.hpp"

void OllamaAPI::InitConnect(std::string host, long port, bool htt, std::string model){
    this->host  = host;
    this->port  = port;
    this->https = htt;
    this->model = model;
}

void OllamaAPI::SetDataGridForHttp(const std::vector<std::vector<wxString>> &vec, const bool is, const long size){
    this->json_prompt["board"]       = vec;
    this->json_prompt["next_player"] = (is) ? "X" : "O";
    this->json_prompt["size"]        = size;
    
    this->json_cpr["model"]   = this->model;
    this->json_cpr["prompt"]  = std::string(PROMPT).append(this->json_prompt.dump());
    this->json_cpr["stream"]  = false;
    
    this->host_url.clear();
    this->host_url.append((this->https) ? HTTPS : HTTP0);
    this->host_url.append(this->host).append(":");
    this->host_url.append(std::to_string(this->port));
    this->host_url.append(APIGN);
}

long OllamaAPI::SendMessage(cpr::Response &res){
    return (res = cpr::Post(cpr::Url{this->host_url}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{this->json_cpr.dump()})).status_code;
}
