#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern const char base64_map[];

extern const char *reserved;

bool encoder_url_encode(const char *data, char *buffer, size_t buffer_size); // https://gist.github.com/jesobreira/4ba48d1699b7527a4a514bfa1d70f61a

bool encoder_base64_encode(const char *data, char *buffer, size_t buffer_size); // https://github.com/elzoughby/Base64

bool encoder_base64_decode(const char *data, char *buffer, size_t buffer_size);

#endif
