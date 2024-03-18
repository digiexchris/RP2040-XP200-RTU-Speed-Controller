#include <zephyr/ztest.h>
#include <stdint.h> // Include the necessary header file
static inline void _exit_qemu() {

register uint32_t r0 __asm__("r0"); // Use 'uint32_t' instead of 'u32_t'
  r0 = 0x18;
  register uint32_t r1 __asm__("r1");
  r1 = 0x20026;
  __asm__ volatile("bkpt #0xAB");
}

#ifdef CONFIG_ZTEST_SHUFFLE
#define NUM_ITER_PER_SUITE CONFIG_ZTEST_SHUFFLE_SUITE_REPEAT_COUNT
#define NUM_ITER_PER_TEST  CONFIG_ZTEST_SHUFFLE_TEST_REPEAT_COUNT
#else
#define NUM_ITER_PER_SUITE 1
#define NUM_ITER_PER_TEST  1
#endif

void test_main(void)
{
#if CONFIG_ZTEST_SHUFFLE
	ztest_run_all(NULL, true, NUM_ITER_PER_SUITE, NUM_ITER_PER_TEST);
#else
	ztest_run_all(NULL, false, NUM_ITER_PER_SUITE, NUM_ITER_PER_TEST);
#endif
	ztest_verify_all_test_suites_ran();

    _exit_qemu();
}