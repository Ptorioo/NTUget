#include "request.h"

static RequestResponse *request_response_init(void)
{
    RequestResponse *res = malloc(sizeof(RequestResponse));

    res->status_code = 0;
    res->response_headers = NULL;
    res->response_body = malloc(1);
    res->response_body[0] = '\0';
    res->cookies = NULL;
    res->redirect_url = NULL;

    return res;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    char **response_body = (char **)userp;

    *response_body = realloc(*response_body, strlen(*response_body) + total_size + 1);
    if (*response_body == NULL)
        return 0;

    strncat(*response_body, contents, total_size);
    return total_size;
}

static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t total_size = size * nitems;
    struct curl_slist **response_headers = (struct curl_slist **)userdata;

    *response_headers = curl_slist_append(*response_headers, buffer);

    return total_size;
}

inline static char *get_certificate_path() {
#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    struct stat buffer;
    if (stat(SYSTEM_CA_PATH, &buffer) == 0) {
        return strdup(SYSTEM_CA_PATH);
    }
    return NULL;
#else
    const char *exePath = executable_directory();
    size_t len = strlen(exePath) + strlen(CA_FILE_NAME) + 1;
    char *crt_dir = malloc(len * sizeof(char));
    if (!crt_dir) {
        free((void *)exePath);
        return NULL;
    }
    snprintf(crt_dir, len, "%s%s", exePath, CA_FILE_NAME);
    free((void *)exePath);
    return crt_dir;
#endif
}

RequestResponse *request_get(RequestOptions opt)
{
    CURL *curl = curl_easy_init();
    RequestResponse *response = request_response_init();

    const char *crt_dir = get_certificate_path();

    if (curl)
    {
        if (opt.verbose)
            printf("[GET] Initialized...\n");

        curl_easy_setopt(curl, CURLOPT_URL, opt.url);

        if (opt.follow)
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        if (opt.request_headers && *opt.request_headers)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, *opt.request_headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response->response_body);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response->response_headers);
        curl_easy_setopt(curl, CURLOPT_CAINFO, crt_dir);
        curl_easy_setopt(curl, CURLOPT_CAPATH, crt_dir);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        if (opt.cookies)
        {
            char *cookies_copy = strdup(opt.cookies);
            curl_easy_setopt(curl, CURLOPT_COOKIELIST, cookies_copy);
            free(cookies_copy);
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "[GET] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status_code);
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &response->cookies);

        if (opt.follow)
        {
            char *effective_url = NULL;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
            if (effective_url)
                response->redirect_url = strdup(effective_url);
            else
                printf("[GET] Redirect URL fetch failed\n");
            if (opt.verbose)
                printf("[GET] %s redirected to %s with status code: %ld\n", opt.url, response->redirect_url, response->status_code);
        }
        else
        {
            if (opt.verbose)
                printf("[GET] %s responded with status code: %ld\n", opt.url, response->status_code);
        }
    }
    else
        printf("[GET] Failed to initialize...\n");

    if (opt.verbose)
        printf("****************************************\n");

    free((void *)crt_dir);
    curl_easy_cleanup(curl);

    return response;
}

RequestResponse *request_post(RequestOptions opt)
{
    CURL *curl = curl_easy_init();
    RequestResponse *response = request_response_init();

    const char *crt_dir = get_certificate_path();

    if (curl)
    {
        if (opt.verbose)
            printf("[POST] Initialized...\n");

        curl_easy_setopt(curl, CURLOPT_URL, opt.url);

        if (opt.follow)
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        if (opt.request_headers && *opt.request_headers)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, *opt.request_headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response->response_body);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response->response_headers);
        curl_easy_setopt(curl, CURLOPT_CAINFO, crt_dir);
        curl_easy_setopt(curl, CURLOPT_CAPATH, crt_dir);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (opt.post_fields)
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opt.post_fields);

        if (opt.cookies)
        {
            char *cookies_copy = strdup(opt.cookies);
            curl_easy_setopt(curl, CURLOPT_COOKIELIST, cookies_copy);
            free(cookies_copy);
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "[POST] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status_code);
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &response->cookies);

        if (opt.follow)
        {
            char *effective_url = NULL;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
            if (effective_url)
                response->redirect_url = strdup(effective_url);
            else
                printf("[POST] Redirect URL fetch failed\n");
            if (opt.verbose)
                printf("[POST] %s redirected to %s with status code: %ld\n", opt.url, response->redirect_url, response->status_code);
        }
        else
        {
            if (opt.verbose)
                printf("[POST] %s responded with status code: %ld\n", opt.url, response->status_code);
        }
    }
    else
        printf("[POST] Failed to initialize...\n");

    if (opt.verbose)
        printf("****************************************\n");

    free((void *)crt_dir);
    curl_easy_cleanup(curl);

    return response;
}

void request_response_cleanup(RequestResponse *res)
{
    if (res->response_headers)
        curl_slist_free_all(res->response_headers);
    if (res->cookies)
        curl_slist_free_all(res->cookies);
    if (res->response_body)
        free((void *)res->response_body);
    if (res->redirect_url)
        free((void *)res->redirect_url);

    res->status_code = 0;
    res->response_headers = NULL;
    res->response_body = NULL;
    res->cookies = NULL;
    res->redirect_url = NULL;

    free((void *)res);
}

void request_options_cleanup(RequestOptions *opt)
{
    if (opt->request_headers)
        curl_slist_free_all(*opt->request_headers);
}
