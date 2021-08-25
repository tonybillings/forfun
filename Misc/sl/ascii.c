/**************************************************************************************************
 INCLUDES
**************************************************************************************************/

#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ascii.h"

/**************************************************************************************************
 FUNCTIONS
**************************************************************************************************/

struct ascii_art get_ascii_art(const char* text) 
{
    const char* domain = ENDPOINT_DOMAIN;
  
    struct ascii_art empty_art;
    empty_art.data = NULL;
    empty_art.length = empty_art.height = 0;
    
    if (strlen(text) > MAX_TEXT_LENGTH) return empty_art;

    char* encoded_text = calloc(MAX_TEXT_LENGTH * 6, sizeof(char));
    int text_idx = 0;
    int encoded_text_idx = 0;
    while (text[text_idx])
    {
        if (text[text_idx] == ' ')
        {
            memcpy(&encoded_text[encoded_text_idx], "%20%20", 6);
            encoded_text_idx += 6;
        }
        else
        {
            encoded_text[encoded_text_idx++] = text[text_idx];
        }

        text_idx++;
    }   

    unsigned short server_port = ENDPOINT_PORT;

    char buffer[BUFSIZ];
    char request[MAX_REQUEST_LENGTH];
    char request_template[] = "GET /" ENDPOINT_PATH " HTTP/1.1\r\nHost: " ENDPOINT_DOMAIN "\r\n\r\n";
    struct protoent* protoent;
    in_addr_t in_addr;
    int request_length;
    int socket_file_desc;
    ssize_t nbytes_total, nbytes_last;
    struct hostent* hostent;
    struct sockaddr_in sock_addr_in;

    request_length = snprintf(request, MAX_REQUEST_LENGTH, request_template, encoded_text, domain);
    
    if (request_length >= MAX_REQUEST_LENGTH) 
        return empty_art;

    protoent = getprotobyname("tcp");
    if (protoent == NULL) 
        return empty_art;
 
    socket_file_desc = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    if (socket_file_desc == -1) 
        return empty_art;

    hostent = gethostbyname(domain);
    if (hostent == NULL) 
        return empty_art;
 
    in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
    if (in_addr == (in_addr_t)-1) 
        return empty_art;
    
    sock_addr_in.sin_addr.s_addr = in_addr;
    sock_addr_in.sin_family = AF_INET;
    sock_addr_in.sin_port = htons(server_port);

    if (connect(socket_file_desc, (struct sockaddr*)&sock_addr_in, sizeof(struct sockaddr)) == -1)
        return empty_art;

    nbytes_total = 0;
    while (nbytes_total < request_length) 
    {
        nbytes_last = write(socket_file_desc, request + nbytes_total, request_length - nbytes_total);

        if (nbytes_last == -1) 
            return empty_art;
 
        nbytes_total += nbytes_last;
    }
    
    char* response = calloc(RESPONSE_BUF_SIZE, sizeof(char));
    int response_bytes_read = read(socket_file_desc, buffer, BUFSIZ); // we know 20 chars converted to ASCII art will fit in buffer, so read just once
    close(socket_file_desc);
    if (response_bytes_read == 0) return empty_art;
    memcpy(response, buffer, response_bytes_read);

    struct ascii_art result;
    result.length = result.height = 1;
    int length = 0;

    for (int i = 0; i < response_bytes_read; ++i)
    {
        if (response[i] == '\n')
        {
            result.length = result.length > length ? result.length : length;
            result.height++;
            length = 0;
        }
        else
        {
            length++;
        }        
    } 

    result.data = calloc(result.height, sizeof(char*));
    length = 0;
    char line_buf[result.length];
    memset(line_buf, 0, result.length);
    int col_index = 0;
    int line_index = 0;
    bool found_start = false;

    for (int i = 0; i < response_bytes_read; ++i)
    {
        if ((col_index == 0 && response[i] == '\n') || (col_index == 1 && response[i] == '\n' && line_buf[0] == '\r'))
            found_start = true;
        
        line_buf[col_index++] = response[i];
      
        if (response[i] == '\n')
        {
            if (found_start)
            {
                result.data[line_index] = calloc(col_index + 1, sizeof(char));
                memcpy(result.data[line_index++], line_buf, col_index);
            }

            memset(line_buf, 0, result.length);
            col_index = 0;
        }        
    }
    
    if (line_buf[0] != 0)
    {
        if (found_start)
        {
            result.data[line_index] = calloc(col_index + 1, sizeof(char));
            memcpy(result.data[line_index++], line_buf, col_index);
        }
    }

    result.height = line_index;
    return result;
}

