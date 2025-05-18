#include "requests.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 104857600

// Get the file extension from a file name
const char *get_file_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) {
        return "";
    }
    return dot + 1;
}

// Get the MIME type based on file extension
const char *get_mime_type(const char *file_ext) {
    if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0) {
        return "text/html";
    } else if (strcasecmp(file_ext, "txt") == 0) {
        return "text/plain";
    } else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcasecmp(file_ext, "png") == 0) {
        return "image/png";
    } else if (strcasecmp(file_ext, "css") == 0) {
        return "text/css";
    } else if (strcasecmp(file_ext, "js") == 0) {
        return "application/javascript";
    } else if (strcasecmp(file_ext, "json") == 0) {
        return "application/json";
    } else if (strcasecmp(file_ext, "pdf") == 0) {
        return "application/pdf";
    } else {
        return "application/octet-stream";
    }
}

// Compare two strings case insensitively
bool case_insensitive_compare(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return false;
        }
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

// Find a file name in the current directory, ignoring case
char *get_file_case_insensitive(const char *file_name) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    char *found_file_name = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (case_insensitive_compare(entry->d_name, file_name)) {
            found_file_name = strdup(entry->d_name);
            break;
        }
    }

    closedir(dir);
    return found_file_name;
}

// Decode URL encoded strings
char *url_decode(const char *src) {
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    size_t decoded_len = 0;

    // decode %2x to hex
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] == '%' && i + 2 < src_len) {
            int hex_val;
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len++] = hex_val;
            i += 2;
        } else if (src[i] == '+') {
            // Convert '+' to space
            decoded[decoded_len++] = ' ';
        } else {
            decoded[decoded_len++] = src[i];
        }
    }

    // add null terminator
    decoded[decoded_len] = '\0';
    return decoded;
}

// Build HTTP response based on requested file
void build_http_response(const char *file_name, 
                        const char *file_ext, 
                        char *response, 
                        size_t *response_len) {
    // build HTTP header
    const char *mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             mime_type);

    // Try to find the file with case-insensitive matching
    char *actual_file_name = get_file_case_insensitive(file_name);
    
    // If file not exist, response is 404 Not Found
    int file_fd;
    if (actual_file_name != NULL) {
        file_fd = open(actual_file_name, O_RDONLY);
        free(actual_file_name);
    } else {
        file_fd = open(file_name, O_RDONLY);
    }
    
    if (file_fd == -1) {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found - The requested file '%s' was not found on this server.",
                 file_name);
        *response_len = strlen(response);
        free(header);
        return;
    }

    // get file size for Content-Length
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;

    // copy header to response buffer
    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    // copy file to response buffer
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, 
                            response + *response_len, 
                            BUFFER_SIZE - *response_len)) > 0) {
        *response_len += bytes_read;
    }
    free(header);
    close(file_fd);
}

// Handle client request
void handle_client_request(int client_fd) {
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        perror("Failed to allocate memory for request buffer");
        close(client_fd);
        return;
    }

    // receive request data from client and store into buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        // Print received request for debugging
        printf("Received request:\n%.*s\n", (int)bytes_received, buffer);
        buffer[bytes_received] = '\0';  // Ensure null termination

        // check if request is GET
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0) {
            // extract filename from request and decode URL
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);
            
            printf("Requested file: '%s'\n", file_name);

            // Default to index.html if root path is requested
            if (strlen(file_name) == 0 || strcmp(file_name, "/") == 0) {
                free(file_name);
                file_name = strdup("index.html");
                printf("Defaulting to index.html\n");
            }

            // get file extension
            char file_ext[32];
            strcpy(file_ext, get_file_extension(file_name));
            printf("File extension: '%s'\n", file_ext);

            // build HTTP response
            char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
            if (response == NULL) {
                perror("Failed to allocate memory for response buffer");
                free(file_name);
                regfree(&regex);
                free(buffer);
                close(client_fd);
                return;
            }

            size_t response_len;
            build_http_response(file_name, file_ext, response, &response_len);

            // send HTTP response to client
            send(client_fd, response, response_len, 0);
            printf("Response sent: %zu bytes\n", response_len);

            free(response);
            free(file_name);
        } else {
            // Not a GET request or invalid format
            const char *error_response = 
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "400 Bad Request - Only GET requests are supported";
            
            send(client_fd, error_response, strlen(error_response), 0);
            printf("Bad request - not a valid GET request\n");
        }
        regfree(&regex);
    } else {
        printf("Error receiving data or connection closed\n");
    }
    
    close(client_fd);
    free(buffer);
}