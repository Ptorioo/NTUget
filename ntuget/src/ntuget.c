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

    printf("Unread conversation count: \n");
    parser_unreadCount();
    printf("--------------------\n");

    printf("Enrolled courses: \n");
    parser_dashboard();
    printf("--------------------\n");

    printf("Activities & Announcements: \n");
    parser_planner();

    return 0;
}
