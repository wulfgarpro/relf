#include "cli.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>

static const struct option options[] = {
    {"help", no_argument, 0, 'H'},
    {"file-header", no_argument, 0, 'h'},
    {"program-headers", no_argument, 0, 'l'},
    {"segments", no_argument, 0, 'l'},
    {"section-headers", no_argument, 0, 'S'},
    {"sections", no_argument, 0, 'S'},
    {0, 0, 0, 0}};

void relf_cli_print_usage(const Relf_Cli_Args *args) {
  const char *argv0 = (args && args->argv0) ? args->argv0 : "relf";
  fprintf(stderr, "usage: %s [OPTION]... <input>\n", argv0);
  fprintf(stderr, "  -h, --file-header\n");
  fprintf(stderr, "  -l, --program-headers, --segments\n");
  fprintf(stderr, "  -S, --section-headers, --sections\n");
  fprintf(stderr, "  -H, --help\n");
}

Relf_Cli_Status relf_cli_parse_args(Relf_Cli_Args *args, int argc,
                                    char *argv[]) {
  if (!args || !argv) {
    return RELF_CLI_ERR;
  }

  args->flags = RELF_CLI_PRINT_NONE;
  args->input_path = NULL;

  args->argv0 = argv[0];

  opterr = 0; // Ignore `getopt` errors
  optind = 1;

  int c = 0;
  while ((c = getopt_long(argc, argv, "HhlS", options, NULL)) != -1) {
    switch (c) {
    case 'h': {
      args->flags |= RELF_CLI_PRINT_EHDR;
      break;
    }
    case 'l': {
      args->flags |= RELF_CLI_PRINT_PHDR;
      break;
    }
    case 'S': {
      args->flags |= RELF_CLI_PRINT_SHDR;
      break;
    }
    case 'H': {
      args->flags |= RELF_CLI_PRINT_HELP;
      break;
    }
    case '?':
    default: {
      return RELF_CLI_ERR;
    }
    }
  }

  if (args->flags & RELF_CLI_PRINT_HELP) {
    return RELF_CLI_OK;
  }

  if (args->flags & RELF_CLI_PRINT_NONE) {
    return RELF_CLI_ERR;
  }

  // No positional argument supplied for non-help options
  if (optind >= argc) {
    return RELF_CLI_ERR;
  }

  // More than one positional argument supplied
  if (optind + 1 < argc) {
    return RELF_CLI_ERR;
  }

  args->input_path = argv[optind];

  return RELF_CLI_OK;
}
