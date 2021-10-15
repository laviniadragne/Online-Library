#include "utils.h"


// Helper pentru comenzile ce folosesc compute_post_request
bool manage_msg_post(char* msg_to_send, char *host_ip, int port, char *route, char* type, 
                int type_log, char* cookie, char* token) {
    int sockfd = open_connection(host_ip, port, AF_INET, SOCK_STREAM, 0);

    // Creez mesajul
    char* message_req = compute_post_request(host_ip, route, type, &msg_to_send, 1, NULL, 0, token);

    // Il trimit
    send_to_server(sockfd, message_req);

    // Primesc raspunsul si il afisez
    char* response = receive_from_server(sockfd);

    // Daca e deja autentificat, logat => primesc un json de eroare
    char* error_response = strdup(response);

    // Verific daca e eroare
    bool flag_error = false;
    char* start_error = strstr(error_response, "error");

    if (start_error != NULL) {
        strtok(start_error, "\n");
        printf("%s\n", start_error);
        flag_error = true;
    }
    
    free(error_response);

    // E un mesaj pentru logare
    if ((type_log == LOG_TYPE) && (flag_error == false)) {
        char* copy_response = strdup(response);

        // Memorez cookie-ul
        char* start = strstr(copy_response, LOG_ANS);
        strtok(start, ";");

        if (start != NULL) {
            strcpy(cookie, start);
        }

        free(copy_response);
    }

    free(message_req);

    bool print = false;
    // Nu am primit eroare
    if (flag_error == false) {
         print = true;
    }

    free(response);
    close(sockfd);
    return print;
}

// Helper pentru functia de enter_library si get_books
void get_msg(char *host_ip, int port, char *route, char* cookie, 
            char* token_jwt, int type, char* type_compute) {
    int sockfd = open_connection(host_ip, port, AF_INET, SOCK_STREAM, 0);
    char* message_req;

    // Creez mesajul
    if (type == ENTER_TYPE) {
        message_req = compute_get_request(host_ip, route, NULL, &cookie, 1, NULL, type_compute);
    }
    else {
        if (type == GET_BOOKS_TYPE || type == DELETE_BOOK_TYPE || type == LOGOUT_TYPE) {
            message_req = compute_get_request(host_ip, route, NULL, &cookie, 1, token_jwt, type_compute);
        }
    }

    // Il trimit
    send_to_server(sockfd, message_req);

    // Primesc raspunsul si il afisez
    char* response = receive_from_server(sockfd);

    // Daca nu e autentificat => primesc un json de eroare
    char* error_response = strdup(response);

    // Verific daca e eroare
    bool flag_error = false;
    char* start_error = strstr(error_response, "error");

    if (start_error != NULL) {
        strtok(start_error, "\n");
        printf("%s\n", start_error);
        flag_error = true;
    }
    
    free(error_response);

    // E un mesaj bun pentru entry_library, memorez token-ul
    if (flag_error == false && type == ENTER_TYPE) {
        char* copy_response = strdup(response);

        // Memorez token-ul
        char* start = strstr(copy_response, "eyJ");
        strtok(start, "\"");

        if (start != NULL) {
            strcpy(token_jwt, start);
        }

        free(copy_response);
    }

    free(message_req);

    // Parsez dupa lista [ ] pentru get_books
    if (flag_error == false && type == GET_BOOKS_TYPE) {

        char* show_response = strdup(response);

        // Memorez lista
        char* start = strchr(show_response, '[');
        char* end = strchr(show_response, ']');
        int dif = strlen(start) - strlen(end) + 1;

        char* to_print = calloc(MAX_MSG, sizeof(char));

        // Afisez lista
        if (start != NULL && end != NULL) {
            strncpy(to_print, start, dif);
            printf("%s\n", to_print);
        }

        free(to_print);
        free(show_response);
    }

    // Nu am primit eroare, doar afisez token-ul pentru enter_type
    if (flag_error == false && type == ENTER_TYPE) {
        printf("You have access to the library!\n");
    }

    // Afisez mesajul pentru delete_book_type
    if (flag_error == false && type == DELETE_BOOK_TYPE) {
        printf("Book deleted successfully!\n");
    }

     // Afisez mesajul pentru logout
    if (flag_error == false && type == LOGOUT_TYPE) {
        printf("Logout successfully!\n");
    }

    free(response);
    close(sockfd);
}