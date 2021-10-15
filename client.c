#include "client.h"
#include "prompts.h"
#include "utils.h"


void parse_json_register(char *username, char* password, char* msg_to_send) {
    JSON_Value *value = json_value_init_object();
    JSON_Object *obj = json_value_get_object(value);

    json_object_set_string(obj, "username", username);
    json_object_set_string(obj, "password", password);

    char* msg_to_send_intern = json_serialize_to_string(value);
    strcpy(msg_to_send, msg_to_send_intern);

    json_value_free(value);
    free(msg_to_send_intern);
}

void parse_json_add(char* title, char* author, char* genre, char* publisher,
                    char* page_count, char* msg_to_send) {
    JSON_Value *value = json_value_init_object();
    JSON_Object *obj = json_value_get_object(value);

    json_object_set_string(obj, "title", title);
    json_object_set_string(obj, "author", author);
    json_object_set_string(obj, "genre", genre);
    json_object_set_string(obj, "publisher", publisher);
    json_object_set_string(obj, "page_count", page_count);


    char* msg_to_send_intern = json_serialize_to_string(value);
    strcpy(msg_to_send, msg_to_send_intern);

    json_value_free(value);
    free(msg_to_send_intern);
}

int main(int argc, char *argv[])
{

    char route_register[MAX_LEN] = "/api/v1/tema/auth/register";
    char route_login[MAX_LEN] = "/api/v1/tema/auth/login";
    char route_acces[MAX_LEN] = "/api/v1/tema/library/access";
    char route_op[MAX_LEN] = "/api/v1/tema/library/books";
    char route_logout[MAX_LEN] = "/api/v1/tema/auth/logout";
    char type[MAX_LEN] = "application/json";
    char host_ip[MAX_LEN] = "34.118.48.238";
    int port = 8080;
    char line[MAX_LEN];
    char command[MAX_LEN];
    char* username = calloc(MAX_LEN, sizeof(char));
    char* password = calloc(MAX_LEN, sizeof(char));
    char* cookie = calloc(MAX_MSG, sizeof(char));
    char* msg_to_send;
    char* token_jwt = calloc(MAX_MSG, sizeof(char));
    char* id = calloc(MAX_LEN, sizeof(char));
    char* title = calloc(MAX_LEN, sizeof(char));
    char* author = calloc(MAX_LEN, sizeof(char));
    char* genre = calloc(MAX_LEN, sizeof(char));
    char* publisher = calloc(MAX_LEN, sizeof(char));
    char* page_count = calloc(MAX_LEN, sizeof(char));


    while(true) {
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%s", command);
    
        // Primesc de la client comanda de exit, intrerup
        // functionarea
        if (strcmp(command, "exit") == 0) {
            break;
        }
        else {
            // Clientul se inregistreaza
            if (strcmp(command, "register") == 0) {
                // Creeaza prompt-ul pentru client
                prompt_for_register(username, password);

                // Parseaza intr-un obiect json
                msg_to_send = calloc(MAX_MSG, sizeof(char));
                parse_json_register(username, password, msg_to_send);

                bool print = manage_msg_post(msg_to_send, host_ip, port, route_register, type, REG_TYPE, cookie, NULL);
                if (print) {
                    printf("Register successfully!\n");
                }
                free(msg_to_send);
            }
            // Clientul se logheaza
            else {
                if (strcmp(command, "login") == 0) {
                    // Am deja un cookie
                    if (cookie[0] != '\0') {
                        printf("Someone already connected!\n");
                    }
                    else {
                        // Creez prompt-ul
                        prompt_for_register(username, password);

                        // Parsez intr-un obiect json
                        msg_to_send = calloc(MAX_LEN, sizeof(char));
                        parse_json_register(username, password, msg_to_send);
                        bool print = manage_msg_post(msg_to_send, host_ip, port, route_login, type, LOG_TYPE, cookie, NULL);

                        if (print) {
                            printf("Login successfully!\n");
                        }

                        free(msg_to_send);
                    }
                }
                else {
                    if (strcmp(command, "enter_library") == 0) {
                        if (cookie[0] == '\0') {
                            printf("You are not connected!\n");
                        }
                        else {
                            get_msg(host_ip, port, route_acces, cookie, token_jwt, ENTER_TYPE, "GET");

                        }
                    }
                    else {
                        if (strcmp(command, "get_books") == 0) {
                            if (token_jwt[0] == '\0') {
                                printf("You don't have acces!\n");
                            }
                            else {
                                get_msg(host_ip, port, route_op, cookie, token_jwt, GET_BOOKS_TYPE, "GET");
                            }
                        }
                        else {
                            if (strcmp(command, "get_book") == 0) {
                                if (token_jwt[0] == '\0') {
                                    printf("You don't have acces!\n");
                                }
                                else {
                                    // Creez prompt-ul
                                    prompt_for_id(id);
                                    char* new_route = calloc(MAX_MSG, sizeof(char));
                                    strcpy(new_route, route_op);
                                    strcat(new_route, "/");
                                    strcat(new_route, id);

                                    get_msg(host_ip, port, new_route, cookie, token_jwt, GET_BOOKS_TYPE, "GET");

                                    free(new_route);
                                }
                            }
                            else {
                                 if (strcmp(command, "add_book") == 0) {
                                    if (token_jwt[0] == '\0') {
                                        printf("You don't have acces!\n");
                                    }
                                    else {
                                        // Creez prompt-ul
                                        prompt_for_add(title, author, genre, publisher, page_count);

                                        // String-ul pentru nr de pagini nu e numar
                                        if (atoi(page_count) == 0) {
                                            printf("Number of pages not number!\n");
                                        }
                                        else {
                                            // Parsez intr-un obiect json
                                            msg_to_send = calloc(MAX_LEN, sizeof(char));
                                            parse_json_add(title, author, genre, publisher, page_count, msg_to_send);
                                            bool print = manage_msg_post(msg_to_send, host_ip, port, route_op, type, ADD_TYPE, cookie, token_jwt);
                                            
                                            if (print) {
                                                printf("Adding book was successfully!\n");
                                            }
                                            
                                            free(msg_to_send);
                                        }
                                    }
                                }
                                else {
                                    if (strcmp(command, "delete_book") == 0) {
                                        if (token_jwt[0] == '\0') {
                                            printf("You don't have acces!\n");
                                        }
                                        else {
                                            // Creez prompt-ul
                                            prompt_for_id(id);

                                            // Creez ruta
                                            char* new_route = calloc(MAX_MSG, sizeof(char));
                                            strcpy(new_route, route_op);
                                            strcat(new_route, "/");
                                            strcat(new_route, id);

                                            get_msg(host_ip, port, new_route, cookie, token_jwt, DELETE_BOOK_TYPE, "DELETE");

                                            free(new_route);
                                        }
                                    }
                                    else {
                                        if (strcmp(command, "logout") == 0) {
                                            if (cookie[0] == '\0') {
                                                printf("You aren't logged in!\n");
                                            }
                                            else {
                                                
                                                get_msg(host_ip, port, route_logout, cookie, token_jwt, LOGOUT_TYPE, "GET");

                                                // Curat cookie-ul si token-ul
                                                if(cookie[0] != '\0') {
                                                    free(cookie);
                                                    cookie = calloc(MAX_MSG, sizeof(char));
                                                }
                                                if(token_jwt[0] != '\0') {
                                                    free(token_jwt);
                                                    token_jwt = calloc(MAX_MSG, sizeof(char));
                                                }
                                                
                                            }
                                        }
                                        else {
                                            printf("Wrong command\n");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(username);
    free(password);
    free(cookie);
    free(token_jwt);
    free(id);
    free(title);
    free(author);
    free(genre);
    free(publisher);
    free(page_count);
    return 0;
}
