#include "test.h"
#include "utils.h"

int unitfail = 0;

int unit_setup()
{
    printf("Setting up the utils tests...\n");
    return 0;
}

void unit_stop()
{
    printf("Cleaning up after the utils tests...\n");
}

UNITTEST_START
printf("Running Utils Test...\n");
{
    char buffer[100];
    bool result;

    // Test 1
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, sizeof(buffer), 3, "Hello, ", "world", "!");
    fail_unless(result, "Utils concat result #1 failed");
    fail_unless(strcmp(buffer, "Hello, world!") == 0, "Utils concat result #1 mismatch");

    // Test 2
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, 10, 3, "Hello, ", "world", "!");
    fail_unless(!result, "Utils concat result #2 failed");

    // Test 3
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, sizeof(buffer), 2, "Hello", "");
    fail_unless(result, "Utils concat result #3 failed");
    fail_unless(strcmp(buffer, "Hello") == 0, "Utils concat result #2 mismatch");

    // Test 4
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, sizeof(buffer), 1, "SingleString");
    fail_unless(result, "Utils concat result #4 failed");
    fail_unless(strcmp(buffer, "SingleString") == 0, "Utils concat result #4 mismatch");

    // Test 5
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, sizeof(buffer), 4, "This ", "is ", "a ", "test ");
    fail_unless(result, "Utils concat result #5 failed");
    fail_unless(strcmp(buffer, "This is a test ") == 0, "Utils concat result #5 mismatch");

    // Test 6
    memset(buffer, 0, sizeof(buffer));
    result = concat(buffer, 10, 4, "This", " is", " too", " long");
    fail_unless(!result, "Utils concat result #6 failed");
}
UNITTEST_STOP
