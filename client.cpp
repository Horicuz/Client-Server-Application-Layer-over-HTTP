#include "helper.h"
#include "requests.h"
#include "functions.h"

#include <unordered_map>
#include <unordered_set>
#include <utility> // for std::pair
#include <vector>

#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// unordered_map<string, unordered_set<string>> users;

int main()
{
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);

    int exit = 1;

    string session_cookie = "-1"; // este cineva logat sau nu
    string curr_admin = "-1";     // ce admin este logat
    string curr_user = "-1";      // ce user este logat
    string jwt = "-1";            // JWT token

    while (exit)
    {
        string command;
        cin >> command;

        if (command == "exit")
        {
            exit = 0;
            break;
        }
        else if (command == "login_admin") // LOGIN ADMIN 1
        {
            if (curr_admin != "-1" || curr_user != "-1")
            {
                cout << "ERROR: Already logged in!" << endl;
                continue;
            }
            else
            {
                session_cookie = login_adm(sockfd, curr_admin);
                if (session_cookie == "")
                {
                    continue;
                }
                continue;
            }
        }
        else if (command == "add_user") // ADD USER 2
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                add_user(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "delete_user") // DELETE USER 3
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                delete_user(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_users") // GET USERS 4
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                get_users(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "logout_admin") // LOGOUT ADMIN 5
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                logout_admin(sockfd, cookies, curr_admin, session_cookie);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "add_user") // ADD USER 6
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                get_users(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "login")
        {
            if (curr_admin != "-1" || curr_user != "-1")
            {
                cout << "ERROR: Already logged in!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                session_cookie = login_user(sockfd, cookies, curr_user);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "logout") // LOGOUT USER 7
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                logout_user(sockfd, cookies, curr_user, session_cookie);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "delete_user") // DELETE USER 8
        {
            if (curr_admin == "-1")
            {
                cout << "ERROR: You must be logged in as admin!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                delete_user(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_access")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                jwt = get_access(sockfd, cookies);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_movies")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                printf("JWT: %s\n", jwt.c_str());
                get_movies(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_movie")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                get_movie(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "add_movie")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                add_movie(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "delete_movie")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                delete_movie(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "update_movie")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                update_movie(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_collections")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                get_collections(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "add_collection")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                add_collection(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "delete_collection")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                delete_collection(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "get_collection")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                get_collection(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "add_movie_to_collection")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                add_movie_to_collection(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else if (command == "delete_movie_from_collection")
        {
            if (curr_user == "-1")
            {
                cout << "ERROR: You must be logged in as user!" << endl;
                continue;
            }
            else
            {
                char **cookies = (char **)malloc(1 * sizeof(char *));
                cookies[0] = (char *)malloc((session_cookie.length() + 1) * sizeof(char));
                strcpy(cookies[0], session_cookie.c_str());
                delete_movie_from_collection(sockfd, cookies, jwt);
                free(cookies[0]);
                free(cookies);
            }
        }
        else
        {
            cout << "ERROR: Invalid command!" << endl;
        }
    }

    close(sockfd);
    return 0;
}
