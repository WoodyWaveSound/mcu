#include <wws.h>

#include <stdio.h>

static void          routine_test_callback(wws_routine_t *r);
static wws_routine_t routine_test = {
  .callback = routine_test_callback,
};

void *begin WWS_ROM("db") = 0;

static wws_data_i8_t i81           = { .data = 0 };
char a               WWS_ROM("db") = 0;
char c               WWS_RAM("db") = 0;
static wws_data_i8_t i8f           = { .ref = &a, .select = { .value = { 1, 2, 3 }, .num = 3 } };

char                     b[128] WWS_ROM("db") = "asds";
static wws_data_string_t str           = { .ref = &b, .size = 128, .len = sizeof("asds") - 1 };
void *end                WWS_ROM("db") = 0;

int main(int argc, char const *argv[])
{
  wws_manifest_print(&wws_manifest);

  printf("hello world\r\n");

  for (int i = 0; i < i8f.select.num; i++) {
    printf("%s\r\n", (i8f.select.value)[i]);
  }

  if (wws_data_write(&i81, 123)) { printf("changed\r\n"); }

  if (wws_data_write_bulk(&str, "asdsada", 12)) { printf("changed\r\n"); }

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
