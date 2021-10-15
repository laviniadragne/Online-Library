#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

// type-ul poate fi "GET" sau "DELETE"
char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* token, char* type)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "%s %s?%s HTTP/1.1", type, url, query_params);
    } else {
        sprintf(line, "%s %s HTTP/1.1", type, url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    memset(line, 0, LINELEN);

    if (cookies != NULL) {
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
    }
    compute_message(message, line);

    // Add token-ul
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer");
        char* final_line = line + strlen(line); 
        sprintf(final_line," %s", token);

        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");

    free(line);

    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count,
                            char* token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */

    int size = 0;
    if (body_data) {
        for (int i = 0; i < body_data_fields_count - 1; i++) {
            strcat(body_data_buffer, body_data[i]);
            strcat(body_data_buffer, "&");
            size += strlen(body_data[i]) + 1;
        }

        strcat(body_data_buffer, body_data[body_data_fields_count - 1]);
        size += strlen(body_data[body_data_fields_count - 1]);
    }

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %d", size);
    compute_message(message, line);

    // Step 4 (optional): add cookies
    memset(line, 0, LINELEN);

    if (cookies != NULL) {
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }


    // Adauga token-ul
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer");
        char* final_line = line + strlen(line); 
        sprintf(final_line," %s", token);

        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);

    return message;
}
