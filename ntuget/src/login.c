#include "login.h"

static char *login_payload(const char *username, const char *password)
{
    const char *template = "__VIEWSTATE=/wEPDwUKMTY2MTc3NjUzM2RkUK4S8IU/lZeKUDrQIAtt4tRhRV4ZOkEMNdoJavm/SBs=&"
                           "__VIEWSTATEGENERATOR=0EE29E36&"
                           "__EVENTVALIDATION=/wEdAAUdVdOEjcCKz7S6sLphMAmFlt/S8mKmQpmuxn2LW6B9thvLC/FQOf5u4GfePSXQdrRBPkcB0cPQF9vyGTuIFWmijKZWG4rH59f66Vc64WGnN/Hmf00Q2eMalQURbQ6cPb45rGUVCHnIwpyxWjkkPDce&"
                           "__db=15&"
                           "ctl00$ContentPlaceHolder1$UsernameTextBox=%s&"
                           "ctl00$ContentPlaceHolder1$PasswordTextBox=%s&"
                           "ctl00$ContentPlaceHolder1$SubmitButton=Sign+In";

    size_t payload_size = strlen(template) + strlen(username) + strlen(password) + 1;

    char *payload = malloc(payload_size);
    snprintf(payload, payload_size, template, username, password);

    return payload;
}

static ParseResult login_parse_SAMLResponse(const char *response_body)
{
    const char *key = "name=\"SAMLResponse\" value=\"";
    const char *start = strstr(response_body, key);
    if (!start)
    {
        fprintf(stderr, "parse_SAML_response: Failed to locate the start of SAMLResponse from response body.\n");
        return (ParseResult){.status = 1, .is_error = 1};
    }
    start += strlen(key);

    const char *end = strchr(start, '\"');
    if (!end)
    {
        fprintf(stderr, "parse_SAML_response: Failed to locate the end of SAMLResponse from response body.\n");
        return (ParseResult){.status = 2, .is_error = 1};
    }

    size_t value_length = end - start;
    char *value = (char *)malloc(value_length + 1);

    strncpy(value, start, value_length);
    value[value_length] = '\0';

    size_t buffer_size = value_length * 3 + 1;
    char *buffer = (char *)malloc(buffer_size);

    if (!encoder_url_encode(value, buffer, buffer_size))
    {
        fprintf(stderr, "parse_SAML_response: Failed to URL encode the SAMLResponse value.\n");
        free(value);
        free(buffer);
        return (ParseResult){.status = 2, .is_error = 1};
    }

    size_t buffer2_size = strlen("SAMLResponse=") + strlen(buffer) + 1;
    char *buffer2 = (char *)malloc(buffer2_size);

    if (!snprintf(buffer2, buffer2_size, "%s%s", "SAMLResponse=", buffer))
    {
        fprintf(stderr, "parse_SAML_response: Failed to concatenate SAMLResponse string.\n");
        free(value);
        free(buffer);
        free(buffer2);
        return (ParseResult){.status = 2, .is_error = 1};
    }

    free(value);
    free(buffer);
    return (ParseResult){.result = buffer2, .is_error = 0};
}

static char *login_session_cookie(struct curl_slist *cookies)
{
    struct curl_slist *current = cookies;
    while (current)
    {
        if (strstr(current->data, "_normandy_session") != NULL && strstr(current->data, "_legacy_normandy_session") == NULL)
        {
            char *_normandy_session = current->data;
            return _normandy_session;
        }
        else
        {
            current = current->next;
        }
    }
    fprintf(stderr, "session_cookie: Failed to locate session cookie.\n");
    return NULL;
}

static inline void login_log(const char *message, bool verbose)
{
    if (verbose)
        printf("%s\n", message);
}

static inline LoginStatus login_handle_response(RequestResponse *response, RequestOptions *options, int expected_status, LoginStatus error_status)
{
    if (response->status_code == 403)
    {
        request_options_cleanup(options);
        request_response_cleanup(response);
        return LOGIN_FORBIDDEN;
    }
    else if (response->status_code != expected_status)
    {
        request_options_cleanup(options);
        request_response_cleanup(response);
        return error_status;
    }
    return LOGIN_SUCCESS;
}

static LoginStatus login_sso(const char *username, const char *pwd, bool verbose)
{
    LoginStatus status = 0;

    RequestOptions opt = {
        .cookies = NULL,
        .follow = true,
        .post_fields = NULL,
        .request_headers = NULL,
        .url = SAML_URL,
        .verbose = verbose};

    login_log("[LOGIN] Requesting login page...", verbose);

    RequestResponse *res1 = request_get(opt);

    status = login_handle_response(res1, &opt, 200, LOGIN_UNAVAILABLE);
    if (status != LOGIN_SUCCESS)
        return status;

    char *payload1 = login_payload(username, pwd);

    opt = (RequestOptions){
        .cookies = NULL,
        .follow = true,
        .post_fields = payload1,
        .request_headers = NULL,
        .url = res1->redirect_url,
        .verbose = verbose};

    login_log("[LOGIN] Requesting login session...", verbose);

    RequestResponse *res2 = request_post(opt);
    free((void *)payload1);
    request_response_cleanup(res1);

    status = login_handle_response(res2, &opt, 200, LOGIN_UNAVAILABLE);
    if (status != LOGIN_SUCCESS)
        return status;

    ParseResult payload2 = login_parse_SAMLResponse(res2->response_body);

    if (payload2.is_error)
    {
        request_options_cleanup(&opt);
        request_response_cleanup(res2);

        if (payload2.status == 1)
            return LOGIN_AUTH_ERROR;
        else
            return LOGIN_SERVICE_ERROR;
    }

    opt = (RequestOptions){
        .cookies = NULL,
        .follow = true,
        .post_fields = payload2.result,
        .request_headers = NULL,
        .url = SAML_URL,
        .verbose = verbose};

    login_log("[LOGIN] Requesting session cookie...", verbose);

    RequestResponse *res3 = request_post(opt);
    free((void *)payload2.result);
    request_response_cleanup(res2);

    status = login_handle_response(res3, &opt, 200, LOGIN_UNAVAILABLE);
    if (status != LOGIN_SUCCESS)
        return status;

    char *session_cookie = login_session_cookie(res3->cookies);

    if (session_cookie == NULL)
    {
        request_options_cleanup(&opt);
        request_response_cleanup(res3);
        return LOGIN_SERVICE_ERROR;
    }

    opt = (RequestOptions){
        .cookies = session_cookie,
        .follow = false,
        .post_fields = NULL,
        .request_headers = NULL,
        .url = COOL_URL,
        .verbose = verbose};

    login_log("[LOGIN] Requesting with acquired session cookie...", verbose);

    RequestResponse *res4 = request_get(opt);

    if (res4->status_code == 302)
    {
        request_options_cleanup(&opt);
        request_response_cleanup(res4);
        return LOGIN_TOKEN_INVALID;
    }

    status = login_handle_response(res4, &opt, 200, LOGIN_UNAVAILABLE);
    if (status != LOGIN_SUCCESS)
        return status;

    FILE *file = fopen("cookie.txt", "w");
    if (file == NULL)
        fprintf(stderr, "login_sso: Failed to open or create cookie file\n");
    else if (fprintf(file, "%s\n", session_cookie) < 0)
        fprintf(stderr, "login_sso: Failed to write session cookie to file\n");

    login_log("[LOGIN] Session cookie successfully saved to file", verbose);

    fclose(file);

    request_options_cleanup(&opt);
    request_response_cleanup(res3);
    request_response_cleanup(res4);

    return LOGIN_SUCCESS;
}

LoginStatus login(const char *username, const char *pwd, bool verbose)
{
    FILE *file = fopen("cookie.txt", "r");

    if (file == NULL)
    {
        login_log("[LOGIN] Session cookie file not found, performing SSO...", verbose);
        return login_sso(username, pwd, verbose);
    }
    else
    {
        char buffer[1024] = {0};
        size_t bytesRead = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            fclose(file);
        }
        else
        {
            fprintf(stderr, "login: Error or empty cookie file\n");
            fclose(file);
            return login_sso(username, pwd, verbose);
        }

        login_log("[LOGIN] Session cookie file found...", verbose);

        RequestOptions opt = {
            .cookies = buffer,
            .follow = false,
            .post_fields = NULL,
            .request_headers = NULL,
            .url = COOL_URL,
            .verbose = verbose};

        RequestResponse *res = request_get(opt);
        long status_code = res->status_code;

        request_response_cleanup(res);
        request_options_cleanup(&opt);

        if (status_code != 200)
        {
            login_log("[LOGIN] Session cookie file invalid, performing SSO...", verbose);
            return login_sso(username, pwd, verbose);
        }

        return LOGIN_SUCCESS;
    }
}
