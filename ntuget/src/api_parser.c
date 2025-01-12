#include "api_parser.h"

static char *parser_planner_url(void)
{
    char formatted_time[64];
    time_t now = time(NULL);
    struct tm *utc_time = gmtime(&now);

    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%dT%H%%3A%M%%3A%S.000Z", utc_time);

    char *url = malloc(256);
    if (!url)
    {
        fprintf(stderr, "parser_planner_url: Failed to allocate memory\n");
        return NULL;
    }

    snprintf(url, 256, "%s?end_date=%s&order=desc", PLANNER_URL, formatted_time);
    return url;
}

static json_value *parser(char *URL)
{
    FILE *file = fopen("cookie.txt", "r");

    if (file == NULL)
    {
        fprintf(stderr, "parser: Session cookie not found, please login\n");
        return NULL;
    }
    else
    {
        char buffer[1024] = {0};
        size_t bytesRead = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            fclose(file);

            RequestOptions opt = {
                .cookies = buffer,
                .follow = false,
                .post_fields = NULL,
                .request_headers = NULL,
                .url = URL,
                .verbose = false};

            RequestResponse *res = request_get(opt);

            json_value *value = json_parse(res->response_body, strlen(res->response_body) + 1);

            request_options_cleanup(&opt);
            request_response_cleanup(res);

            return value;
        }
        else
        {
            fprintf(stderr, "parser: Error or empty cookie file, please login\n");
            fclose(file);
            return NULL;
        }
    }
}

void parser_unreadCount(void)
{
    json_value *value = parser(UNREAD_COUNT_URL);

    if (value != NULL)
    {
        printf("%s\n", value->u.object.values[0].value->u.string.ptr); // Unread conversation count
        json_value_free(value);
    }
}

void parser_courseInfo(const int id)
{
    char url[256];
    snprintf(url, sizeof(url), "%s%d", COURSE_URL, id);

    json_value *value = parser(url);

    if (value != NULL)
    {
        printf("%s\n", value->u.object.values[1].value->u.string.ptr); // Course name
        printf("%s\n", value->u.object.values[8].value->u.string.ptr); // Course name with course code
        json_value_free(value);
    }
}

void parser_dashboard(void)
{
    json_value *value = parser(DASHBOARD_URL);

    if (value != NULL)
    {
        size_t len = value->u.array.length;
        for (size_t i = 0; i < len; i++)
        {
            printf("%s\n", value->u.array.values[i]->u.object.values[0].value->u.string.ptr); // Course long name
        }
        json_value_free(value);
    }
}

void parser_planner(void)
{
    char *url = parser_planner_url();

    json_value *value = parser(url);

    if (value != NULL)
    {
        size_t len = value->u.array.length;
        for (size_t i = 0; i < len; i++)
        {
            printf("%s - %s\n", value->u.array.values[i]->u.object.values[10].value->u.string.ptr, value->u.array.values[i]->u.object.values[8].value->u.object.values[1].value->u.string.ptr); // Course name & Course announcement title
        }
        json_value_free(value);
    }

    free((void *)url);
}
