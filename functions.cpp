#include <iostream>
#include "json.hpp"
#include "helper.h"
#include "requests.h"
#include "functions.h"

#include <unordered_map>
#include <unordered_set>
#include <utility> // for std::pair
#include <vector>

using json = nlohmann::json;
using namespace std;

json extract_json_from_response(char *response)
{
    // Find the start of the JSON data (after the headers)
    const char *json_start = strstr(response, "\r\n\r\n");
    if (json_start)
    {
        json_start += 4; // Move past the header separator
        try
        {
            return json::parse(json_start);
        }
        catch (json::parse_error &e)
        {
            // Return empty JSON object if parsing fails
            return json();
        }
    }
    return json();
}

vector<string> get_cookies(char *response)
{
    vector<string> cookies;
    const char *search_start = response;

    // Find all "Set-Cookie:" headers
    while (true)
    {
        const char *cookie_header = strstr(search_start, "Set-Cookie: ");
        if (!cookie_header)
        {
            break;
        }

        cookie_header += 12; // Move past "Set-Cookie: "
        const char *cookie_end = strstr(cookie_header, ";");
        if (!cookie_end)
        {
            cookie_end = strstr(cookie_header, "\r\n");
            if (!cookie_end)
            {
                break;
            }
        }

        // Extract the cookie string (includes both name and value)
        string cookie_str(cookie_header, cookie_end);

        // Add the complete cookie (key=value) to the vector
        cookies.push_back(cookie_str);

        // Move search position for next iteration
        search_start = cookie_end;
    }

    return cookies;
}

//  SUBTASK: USERS
string login_adm(int sockfd, string &admin)
{
    char *message;
    char *response;

    string username;
    string password;
    json data;

    cout << "username=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> username;
    if (!username.empty() && username.length() < 100)
    {

        data["username"] = username;
    }
    else
    {
        printf("ERROR: Invalid credentials (Username Input error)!\n");
        return "";
    }

    cout << "password=";
    cout.flush();
    cin >> password;
    if (!password.empty() && password.length() < 100)
    {

        data["password"] = password;
    }
    else
    {
        printf("ERROR: Invalid credentials (Password Input error)!\n");
        return "";
    }

    message = compute_json_post_request(HOST, "/api/v1/tema/admin/login", data, NULL, 0, 0, "");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return "";
    }
    // Check for success message
    else if (response_json.contains("message"))
    {
        // printf("SUCCESS: %s\n", response_json["message"].get<string>().c_str());
        printf("SUCCESS: Admin autentificat cu succes\n");
        vector<string> cookie = get_cookies(response);
        admin = username;
        free(message);
        free(response);
        return cookie[0]; // session cookie
    }
    // Fallback for unexpected responses
    else
    {
        printf("ERROR: Unexpected response format\n");
        free(message);
        free(response);
        return "";
    }
}

void add_user(int sockfd, char **cookies)
{
    char *message;
    char *response;

    string username;
    string password;
    json data;

    cout << "username=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> ws;
    getline(cin, username);

    if (!username.empty() && username.length() < 100 && username.find(' ') == string::npos)
    {

        data["username"] = username;
    }
    else
    {
        printf("ERROR: Invalid username (Username Input error)!\n");
        return;
    }

    cout << "password=";
    cout.flush();
    cin >> ws;
    getline(cin, password);
    if (!password.empty() && password.length() < 100 && password.find(' ') == string::npos)
    {

        data["password"] = password;
    }
    else
    {
        printf("ERROR: Invalid password (Password Input error)!\n");
        return;
    }

    message = compute_json_post_request(HOST, "/api/v1/tema/admin/users", data, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    // Check for success message
    else if (response_json.contains("message"))
    {
        printf("SUCCESS: Utilizator adaugat cu succes\n");
        free(message);
        free(response);
        return;
        // return username + ":" + password; // return username and password
    }
}

void get_users(int sockfd, char **cookies)
{
    // GET /api/v1/tema/admin/users
    char *message = compute_get_request(HOST, "/api/v1/tema/admin/users", NULL, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    auto users_resp = response_json["users"];
    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        return;
    }
    else
    {
        int users_count = 1;
        cout << "SUCCESS: Lista utilizatorilor:" << endl;
        for (auto &user : users_resp)
        {
            cout << "#" << users_count++ << " "
                 << user["username"].get<string>() << ":"
                 << user["password"].get<string>() << endl;
        }
    }
    free(message);
    free(response);
}

void logout_admin(int sockfd, char **cookies, string &admin, string &ses)
{
    char *message = compute_get_request(HOST, "/api/v1/tema/admin/logout", NULL, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    // printf("Response from main server: %s\n", response);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else if (response_json.contains("message"))
    {
        admin = "-1";
        ses = "-1";
        printf("SUCCESS: Admin delogat\n");
        free(message);
        free(response);
        return;
    }
    else
    {
        printf("ERROR: Unexpected response format\n");
        free(message);
        free(response);
        return;
    }
}

void delete_user(int sockfd, char **cookies)
{
    string username;
    cout << "username=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> username;
    if (!username.empty() && username.length() < 100)
    {
    }
    else
    {
        printf("ERROR: Invalid username (Username Input error)!\n");
        return;
    }

    char *message = compute_delete(HOST, "/api/v1/tema/admin/users", username.c_str(), cookies, 1, 0, "");
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else if (response_json.contains("message"))
    {
        printf("SUCCESS: %s\n", response_json["message"].get<string>().c_str());
    }
    else
    {
        printf("ERROR: Unexpected response format\n");
    }
}

void logout_user(int sockfd, char **cookies, string &user, string &ses)
{
    char *message = compute_get_request(HOST, "/api/v1/tema/user/logout", NULL, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else if (response_json.contains("message"))
    {
        user = "-1";
        ses = "-1";
        printf("SUCCESS: Utilizator delogat\n");
        free(message);
        free(response);
    }
    else
    {
        printf("ERROR: Unexpected response format\n");
        free(message);
        free(response);
    }
}

string login_user(int sockfd, char **cookies, string &user)
{
    char *message;
    char *response;

    string admin_user;
    string username;
    string password;
    json data;

    cout << "admin_username=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> admin_user;
    if (!admin_user.empty() && admin_user.length() < 100)
    {
        data["admin_username"] = admin_user;
    }
    else
    {
        printf("ERROR: Invalid credentials (Admin Username Input error)!\n");
        return "";
    }

    cout << "username=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> username;
    if (!username.empty() && username.length() < 100)
    {

        data["username"] = username;
    }
    else
    {
        printf("ERROR: Invalid credentials (Username Input error)!\n");
        return "";
    }

    cout << "password=";
    cout.flush();
    cin >> password;
    if (!password.empty() && password.length() < 100)
    {

        data["password"] = password;
    }
    else
    {
        printf("ERROR: Invalid credentials (Password Input error)!\n");
        return "";
    }

    message = compute_json_post_request(HOST, "/api/v1/tema/user/login", data, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return "";
    }
    // Check for success message
    else if (response_json.contains("message"))
    {
        printf("SUCCESS: %s\n", response_json["message"].get<string>().c_str());
        vector<string> cookie = get_cookies(response);
        user = username;
        free(message);
        free(response);
        return cookie[0]; // session cookie
    }
    else
    {
        printf("ERROR: Unexpected response format\n");
        free(message);
        free(response);
        return "";
    }
}

string get_access(int sockfd, char **cookies)
{
    char *message = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, cookies, 1, 0, "");
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);

    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return "";
    }
    else
    {
        printf("SUCCESS: Token JWT primit\n");
        string token_jwt = response_json["token"].get<string>();
        free(message);
        free(response);
        return token_jwt; // session cookie
    }
}

// SUBTASK: MOVIES

void get_movies(int sockfd, char **cookies, string jwt)
{
    // GET /api/v1/tema/library/books
    char *message = compute_get_request(HOST, "/api/v1/tema/library/movies", NULL, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        cout << "SUCCESS: Lista filmelor:" << endl;
        for (auto &movie : response_json["movies"])
        {
            cout << "#" << movie["id"].get<int>() << " "
                 << movie["title"].get<string>() << endl;
        }
    }
    free(message);
    free(response);
}

void get_movie(int sockfd, char **cookies, string jwt)
{
    string id;
    cout << "id=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> id;
    printf("ID: %s\n", id.c_str());
    if (!id.empty() && id.length() < 100)
    {
    }
    else
    {
        printf("ERROR: Invalid title (Title Input error)!\n");
        return;
    }

    char *message = compute_get_request(HOST, "/api/v1/tema/library/movies", id.c_str(), cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        cout << "SUCCESS: Detalii film" << endl;
        cout << "title: " << response_json["title"].get<string>() << endl;
        cout << "year: " << response_json["year"].get<int>() << endl;
        cout << "description: " << response_json["description"].get<string>() << endl;
        cout << "rating: " << response_json["rating"].get<string>() << endl;

        free(message);
        free(response);
        return;
    }
}

void add_movie(int sockfd, char **cookies, string jwt)
{

    char *message;
    char *response;

    string title;
    string year;
    string description;
    string rating;
    json data;

    cout << "title=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> ws;    // Clear leading whitespace
    getline(cin, title);
    if (!title.empty() && title.length() < 100)
    {
        data["title"] = title;
    }
    else
    {
        printf("ERROR: Invalid title (Title Input error)!\n");
        return;
    }

    cout << "year=";
    cout.flush();
    cin >> ws;
    getline(cin, year);
    try
    {
        int year_value = stoi(year);
        if (year_value > 1500 && year_value <= 2025)
        {
            data["year"] = year_value;
        }
        else
        {
            printf("ERROR: Invalid year (Year Input error)!\n");
            return;
        }
    }
    catch (const std::invalid_argument &e)
    {
        printf("ERROR: Year must be a number!\n");
        return;
    }
    catch (const std::out_of_range &e)
    {
        printf("ERROR: Year value out of range!\n");
        return;
    }
    cout << "description=";
    cout.flush();
    cin >> ws;
    getline(cin, description);
    if (!description.empty() && description.length() < 100)
    {
        data["description"] = description;
    }
    else
    {
        printf("ERROR: Invalid description (Description Input error)!\n");
        return;
    }

    cout << "rating=";
    cout.flush();
    cin >> ws;
    getline(cin, rating);
    try
    {
        float rating_value = stof(rating);
        if (rating_value >= 0 && rating_value <= 10)
        {
            data["rating"] = rating_value;
        }
        else
        {
            printf("ERROR: Invalid rating (Rating Input error)!\n");
            return;
        }
    }
    catch (const std::invalid_argument &e)
    {
        printf("ERROR: Rating must be a number!\n");
        return;
    }
    catch (const std::out_of_range &e)
    {
        printf("ERROR: Rating value out of range!\n");
        return;
    }

    message = compute_json_post_request(HOST, "/api/v1/tema/library/movies", data, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        printf("SUCCESS: Film adaugat\n");
        free(message);
        free(response);
    }
}

void delete_movie(int sockfd, char **cookies, string jwt)
{
    string id;
    cout << "id=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> id;
    if (!id.empty() && id.length() < 100)
    {
    }
    else
    {
        printf("ERROR: Invalid title (Title Input error)!\n");
        return;
    }

    char *message = compute_delete(HOST, "/api/v1/tema/library/movies", id.c_str(), cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        printf("SUCCESS: Film sters cu succes!\n");
        free(message);
        free(response);
        return;
    }
}

void update_movie(int sockfd, char **cookies, string jwt)
{
    string id;
    cout << "id=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> id;
    if (!id.empty() && id.length() < 100)
    {
    }
    else
    {
        printf("ERROR: Invalid title (Title Input error)!\n");
        return;
    }

    string title;
    string year;
    string description;
    string rating;

    json data;
    cout << "title=";
    cout.flush(); // Flush to ensure prompt appears
    cin >> ws;    // Clear leading whitespace
    getline(cin, title);
    if (!title.empty() && title.length() < 100)
    {
        data["title"] = title;
    }
    else
    {
        printf("ERROR: Invalid title (Title Input error)!\n");
        return;
    }

    cout << "year=";
    cout.flush();
    cin >> ws;
    getline(cin, year);
    try
    {
        int year_value = stoi(year);
        if (year_value > 1500 && year_value <= 2025)
        {
            data["year"] = year_value;
        }
        else
        {
            printf("ERROR: Invalid year (Year Input error)!\n");
            return;
        }
    }
    catch (const std::invalid_argument &e)
    {
        printf("ERROR: Year must be a number!\n");
        return;
    }
    catch (const std::out_of_range &e)
    {
        printf("ERROR: Year value out of range!\n");
        return;
    }
    cout << "description=";
    cout.flush();
    cin >> ws;
    getline(cin, description);
    if (!description.empty() && description.length() < 100)
    {
        data["description"] = description;
    }
    else
    {
        printf("ERROR: Invalid description (Description Input error)!\n");
        return;
    }

    cout << "rating=";
    cout.flush();
    cin >> ws;
    getline(cin, rating);
    try
    {
        float rating_value = stof(rating);
        if (rating_value >= 0 && rating_value <= 10)
        {
            data["rating"] = rating_value;
        }
        else
        {
            printf("ERROR: Invalid rating (Rating Input error)!\n");
            return;
        }
    }
    catch (const std::invalid_argument &e)
    {
        printf("ERROR: Rating must be a number!\n");
        return;
    }
    catch (const std::out_of_range &e)
    {
        printf("ERROR: Rating value out of range!\n");
        return;
    }

    char *message = compute_json_put_request(HOST, "/api/v1/tema/library/movies", id.c_str(), data, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    // Parse the response and provide a meaningful message to the client
    json response_json = extract_json_from_response(response);
    // Check for error in response
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        printf("SUCCESS: Film actualizat\n");
        free(message);
        free(response);
        return;
    }
}

// SUBTASK: COLLECTIONS

void get_collections(int sockfd, char **cookies, string jwt)
{
    char *message = compute_get_request(HOST, "/api/v1/tema/library/collections", NULL, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        cout << "SUCCESS: Lista colecțiilor:" << endl;
        for (auto &collection : response_json["collections"])
        {
            cout << "#" << collection["id"].get<int>() << " "
                 << collection["title"].get<string>() << endl;
        }
    }
    free(message);
    free(response);
}

void get_collection(int sockfd, char **cookies, string jwt)
{
    string id;
    cout << "id=";
    cout.flush();
    cin >> id;
    if (id.empty() || id.length() >= 100)
    {
        printf("ERROR: Invalid collection ID!\n");
        return;
    }

    char *message = compute_get_request(HOST, "/api/v1/tema/library/collections", id.c_str(), cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
        free(message);
        free(response);
        return;
    }
    else
    {
        cout << "SUCCESS: Detalii colecție" << endl;
        cout << "title: " << response_json["title"].get<string>() << endl;
        cout << "owner: " << response_json["owner"].get<string>() << endl;
        for (auto &movie : response_json["movies"])
        {
            cout << "#" << movie["id"].get<int>() << ": " << movie["title"].get<string>() << endl;
        }
    }
    free(message);
    free(response);
}

void add_movie_to_collection(int sockfd, char **cookies, string jwt)
{
    string collection_id, movie_id;

    cout << "collection_id=";
    cout.flush();
    cin >> collection_id;
    if (collection_id.empty() || collection_id.length() >= 100)
    {
        printf("ERROR: Invalid collection ID!\n");
        return;
    }

    json data;
    cout << "movie_id=";
    cout.flush();
    cin >> movie_id;
    if (movie_id.empty() || movie_id.length() >= 100)
    {
        printf("ERROR: Invalid movie ID!\n");
        return;
    }
    else
    {
        data["id"] = stoi(movie_id);
    }

    string url = "/api/v1/tema/library/collections/" + collection_id + "/movies";

    char *message = compute_json_post_request(HOST, url.c_str(), data, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
    }
    else
    {
        printf("SUCCESS: Film adăugat în colecție\n");
    }
    free(message);
    free(response);
}

void add_collection(int sockfd, char **cookies, string jwt)
{
    string name;
    json data;

    cout << "title=";
    cout.flush();
    cin >> ws;
    getline(cin, name);
    if (name.empty() || name.length() >= 100)
    {
        printf("ERROR: Invalid collection name!\n");
        return;
    }
    data["title"] = name;

    char *message = compute_json_post_request(HOST, "/api/v1/tema/library/collections", data, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
    }
    else
    {
        printf("SUCCESS: Colecție adăugată\n");
    }

    int col_id = response_json["id"].get<int>();

    int num_movies;
    cout << "num_movies=";
    cout.flush();
    cin >> num_movies;

    if (num_movies < 0 || num_movies > 100)
    {
        printf("ERROR: Invalid number of movies!\n");
        return;
    }

    for (int i = 0; i < num_movies; i++)
    {
        json data2;
        cout << "movie_id[" << i << "]=";
        cout.flush();
        string movie_id;
        cin >> movie_id;
        if (movie_id.empty() || movie_id.length() >= 100)
        {
            printf("ERROR: Invalid movie ID!\n");
            break;
        }
        else
        {
            data2["id"] = stoi(movie_id);
        }

        string url2 = "/api/v1/tema/library/collections/" + to_string(col_id) + "/movies";

        char *message2 = compute_json_post_request(HOST, url2.c_str(), data2, cookies, 1, 1, jwt);
        send_to_server(sockfd, message2);
        char *response2 = receive_from_server(sockfd);

        json response_json2 = extract_json_from_response(response2);
        if (response_json2.contains("error"))
        {
            printf("ERROR: %s\n", response_json2["error"].get<string>().c_str());
            break;
        }
        else
        {
            printf("SUCCESS: Film adăugat în colecție\n");
        }
        free(message2);
        free(response2);
    }
    free(message);
    free(response);
}

void delete_collection(int sockfd, char **cookies, string jwt)
{
    string id;
    cout << "id=";
    cout.flush();
    cin >> id;
    if (id.empty() || id.length() >= 100)
    {
        printf("ERROR: Invalid collection ID!\n");
        return;
    }

    char *message = compute_delete(HOST, "/api/v1/tema/library/collections", id.c_str(), cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
    }
    else
    {
        printf("SUCCESS: Colecție ștearsă cu succes!\n");
    }
    free(message);
    free(response);
}

void delete_movie_from_collection(int sockfd, char **cookies, string jwt)
{
    string collection_id, movie_id;

    cout << "collection_id=";
    cout.flush();
    cin >> collection_id;
    if (collection_id.empty() || collection_id.length() >= 100)
    {
        printf("ERROR: Invalid collection ID!\n");
        return;
    }

    cout << "movie_id=";
    cout.flush();
    cin >> movie_id;
    if (movie_id.empty() || movie_id.length() >= 100)
    {
        printf("ERROR: Invalid movie ID!\n");
        return;
    }

    string url = "/api/v1/tema/library/collections/" + collection_id + "/movies/" + movie_id;

    char *message = compute_delete(HOST, url.c_str(), NULL, cookies, 1, 1, jwt);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    json response_json = extract_json_from_response(response);
    if (response_json.contains("error"))
    {
        printf("ERROR: %s\n", response_json["error"].get<string>().c_str());
    }
    else
    {
        printf("SUCCESS: Film eliminat din colecție\n");
    }
    free(message);
    free(response);
}
