/*
 * Simple tests for the functions implemented in src/tox.c
 *
 * just a main function running some stuff, no test framework used (yet).
 */

#include "../src/main.h"
#include "../src/tox.h"


/* below here are copied from xlib/main.c TODO refactor */

#include <unistd.h>

void thread(void func(void *), void *args) {
    pthread_t      thread_temp;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1 << 20);
    pthread_create(&thread_temp, &attr, (void *(*)(void *))func, args);
    pthread_attr_destroy(&attr);
}

void yieldcpu(uint32_t ms) {
    usleep(1000 * ms);
}
/* above here are copied from xlib/main.c TODO refactor */

#define TOX_TEST(TESTCODE) \
    thread(toxcore_thread, NULL); \
    TESTCODE \
/*    postmessage_utoxav(UTOXAV_KILL, 0, 0, NULL);*/ \
    postmessage_toxcore(TOX_KILL, 0, 0, NULL); \
    int i = 0; while (tox_thread_init) { yieldcpu(100); }

#define TEST(TESTFUN, NAME) \
    printf("Running Test: %s\n", NAME); \
    if (TESTFUN) { printf("SUCCESSFUL.\n"); } else { printf("FAILED!\n"); error = 1; }


bool test_settings(bool enable_ipv6, bool enable_udp, bool use_proxy, bool force_proxy, bool expect_fail);


int main(int argc, char *argv[]) {

    int error = 0;

    TEST(test_settings(0, 0, 0, 0, 0), "Init tox with IPv6=0, UDP=0, Proxy=None, should succeed");
    TEST(test_settings(0, 0, 1, 1, 1), "Init tox with IPv6=0, UDP=0, Proxy=Yes+force, should fail");

    return error;
}

bool test_settings(bool enable_ipv6, bool enable_udp, bool use_proxy, bool force_proxy, bool expect_fail)
{
    settings.enable_ipv6 = enable_ipv6;
    settings.enable_udp = enable_udp;
    settings.use_proxy = use_proxy;
    settings.force_proxy = force_proxy;
    settings.proxy_port = 61234; // there should be nothing running on that port
    // TODO where is proxy host?

    TOX_TEST(

        while(!tox_thread_init) {
            yieldcpu(20);
            return 1;
        }

    )
    return 0;
}
