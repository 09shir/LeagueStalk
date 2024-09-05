#pragma once
#include <iostream>
#include <string>

class UserDetails {
public:
    std::string queueType;
    std::string tier;
    std::string rank;
    int wins;
    int losses;
    
    UserDetails(const std::string& queueType = "", const std::string& tier = "", const std::string& rank = "", const int wins = -1, const int losses = -1)
    : queueType(queueType), tier(tier), rank(rank), wins(wins), losses(losses) {}
};

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
std::string getUserIdAPI(const char* username);
std::vector<UserDetails> getUserDetailsAPI(const std::string id);
