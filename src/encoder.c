#include "encoder.h"

const char base64_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                           'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                           'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                           'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

const char *reserved = "!*()-_'.~";

bool encoder_url_encode(const char *data, char *buffer, size_t buffer_size)
{
    const char *hex = "0123456789ABCDEF";
    size_t pos = 0;

    for (size_t i = 0; i < strlen(data); i++)
    {
        if (('a' <= data[i] && data[i] <= 'z') || ('A' <= data[i] && data[i] <= 'Z') || ('0' <= data[i] && data[i] <= '9') || strchr(reserved, data[i]))
        {
            if (pos + 1 >= buffer_size)
                return false;
            buffer[pos++] = data[i];
        }
        else
        {
            if (pos + 3 >= buffer_size)
                return false;
            buffer[pos++] = '%';
            buffer[pos++] = hex[data[i] >> 4];
            buffer[pos++] = hex[data[i] & 15];
        }
    }

    if (pos + 1 >= buffer_size)
        return false;
    buffer[pos] = '\0';

    return true;
}

/***********************************************************
 * Base64 library implementation                            *
 * @author Ahmed Elzoughby                                  *
 * @date July 23, 2017                                      *
 ***********************************************************/

bool encoder_base64_encode(const char *data, char *buffer, size_t buffer_size)
{
    unsigned char counts = 0;
    char temp[3];
    size_t data_len = strlen(data);
    size_t required_size = ((data_len + 2) / 3) * 4 + 1;

    if (buffer_size < required_size)
        return false;

    size_t c = 0;

    for (size_t i = 0; i < data_len; i++)
    {
        temp[counts++] = data[i];
        if (counts == 3)
        {
            buffer[c++] = base64_map[temp[0] >> 2];
            buffer[c++] = base64_map[((temp[0] & 0x03) << 4) | (temp[1] >> 4)];
            buffer[c++] = base64_map[((temp[1] & 0x0f) << 2) | (temp[2] >> 6)];
            buffer[c++] = base64_map[temp[2] & 0x3f];
            counts = 0;
        }
    }

    if (counts > 0)
    {
        buffer[c++] = base64_map[temp[0] >> 2];
        if (counts == 1)
        {
            buffer[c++] = base64_map[(temp[0] & 0x03) << 4];
            buffer[c++] = '=';
            buffer[c++] = '=';
        }
        else if (counts == 2)
        {
            buffer[c++] = base64_map[((temp[0] & 0x03) << 4) | (temp[1] >> 4)];
            buffer[c++] = base64_map[(temp[1] & 0x0f) << 2];
            buffer[c++] = '=';
        }
    }

    buffer[c] = '\0';
    return true;
}

bool encoder_base64_decode(const char *data, char *buffer, size_t buffer_size)
{
    unsigned char counts = 0;
    char temp[4];
    size_t data_len = strlen(data);
    size_t required_size = (data_len * 3) / 4 + 1;

    if (buffer_size < required_size)
        return false;

    size_t p = 0;

    for (size_t i = 0; i < data_len; i++)
    {
        if (data[i] == '=')
            break;

        unsigned char k = 0;
        while (k < 64 && base64_map[k] != data[i])
            k++;
        if (k == 64)
            return false;

        temp[counts++] = k;

        if (counts == 4)
        {
            buffer[p++] = (temp[0] << 2) | (temp[1] >> 4);
            if (temp[2] != 64)
                buffer[p++] = (temp[1] << 4) | (temp[2] >> 2);
            if (temp[3] != 64)
                buffer[p++] = (temp[2] << 6) | temp[3];
            counts = 0;
        }
    }

    if (counts > 0)
    {
        if (counts >= 2)
        {
            buffer[p++] = (temp[0] << 2) | (temp[1] >> 4);
        }
        if (counts == 3)
        {
            buffer[p++] = (temp[1] << 4) | (temp[2] >> 2);
        }
    }

    buffer[p] = '\0';
    return true;
}
