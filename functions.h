#pragma once

#include <iostream>
#include "json.hpp"
#include "helper.h"
#include "requests.h"

using json = nlohmann::json;
using namespace std;

string get_cookies(const char *response);
json extract_json_from_response(const char *response);

string login_adm(int sockfd, string &admin); //

void add_user(int sockfd, char **cookies); //

void get_users(int sockfd, char **cookies); //

string login_user(int sockfd, char **cookies, string &user);

void logout_user(int sockfd, char **cookies, string &user, string &ses);

void delete_user(int sockfd, char **cookies);

void logout_admin(int sockfd, char **cookies, string &admin, string &ses);

void delete_user(int sockfd, char **cookies);

string get_access(int sockfd, char **cookies);

void get_movies(int sockfd, char **cookies, string jwt);

void get_movie(int sockfd, char **cookies, string jwt);

void add_movie(int sockfd, char **cookies, string jwt);

void delete_movie(int sockfd, char **cookies, string jwt);

void update_movie(int sockfd, char **cookies, string jwt);

void get_collections(int sockfd, char **cookies, string jwt);

void add_collection(int sockfd, char **cookies, string jwt);

void delete_collection(int sockfd, char **cookies, string jwt);

void get_collection(int sockfd, char **cookies, string jwt);

void add_movie_to_collection(int sockfd, char **cookies, string jwt);

void delete_movie_from_collection(int sockfd, char **cookies, string jwt);
