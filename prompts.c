#include "prompts.h"

void prompt_for_register(char* username, char* password) {
    printf("username");
    printf("=");
    fgets(username, MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';

    printf("password");
    printf("=");
    fgets(password, MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
}

void prompt_for_id(char* id) {
    printf("id");
    printf("=");

    fgets(id, MAX_LEN, stdin);
    id[strlen(id) - 1] = '\0';
}

void prompt_for_add(char* title, char* author, char* genre, char* publisher,
                    char* page_count) {
   
    printf("title");
    printf("=");
    fgets(title, MAX_LEN, stdin);
    // Scot '/n'
    title[strlen(title) - 1] = '\0';

    printf("author");
    printf("=");
    fgets(author, MAX_LEN, stdin);
    author[strlen(author) - 1] = '\0';

    printf("genre");
    printf("=");
    fgets(genre, MAX_LEN, stdin);
    genre[strlen(genre) - 1] = '\0';

    printf("publisher");
    printf("=");
    fgets(publisher, MAX_LEN, stdin);
    publisher[strlen(publisher) - 1] = '\0';

    printf("page_count");
    printf("=");
    fgets(page_count, MAX_LEN, stdin);
    page_count[strlen(page_count) - 1] = '\0';
    
}