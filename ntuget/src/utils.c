#include "utils.h"

/* Deprecated concat method, use snprintf instead.*/
bool concat(char *buffer, size_t buffer_size, int count, ...)
{
    va_list ap;
    int i;
    size_t len = 0;

    va_start(ap, count);
    for (i = 0; i < count; i++)
    {
        len += strlen(va_arg(ap, char *));
    }
    va_end(ap);

    if (len >= buffer_size)
        return false;

    va_start(ap, count);
    for (i = 0; i < count; i++)
    {
        strcat(buffer, va_arg(ap, char *));
    }
    va_end(ap);
    return true;
}

inline void print_header(struct curl_slist *headers)
{
    struct curl_slist *current = headers;
    while (current)
    {
        printf("%s\n", current->data);
        current = current->next;
    }
}

const char *executable_directory(void)
{
    char exePath[PATH_MAX];

#ifdef _WIN32
    DWORD result = GetModuleFileName(NULL, exePath, MAX_PATH);
    if (result == 0)
    {
        fprintf(stderr, "executable_directory: Error getting executable path: %lu\n", GetLastError());
        return NULL;
    }

    char *lastSlash = strrchr(exePath, '\\');
    if (lastSlash)
    {
        *(lastSlash + 1) = '\0';
    }

#else

    ssize_t result = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (result == -1)
    {
        perror("executable_directory: Error getting executable path");
        return NULL;
    }
    exePath[result] = '\0';

    char *lastSlash = strrchr(exePath, '/');
    if (lastSlash)
    {
        *(lastSlash + 1) = '\0';
    }
#endif

    char *directory = strdup(exePath);
    if (!directory)
    {
        fprintf(stderr, "executable_directory: Memory allocation failed.\n");
        return NULL;
    }

    return directory;
}

inline const char *get_full_path(void)
{
    const char *path = executable_directory();
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s/cookie.txt", path);
    free((void *)path);
    return strdup(full_path);
}
