#include "test.h"

char *hexdump(const unsigned char *data, size_t len)
{
    if (!data || len == 0)
    {
        return strdup("");
    }

    size_t output_len = len * 3;
    char *output = (char *)malloc(output_len + 1);
    if (!output)
    {
        return NULL;
    }

    char *ptr = output;
    for (size_t i = 0; i < len; i++)
    {
        sprintf(ptr, "%02x ", data[i]);
        ptr += 3;
    }
    *(ptr - 1) = '\0';

    return output;
}

int main(int argc, char **argv)
{
    int result = test(argc > 1 ? argv[1] : NULL);

    if (result != 0)
        printf("Some tests failed. Total failures: %d\n", unitfail);

    return result;
}