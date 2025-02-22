#ifndef REQUEST_H
#define REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#include <sys/stat.h>
#endif

#include <curl/curl.h>
#include "config.h"
#include "utils.h"

typedef struct
{
    long status_code;
    struct curl_slist *response_headers;
    char *response_body;
    struct curl_slist *cookies;
    char *redirect_url;
} RequestResponse;

typedef struct
{
    char *url;
    struct curl_slist **request_headers;
    char *cookies;
    bool follow; // Redirect
    bool verbose;
    char *post_fields;
} RequestOptions;

RequestResponse *request_get(RequestOptions opt);

RequestResponse *request_post(RequestOptions opt);

void request_response_cleanup(RequestResponse *res);

void request_options_cleanup(RequestOptions *opt);

#endif
