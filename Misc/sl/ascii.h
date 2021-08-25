#pragma once

/**************************************************************************************************
 DEFINES (change as desired)
**************************************************************************************************/

#define MAX_TEXT_LENGTH         50
#define MAX_TEXT_LENGTH_STR     "50"
#define MAX_REQUEST_LENGTH      1024
#define RESPONSE_BUF_SIZE       50000
#define ENDPOINT_DOMAIN_1       "ar"
#define ENDPOINT_DOMAIN_2       "tii.hero"
#define ENDPOINT_DOMAIN_3       "kuapp.com"
#define ENDPOINT_DOMAIN         ENDPOINT_DOMAIN_1 ENDPOINT_DOMAIN_2 ENDPOINT_DOMAIN_3
#define ENDPOINT_PORT           80
#define ENDPOINT_PATH           "make?text=%s"

/**************************************************************************************************
 INTERFACE
**************************************************************************************************/

struct ascii_art
{
    char** data;
    int    length;
    int    height;
};

struct ascii_art get_ascii_art(const char* text); 
