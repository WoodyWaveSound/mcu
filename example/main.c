#include <wws.h>

#include <stdio.h>

static void          routine_test_callback(wws_routine_t *r);
static wws_routine_t routine_test = {
  .callback = routine_test_callback,
};

int main(int argc, char const *argv[])
{
  wws_manifest_print(&wws_manifest);

  printf("hello world\r\n");

  wws_event(NULL, NULL, &routine_test);

  wws_routine_start(&routine_test);

  for (;;) {
    wws_routine_run(&routine_test);
  }

  return 0;
}

static void routine_test_callback(wws_routine_t *r)
{
  WWS_COROUTINE_RESUME(r);

  printf("step1\r\n");

  WWS_COROUTINE_YIELD(r);

  printf("step2, before sleep\r\n");

  WWS_COROUTINE_SLEEP(r, WWS_MS(100));

  printf("step3, after sleep\r\n");

  WWS_COROUTINE_RESTART(r);
}
