#include "api_parser.h"

void parser_unreadCount()
{
    FILE *file = fopen("cookie.txt", "r");

    if (file == NULL)
    {
        fprintf(stderr, "parser_unreadCount: Session cookie not found, please login\n");
        // return NULL;
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
                .url = UNREAD_COUNT_URL,
                .verbose = false};

            RequestResponse *res = request_get(opt);

            json_value *value = json_parse(res->response_body, strlen(res->response_body) + 1);

            printf("%s\n", value->u.object.values[0].value->u.string.ptr);

            json_value_free(value);

            request_options_cleanup(&opt);
            request_response_cleanup(res);
        }
        else
        {
            fprintf(stderr, "parser_unreadCount: Error or empty cookie file, please login\n");
            fclose(file);
            // return NULL;
        }
    }
}
