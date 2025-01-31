#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdbool.h>
#include "config.h"
#include "request.h"
#include "utils.h"
#include "encoder.h"

#define PAYLOAD_SIZE 1024

typedef enum
{
    LOGIN_SUCCESS,       // Login successful
    LOGIN_AUTH_ERROR,    // Invalid username or password
    LOGIN_UNAVAILABLE,   // Service provider is down or network having issues
    LOGIN_FORBIDDEN,     // User does not have access to the requested service
    LOGIN_TOKEN_INVALID, // Authentication token is invalid or malformed
    LOGIN_SERVICE_ERROR  // Unknown service errors
} LoginStatus;

typedef struct
{
    union
    {
        char *result;
        int status;
    };
    int is_error;
} ParseResult;

LoginStatus login(const char *username, const char *pwd, bool verbose);

#endif
