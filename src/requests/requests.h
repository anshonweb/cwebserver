
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <stdbool.h>
#include <stddef.h>

// Function to handle a client request
void handle_client_request(int client_fd);

// Helper function to get file extension
const char *get_file_extension(const char *file_name);

// Helper function to get MIME type based on file extension
const char *get_mime_type(const char *file_ext);

// Helper function for case-insensitive string comparison
bool case_insensitive_compare(const char *s1, const char *s2);

// Helper function to find a file case-insensitively
char *get_file_case_insensitive(const char *file_name);

// Helper function to decode URL-encoded strings
char *url_decode(const char *src);

// Helper function to build HTTP response
void build_http_response(const char *file_name, 
                        const char *file_ext, 
                        char *response, 
                        size_t *response_len);

#endif // REQUEST_HANDLER_H