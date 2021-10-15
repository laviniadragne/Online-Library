#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <stdbool.h>
#include "parson.h"

# define ENTER_TYPE 0
# define GET_BOOKS_TYPE 1
# define REG_TYPE 0
# define LOG_TYPE 1
# define ADD_TYPE 2
# define DELETE_BOOK_TYPE 3
# define LOGOUT_TYPE 4
# define MAX_LEN 100
# define MAX_MSG 10000 
# define LOG_ANS  "connect.sid"

void parse_json_register(char *username, char* password, char* msg_to_send);

void parse_json_add(char* title, char* author, char* genre, char* publisher,
                    char* page_count, char* msg_to_send);