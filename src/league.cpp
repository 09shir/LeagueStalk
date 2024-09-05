#include "../include/league.h"

/**
 * returns 2d vector, each vector represents a list of user matches
 * an empty vector that can be detected with empty() for unfound user
*/
std::vector<std::vector<UserDetails>> getPlayerInfo() {

    SaveScreenshot();

    // --------------- extract username from screenshot ---------------
    std::vector<std::string> extractedUsernames = extractUsernames();

    for (std::string username : extractedUsernames){
        std::cout << username << std::endl;
    }

    // std::vector<std::vector<UserDetails>> listOfUsersDetails;

    std::vector<std::future<std::pair<std::string, std::vector<UserDetails>>>> futures;

    for (auto& username : extractedUsernames) {
        futures.push_back(std::async(std::launch::async, [&username]() -> std::pair<std::string, std::vector<UserDetails>> {
            const std::string id = getUserIdAPI(username.c_str());
            if (id == "failed to get id") {
                return {username, std::vector<UserDetails>{{"Failed to find player", "", "", -1, -1}}};
            }

            std::vector<UserDetails> userDetails = getUserDetailsAPI(id);
            if (userDetails.empty()) {
                userDetails.push_back({"Player unranked", "", "", -1, -1});
            }

            return {id, userDetails};
        }));
    }

    std::vector<std::vector<UserDetails>> listOfUsersDetails;

    for (auto& future : futures) {
        auto result = future.get();
        listOfUsersDetails.push_back(result.second);

        std::cout << "------------------------------" << std::endl;
        std::cout << "Username: " << result.first << std::endl;
        for (UserDetails& detail : result.second) {
            std::cout << "Queue Type: " << detail.queueType << std::endl;
            std::cout << "Rank: " << detail.rank << std::endl;
            std::cout << "Tier: " << detail.tier << std::endl;
            std::cout << "Wins: " << detail.wins << std::endl;
            std::cout << "Losses: " << detail.losses << std::endl << std::endl;
        }
    }

    return listOfUsersDetails;

}

