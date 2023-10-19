#include <wws.h>

#include <stdio.h>

static void debug(const wws_debug_t *dbg)
{
  if (dbg->type == WWS_DEBUG_MESSAGE) {
    if (dbg->event) { printf("[%s] %s", dbg->event, (const char *) dbg->data); }
    else {
      puts((const char *) dbg->data);
    }
  }
  else {
    printf("EVT [%s][%s]\r\n", dbg->component, dbg->event);
  }
}

// static unsigned int get(void *inst, char *buf)
// {
//   *buf = (char) getchar();
//   return 1;
// }

// static unsigned int put(void *inst, char byte)
// {
//   printf("%c", byte);

//   return 1;
// }

// static wws_byte_t io = { .interface = &(wws_byte_inf_t){ .get = get, .put = put } };

// static wws_cli_err_t cmd_callback(
//   wws_cli_cmd_type_t type, const char *ptr, unsigned int len, wws_cli_cmd_t *cmd, wws_cli_t *cli)
// {
//   // wws_infoln("<%5s> [%5s] (%02d) %s", wws_cli_cmd_type_str[type], cmd->cmd.str, len, ptr);

//   return WWS_CLI_ERR_OK;
// }

// static wws_cli_cmd_t scan = { .cmd = wws_new_cstr("scan"), .callback = cmd_callback };
// static wws_cli_cmd_t i2ca = { .cmd      = wws_new_cstr("-a"),
//                               .arg_num  = 1,
//                               .flag     = WWS_CLI_CMD_OPTION_BATCH,
//                               .callback = cmd_callback };
// static wws_cli_cmd_t i2cw = { .cmd      = wws_new_cstr("-w"),
//                               .arg_num  = 1,
//                               .flag     = WWS_CLI_CMD_OPTION_BATCH,
//                               .callback = cmd_callback };
// static wws_cli_cmd_t i2cr = { .cmd      = wws_new_cstr("-r"),
//                               .arg_num  = 1,
//                               .flag     = WWS_CLI_CMD_OPTION_BATCH,
//                               .callback = cmd_callback };
// static wws_cli_cmd_t i2c  = {
//    .cmd      = wws_new_cstr("i2c"),
//    .callback = cmd_callback,
//    .arg_num  = 1,
//    .children = (wws_cli_cmd_t *[]){ &scan, &i2ca, &i2cw, &i2cr, 0 },
// };

// static wws_cli_t cli = { .io = &io, .root = { .children = (wws_cli_cmd_t *[]){ &i2c, 0 } } };

static void cb(const char *p, wws_service_t *s) {}

static wws_service_t t1[] = {
  { .callback = cb, .name = "1", .id = 1 }, { .callback = cb, .name = "2", .id = 2 },
  { .callback = cb, .name = "3", .id = 3 }, { .callback = cb, .name = "4", .id = 4 },
  { .callback = cb, .name = "5", .id = 5 }, { .callback = cb, .name = "6", .id = 6 },
  { .callback = cb, .name = "7", .id = 7 }, { .callback = cb, .name = "8", .id = 8 },
  { .callback = cb, .name = "9", .id = 9 }, { .callback = 0 },
};

int main(int argc, char const *argv[])
{
  wws_debug_set_callback(debug);
  wws_manifest_print(&wws_manifest);
  wws_service_init(t1);

  // wws_infoln("%p", &t1);
  // wws_infoln("%p", &t1[0]);
  // wws_infoln("%p", wws_services[0]);
  // wws_infoln("%p", *wws_services);

  wws_infoln("name = %s, id = %d", wws_service_by_index(0)->name, wws_service_by_index(0)->id);
  for (wws_service_t *s = &wws_services[0]; s->callback != 0; s++) {
    wws_infoln("name = %s, id = %d", s->name, s->id);
  }

  for (;;) {
    // wws_cli_parse(&cli);
  }


  return 0;
}
