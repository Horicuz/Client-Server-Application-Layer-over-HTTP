#ifndef _REQUESTS_
#define _REQUESTS_

#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <cstdio>       /* for sprintf, printf */
#include <cstdlib>      /* for calloc, free */
#include <sys/socket.h> /* for socket, connect */

#include "json.hpp"

using json = nlohmann::json;
using namespace std;
// Computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(string host, string url, const char *query_params,
                          char **cookies, int cookies_count, int auth, string jwt);

// Specialized function for creating a POST request with JSON data
char *compute_json_post_request(string host, const string url, json &data,
                                char **cookies, int cookies_count, int auth, string jwt);

char *compute_delete(string host, const string url, const char *query_params, char **cookies, int cookies_count, int auth, string jwt);

char *compute_json_put_request(string host, string url, const char *query_params,
                               json &data, char **cookies, int cookies_count, int auth, string jwt);
#endif // _REQUESTS_
