#include "cli.h"
#include "utils.h"
#include <stdio.h>

void relf_cli_print_usage(const Relf_Cli_Args *args) {
  const char *argv0 = (args && args->argv0) ? args->argv0 : "relf";
  fprintf(stderr, "usage: %s <input>\n", argv0);
}

Relf_Cli_Status relf_cli_parse_args(Relf_Cli_Args *args, int argc,
                                    const char *argv[]) {
  if (!args || argc <= 0 || !argv) {
    return RELF_CLI_ERR;
  }

  // TODO: Complete CLI implementation
  args->argv0 = argv[0];
  args->input_path = argv[1];

  return RELF_CLI_OK;
}
