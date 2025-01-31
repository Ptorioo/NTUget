#ifndef CONFIG_H
#define CONFIG_H

#define COOL_URL "https://cool.ntu.edu.tw/"
#define SAML_URL "https://cool.ntu.edu.tw/login/saml"
#define API_BASE_URL "https://cool.ntu.edu.tw/api/v1"

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#define SYSTEM_CA_PATH "/etc/ssl/certs/ca-certificates.crt"
#else
#define CA_FILE_NAME "ca-bundle.crt"
#endif

#endif