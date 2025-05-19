
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <stdbool.h>
#include <stddef.h>


void handle_client_request(int client_fd);


const char *get_file_extension(const char *file_name);


const char *get_mime_type(const char *file_ext);


bool case_insensitive_compare(const char *s1, const char *s2);


char *get_file_case_insensitive(const char *file_name);


char *url_decode(const char *src);

void build_http_response(const char *file_name, 
                        const char *file_ext, 
                        char *response, 
                        size_t *response_len);

#endif 