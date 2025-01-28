#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#endif

#include "api_parser.h"
#include "login.h"

void get_password(char *password, size_t max_len)
{
#ifdef _WIN32
    printf("Enter password: ");
    size_t i = 0;
    int ch;

    while (i < max_len - 1 && (ch = _getch()) != '\r')
    {
        if (ch == '\b' && i > 0)
        {
            printf("\b \b");
            i--;
        }
        else if (ch != '\b')
        {
            password[i++] = (char)ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Enter password: ");
    if (!fgets(password, max_len, stdin))
    {
        fprintf(stderr, "Error reading password.\n");
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        exit(1);
    }
    printf("\n");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    password[strcspn(password, "\n")] = '\0';
#endif
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    system("chcp 65001 > NUL");
#endif

    char usr[50];
    char pwd[50];

    printf("Enter username: ");
    if (!fgets(usr, sizeof(usr), stdin))
    {
        fprintf(stderr, "Error reading username.\n");
        return 1;
    }

    usr[strcspn(usr, "\n")] = '\0';

    get_password(pwd, sizeof(pwd));

    LoginStatus status = login(usr, pwd, true);
    printf("Login returned status: %d\n", status);
    if (status == 0)
    {
        printf("Unread conversation count: \n");
        parser_unreadCount();
        printf("--------------------\n");

        printf("Enrolled courses: \n");
        parser_dashboard();
        printf("--------------------\n");

        printf("Activities & Announcements: \n");
        parser_planner();
    }

    return 0;
}
