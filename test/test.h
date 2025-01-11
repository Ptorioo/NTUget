/* Unit test configuration similar to libcurl. */
#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>

extern int unitfail;

int test(char *arg);

char *hexdump(const unsigned char *data, size_t len);

/* The fail macros mark the current test step as failed, and continue */
#define fail_if(expr, msg)                                           \
    do                                                               \
    {                                                                \
        if (expr)                                                    \
        {                                                            \
            fprintf(stderr, "%s:%d FAILED Assertion '%s' met: %s\n", \
                    __FILE__, __LINE__, #expr, msg);                 \
            unitfail++;                                              \
        }                                                            \
    } while (0)

#define fail_unless(expr, msg)                                   \
    do                                                           \
    {                                                            \
        if (!(expr))                                             \
        {                                                        \
            fprintf(stderr, "%s:%d Assertion '%s' FAILED: %s\n", \
                    __FILE__, __LINE__, #expr, msg);             \
            unitfail++;                                          \
        }                                                        \
    } while (0)

#define verify_memory(dynamic, check, len)                                      \
    do                                                                          \
    {                                                                           \
        if (dynamic && memcmp(dynamic, check, len))                             \
        {                                                                       \
            char *check_dump = hexdump((const unsigned char *)check, len);      \
            char *dynamic_dump = hexdump((const unsigned char *)dynamic, len);  \
            if (check_dump && dynamic_dump)                                     \
            {                                                                   \
                fprintf(stderr, "%s:%d Memory buffer FAILED match size %d. "    \
                                "'%s' is not\n",                                \
                        __FILE__, __LINE__, len, check_dump);                   \
                fprintf(stderr, "%s:%d the same as '%s'\n", __FILE__, __LINE__, \
                        dynamic_dump);                                          \
            }                                                                   \
            free(check_dump);                                                   \
            free(dynamic_dump);                                                 \
            unitfail++;                                                         \
        }                                                                       \
    } while (0)

/* fail() is for when the test case figured out by itself that a check
   proved a failure */
#define fail(msg)                                    \
    do                                               \
    {                                                \
        fprintf(stderr, "%s:%d test FAILED: '%s'\n", \
                __FILE__, __LINE__, msg);            \
        unitfail++;                                  \
    } while (0)

/* The abort macros mark the current test step as failed, and exit the test */
#define abort_if(expr, msg)                                         \
    do                                                              \
    {                                                               \
        if (expr)                                                   \
        {                                                           \
            fprintf(stderr, "%s:%d ABORT assertion '%s' met: %s\n", \
                    __FILE__, __LINE__, #expr, msg);                \
            unitfail++;                                             \
            goto unit_test_abort;                                   \
        }                                                           \
    } while (0)

#define abort_unless(expr, msg)                                        \
    do                                                                 \
    {                                                                  \
        if (!(expr))                                                   \
        {                                                              \
            fprintf(stderr, "%s:%d ABORT assertion '%s' failed: %s\n", \
                    __FILE__, __LINE__, #expr, msg);                   \
            unitfail++;                                                \
            goto unit_test_abort;                                      \
        }                                                              \
    } while (0)

#define unittest_abort(msg)                           \
    do                                                \
    {                                                 \
        fprintf(stderr, "%s:%d test ABORTED: '%s'\n", \
                __FILE__, __LINE__, msg);             \
        unitfail++;                                   \
        goto unit_test_abort;                         \
    } while (0)

#define UNITTEST_START                    \
    int test(char *arg)                   \
    {                                     \
        (void)arg;                        \
        if (unit_setup())                 \
        {                                 \
            fail("unit_setup() FAILURE"); \
        }                                 \
        else                              \
        {

#define UNITTEST_STOP                         \
    goto unit_test_abort; /* avoid warning */ \
    unit_test_abort:                          \
    unit_stop();                              \
    }                                         \
    return (int)unitfail;                     \
    }

#endif
