#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_parser.h"
#include "login.h"

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        LoginStatus status = login(argv[1], argv[2], false);
        printf("Login returned status: %d\n", status);
    }

    parser_unreadCount();
    return 0;
}
