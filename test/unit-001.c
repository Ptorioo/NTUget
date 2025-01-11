#include "test.h"
#include "encoder.h"

int unitfail = 0;

int unit_setup()
{
    printf("Setting up the encoder tests...\n");
    return 0;
}

void unit_stop()
{
    printf("Cleaning up after the encoder tests...\n");
}

UNITTEST_START
printf("Running URL Encoding Test...\n");
{
    const char *data;
    char buffer[256];
    bool result;

    // Test 1
    data = "";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_url_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "URL encoding result #1 failed");
    fail_unless(strcmp(buffer, "") == 0, "URL encoding result #1 mismatch");

    // Test 2
    data = "!@#$%^&*()-=_+[]\\{}|;':\",./<>?`~";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_url_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "URL encoding result #2 failed");
    fail_unless(strcmp(buffer, "!%40%23%24%25%5E%26*()-%3D_%2B%5B%5D%5C%7B%7D%7C%3B'%3A%22%2C.%2F%3C%3E%3F%60~") == 0,
                "URL encoding result #2 mismatch");

    // Test 3
    data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_url_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "URL encoding result #3 failed");
    fail_unless(strcmp(buffer, data) == 0, "URL encoding result #3 mismatch");

    // Test 4
    data = "!*()-_'.~";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_url_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "URL encoding result #4 failed");
    fail_unless(strcmp(buffer, data) == 0, "URL encoding result #4 mismatch");
}

printf("Running Base64 Encoding Test...\n"); // Test data specified in RFC 4648
{
    const char *data;
    char buffer[256];
    bool result;

    // Test 1
    data = "";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #1 failed");
    fail_unless(strcmp(buffer, "") == 0, "Base64 encoding result #1 mismatch");

    // Test 2
    data = "f";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #2 failed");
    fail_unless(strcmp(buffer, "Zg==") == 0, "Base64 encoding result #2 mismatch");

    // Test 3
    data = "fo";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #3 failed");
    fail_unless(strcmp(buffer, "Zm8=") == 0, "Base64 encoding result #3 mismatch");

    // Test 4
    data = "foo";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #4 failed");
    fail_unless(strcmp(buffer, "Zm9v") == 0, "Base64 encoding result #4 mismatch");

    // Test 5
    data = "foob";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #5 failed");
    fail_unless(strcmp(buffer, "Zm9vYg==") == 0, "Base64 encoding result #5 mismatch");

    // Test 6
    data = "fooba";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #6 failed");
    fail_unless(strcmp(buffer, "Zm9vYmE=") == 0, "Base64 encoding result #6 mismatch");

    // Test 7
    data = "foobar";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_encode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 encoding result #7 failed");
    fail_unless(strcmp(buffer, "Zm9vYmFy") == 0, "Base64 encoding result #7 mismatch");
}

printf("Running Base64 Decoding Test...\n");
{
    const char *data;
    char buffer[256];
    bool result;

    // Test 1
    data = "";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #1 failed");
    fail_unless(strcmp(buffer, "") == 0, "Base64 decoding result #1 mismatch");

    // Test 2
    data = "Zg==";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #2 failed");
    fail_unless(strcmp(buffer, "f") == 0, "Base64 decoding result #2 mismatch");

    // Test 3
    data = "Zm8=";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #3 failed");
    fail_unless(strcmp(buffer, "fo") == 0, "Base64 decoding result #3 mismatch");

    // Test 4
    data = "Zm9v";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #4 failed");
    fail_unless(strcmp(buffer, "foo") == 0, "Base64 decoding result #4 mismatch");

    // Test 5
    data = "Zm9vYg==";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #5 failed");
    fail_unless(strcmp(buffer, "foob") == 0, "Base64 decoding result #5 mismatch");

    // Test 6
    data = "Zm9vYmE=";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #6 failed");
    fail_unless(strcmp(buffer, "fooba") == 0, "Base64 decoding result #6 mismatch");

    // Test 7
    data = "Zm9vYmFy";
    memset(buffer, 0, sizeof(buffer));
    result = encoder_base64_decode(data, buffer, sizeof(buffer));
    fail_unless(result, "Base64 decoding result #7 failed");
    fail_unless(strcmp(buffer, "foobar") == 0, "Base64 decoding result #7 mismatch");
}
UNITTEST_STOP
