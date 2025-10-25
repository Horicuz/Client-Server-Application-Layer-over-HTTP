#include "requests.h"
#include "helper.h"

using namespace std;
using json = nlohmann::json;

char *compute_get_request(string host,
                          string url,
                          const char *query_params,
                          char **cookies,
                          int cookies_count, int auth, string jwt)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL)
    {
        sprintf(line, "GET %s/%s HTTP/1.1", url.c_str(), query_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url.c_str());
    }
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);

    // Step 3: Add headers for JSON communication
    compute_message(message, "Accept: application/json");

    // Authorization: Bearer eijjkwuqioueu9182712093801293

    if (auth == 1)
    {
        string jwt_header = "Authorization: Bearer " + jwt;
        compute_message(message, jwt_header.c_str());
    }

    // Step 4: add cookies, according to the protocol format
    if (cookies != NULL && cookies_count > 0)
    {
        // Start with "Cookie: " and the first cookie
        sprintf(line, "Cookie: %s", cookies[0]);

        // Append additional cookies with semicolons
        for (int i = 1; i < cookies_count; i++)
        {
            strcat(line, "; ");
            strcat(line, cookies[i]);
        }
        compute_message(message, line);
    }

    // Step 5: add final new line
    compute_message(message, "");

    free(line);
    return message;
}

// New function specialized for JSON data
char *compute_json_post_request(string host,
                                const string url,
                                json &data,
                                char **cookies,
                                int cookies_count, int auth, string jwt)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Convert JSON to string
    std::string json_str = data.dump();

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url.c_str());
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);

    // Step 3: add necessary headers
    compute_message(message, "Content-Type: application/json");

    // Step 3.2: add Content-Length
    sprintf(line, "Content-Length: %zu", json_str.length());
    compute_message(message, line);

    if (auth == 1)
    {
        string jwt_header = "Authorization: Bearer " + jwt;
        compute_message(message, jwt_header.c_str());
    }

    // Step 4: add cookies
    if (cookies != NULL && cookies_count > 0)
    {
        // Start with "Cookie: " and the first cookie
        sprintf(line, "Cookie: %s", cookies[0]);

        // Append additional cookies with semicolons
        for (int i = 1; i < cookies_count; i++)
        {
            strcat(line, "; ");
            strcat(line, cookies[i]);
        }
        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual JSON payload data
    strcat(message, json_str.c_str());

    free(line);
    return message;
}

char *compute_delete(string host,
                     const string url,
                     const char *query_params,
                     char **cookies,
                     int cookies_count, int auth, string jwt)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL)
    {
        sprintf(line, "DELETE %s/%s HTTP/1.1", url.c_str(), query_params);
    }
    else
    {
        sprintf(line, "DELETE %s HTTP/1.1", url.c_str());
    }
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);

    if (auth == 1)
    {
        string jwt_header = "Authorization: Bearer " + jwt;
        compute_message(message, jwt_header.c_str());
    }

    // Step 3: add cookies, according to the protocol format
    if (cookies != NULL && cookies_count > 0)
    {
        // Start with "Cookie: " and the first cookie
        sprintf(line, "Cookie: %s", cookies[0]);

        // Append additional cookies with semicolons
        for (int i = 1; i < cookies_count; i++)
        {
            strcat(line, "; ");
            strcat(line, cookies[i]);
        }
        compute_message(message, line);
    }

    // Step 5: add final new line
    compute_message(message, "");

    free(line);
    return message;
}

char *compute_json_put_request(string host,
                               string url,
                               const char *query_params,
                               json &data,
                               char **cookies,
                               int cookies_count, int auth, string jwt)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Convert JSON to string
    std::string json_str = data.dump();

    if (query_params != NULL)
    {
        sprintf(line, "PUT %s/%s HTTP/1.1", url.c_str(), query_params);
    }
    else
    {
        sprintf(line, "PUT %s HTTP/1.1", url.c_str());
    }
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host.c_str());
    compute_message(message, line);

    // Step 3: add necessary headers
    compute_message(message, "Content-Type: application/json");

    // Step 3.2: add Content-Length
    sprintf(line, "Content-Length: %zu", json_str.length());
    compute_message(message, line);

    if (auth == 1)
    {
        string jwt_header = "Authorization: Bearer " + jwt;
        compute_message(message, jwt_header.c_str());
    }

    // Step 4: add cookies
    if (cookies != NULL && cookies_count > 0)
    {
        // Start with "Cookie: " and the first cookie
        sprintf(line, "Cookie: %s", cookies[0]);

        // Append additional cookies with semicolons
        for (int i = 1; i < cookies_count; i++)
        {
            strcat(line, "; ");
            strcat(line, cookies[i]);
        }
        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual JSON payload data
    strcat(message, json_str.c_str());

    free(line);
    return message;
}