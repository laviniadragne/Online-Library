# include "client.h"

// Helper pentru functia de enter_library si get_books
void get_msg(char *host_ip, int port, char *route, char* cookie, 
            char* token_jwt, int type, char* type_compute);

// Helper pentru comenzile ce folosesc compute_post_request
bool manage_msg_post(char* msg_to_send, char *host_ip, int port, char *route, char* type, 
                int type_log, char* cookie, char* token);