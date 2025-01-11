#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <curl/curl.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <libgen.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 260
#endif

bool concat(char *buffer, size_t buffer_size, int count, ...);

void print_header(struct curl_slist *headers);

const char *executable_directory(void);

#endif
