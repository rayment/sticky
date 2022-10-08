/*
 * test_common.h
 * Common test functions/macros.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 08/10/2021
 */

#ifndef FR_RAYMENT_TEST_COMMON_H
#define FR_RAYMENT_TEST_COMMON_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sticky.h>

Smutex print_mutex;
int clockiter;
clock_t tic, toc;
double tmptime, totaltime, besttime, worsttime, avgtime;

#define RED    "\x1b[1;31m"
#define YELLOW "\x1b[1;33m"
#define GREEN  "\x1b[1;32m"
#define BOLD   "\x1b[1m"
#define RESET  "\x1b[0m"

#define INIT()                                               \
	(void) clockiter; (void) tic; (void) toc;                \
	(void) tmptime; (void) totaltime; (void) besttime;       \
	(void) worsttime; (void) avgtime; (void) print_mutex;    \
	S_sticky_init();                                         \
	print_mutex = S_mutex_new();                             \
	S_debug("Seed set to: %d\n", S_random_get_seed())

#define FREE()                                               \
	S_mutex_delete(print_mutex);                             \
	S_sticky_free()

#define ATOMIC_PRINT(...)                                    \
	S_mutex_lock(print_mutex);                               \
	fprintf(stdout, __VA_ARGS__);                            \
	fflush(stdout);                                          \
	S_mutex_unlock(print_mutex)

#define FAIL(x,y) if (!(x) || SERRNO != S_NO_ERROR) {                        \
                      ATOMIC_PRINT("\r" RED "FAILURE : " RESET "%s\n", y);   \
                      if (SERRNO != S_NO_ERROR) {                            \
                          ATOMIC_PRINT("\r" RED "SERRNO  : %s\n" RESET,      \
                                       S_error_string());                    \
                      }                                                      \
                      exit(EXIT_FAILURE);                                    \
                  } else {                                                   \
                      ATOMIC_PRINT("\r" GREEN "SUCCESS : " RESET "%s\n", y); \
                  }
#define TEST(x,y,z) ATOMIC_PRINT("TEST    : %s", z); x FAIL(y,z)

#define FAIL_RAW(x,y) if (!(x) || SERRNO != S_NO_ERROR) {                    \
                      printf("\r" RED "FAILURE : " RESET "%s\n", y);         \
                      if (SERRNO != S_NO_ERROR) {                            \
                          printf("\r" RED "SERRNO  : %s\n" RESET,            \
                                 S_error_string());                          \
                      }                                                      \
                      exit(EXIT_FAILURE);                                    \
                  } else {                                                   \
                      printf("\r" GREEN "SUCCESS : " RESET "%s\n", y);       \
                  }
#define TEST_RAW(x,y,z) printf("TEST: %s", z); x FAIL_RAW(y,z)

#define TIME(x,y,z) clockiter = 0;                                           \
                    totaltime = 0; besttime = 0; worsttime = 0, avgtime = 0; \
                    while (clockiter++ < z) {                                \
                        tic = clock(); x toc = clock();                      \
                        tmptime = (double) (toc - tic) / CLOCKS_PER_SEC;     \
                        avgtime += tmptime;                                  \
                        if (clockiter == 1 || tmptime < besttime)            \
                            besttime = tmptime;                              \
                        if (clockiter == 1 || tmptime > worsttime)           \
                            worsttime = tmptime;                             \
                    }                                                        \
                    totaltime = avgtime; avgtime /= z;                       \
                    fprintf(stdout, YELLOW "BENCHMARK " RESET "(%s):\n"      \
                            "          total: " BOLD "%fs" RESET             \
                            ", iters: %d\n"                                  \
                            "          avg: " BOLD "%fs" RESET               \
                            ", best: " BOLD "%fs" RESET                      \
							", worst: " BOLD "%fs" RESET "\n",               \
							y, totaltime, z, avgtime, besttime, worsttime);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_TEST_COMMON_H */

