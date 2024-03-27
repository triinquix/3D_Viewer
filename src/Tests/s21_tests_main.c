#include "s21_tests.h"

int main(void) {
  char failed = 0;
  char num_of_tests = 0;

  Suite *s_1, *s_2, *s_3, *s_4, *s_5, *s_6, *s_7, *s_8, *s_9;
  SRunner *runner_1, *runner_2, *runner_3, *runner_4, *runner_5, *runner_6,
      *runner_7, *runner_8, *runner_9;

  s_1 = suite_scale_1();
  s_2 = suite_scale_2();
  s_3 = suite_scale_3();
  s_4 = suite_rotate_1();
  s_5 = suite_rotate_2();
  s_6 = suite_rotate_3();
  s_7 = suite_translate_1();
  s_8 = suite_translate_2();
  s_9 = suite_translate_3();

  // srunner_set_fork_status(runner_1, CK_NOFORK);

  runner_1 = srunner_create(s_1);
  printf("\033[1;32mRUNNING SCALE_1 TESTS\033[1;0m\n");
  srunner_run_all(runner_1, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_1);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_2 = srunner_create(s_2);
  printf("\033[1;32mRUNNING SCALE_2 TESTS\033[1;0m\n");
  srunner_run_all(runner_2, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_2);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_3 = srunner_create(s_3);
  printf("\033[1;32mRUNNING SCALE_3 TESTS\033[1;0m\n");
  srunner_run_all(runner_3, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_3);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_4 = srunner_create(s_4);
  printf("\033[1;32mRUNNING ROTATE_1 TESTS\033[1;0m\n");
  srunner_run_all(runner_4, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_4);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_5 = srunner_create(s_5);
  printf("\033[1;32mRUNNING ROTATE_2 TESTS\033[1;0m\n");
  srunner_run_all(runner_5, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_5);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_6 = srunner_create(s_6);
  printf("\033[1;32mRUNNING ROTATE_3 TESTS\033[1;0m\n");
  srunner_run_all(runner_6, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_6);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_7 = srunner_create(s_7);
  printf("\033[1;32mRUNNING TRANSLATE_1 TESTS\033[1;0m\n");
  srunner_run_all(runner_7, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_7);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_8 = srunner_create(s_8);
  printf("\033[1;32mRUNNING TRANSLATE_2 TESTS\033[1;0m\n");
  srunner_run_all(runner_8, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_8);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  runner_9 = srunner_create(s_9);
  printf("\033[1;32mRUNNING TRANSLATE_3 TESTS\033[1;0m\n");
  srunner_run_all(runner_9, CK_VERBOSE);
  num_of_tests += srunner_ntests_run(runner_9);
  printf("\033[1;33m[+] Total number of tests run -> %d\033[1;0m\n",
         num_of_tests);
  printf("\n");

  failed = srunner_ntests_failed(runner_1) + srunner_ntests_failed(runner_2) +
           srunner_ntests_failed(runner_3) + srunner_ntests_failed(runner_4) +
           srunner_ntests_failed(runner_5) + srunner_ntests_failed(runner_6) +
           srunner_ntests_failed(runner_7) + srunner_ntests_failed(runner_8) +
           srunner_ntests_failed(runner_9);

  srunner_free(runner_1);
  srunner_free(runner_2);
  srunner_free(runner_3);
  srunner_free(runner_4);
  srunner_free(runner_5);
  srunner_free(runner_6);
  srunner_free(runner_7);
  srunner_free(runner_8);
  srunner_free(runner_9);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}