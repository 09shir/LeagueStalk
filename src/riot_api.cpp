#include <iostream>
#include <cstdlib>
#include <curl/curl.h>
#include "../include/riot_api.h"
#include <nlohmann/json.hpp>

const char* API_KEY = std::getenv("API_KEY");
const char* API_KEY_SUFFIX = "?api_key=";

// example: https://na1.api.riotgames.com/lol/summoner/v4/summoners/by-name/Old8Sma11Burger?api_key={API_KEY}
const char* API_GET_USER_ID = "https://na1.api.riotgames.com/lol/summoner/v4/summoners/by-name/";
const char* API_GET_RANKS = "https://na1.api.riotgames.com/lol/league/v4/entries/by-summoner/";

using json = nlohmann::json;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * if not found: return "failed to get id"
 */
std::string getUserIdAPI(const char* username) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    long httpStatusCode = 0; // holds the HTTP status code

    char* API_URL = new char[200];
    strcat(API_URL, API_GET_USER_ID);
    strcat(API_URL, username);
    strcat(API_URL, API_KEY_SUFFIX);
    strcat(API_URL, API_KEY);

    std::string id = "failed to get id";

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // perform the request
        // res will get the return code
        res = curl_easy_perform(curl);

        // check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {

            // get the HTTP status code
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);
            if(httpStatusCode == 200) {
                try {
                    auto json_data = json::parse(readBuffer); // Parse the JSON data
                    id = json_data["id"]; // Access the id field
                } catch(json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                }
            }
        }

        // cleanup
        curl_easy_cleanup(curl);
    }
    return id;
}

std::vector<UserDetails> getUserDetailsAPI(const std::string id) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    char* API_URL = new char[200];
    strcat(API_URL, API_GET_RANKS);
    strcat(API_URL, id.c_str());
    strcat(API_URL, API_KEY_SUFFIX);
    strcat(API_URL, API_KEY);

    std::vector<UserDetails> retUserDetails;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // perform request
        // res stores return code
        res = curl_easy_perform(curl);

        // check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                auto json_data = json::parse(readBuffer); // Parse the JSON data

                if (!json_data.empty()) {
                    for (int i = 0; i < json_data.size(); i++){
                        auto entry = json_data.at(i);
                        UserDetails userDetail = UserDetails(
                            entry.value("queueType", ""),
                            entry.value("tier", ""),
                            entry.value("rank", ""),
                            entry.value("wins", -1),
                            entry.value("losses", -1)
                        );
                        retUserDetails.push_back(userDetail);
                    }
                }

            } catch(json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        }

        // cleanup
        curl_easy_cleanup(curl);

    }

    return retUserDetails; 
}
