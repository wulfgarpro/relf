#pragma once

typedef enum { RELF_CLI_OK, RELF_CLI_ERR, RELF_CLI_HELP } Relf_Cli_Status;

typedef enum {
  RELF_CLI_PRINT_EHDR = 1u << 0,
  RELF_CLI_PRINT_PHDR = 1u << 1,
  RELF_CLI_PRINT_SHDR = 1u << 2,
} Relf_Cli_PrintFlags;

typedef struct {
  const char *argv0;
  const char *input_path;
  Relf_Cli_PrintFlags flags;
} Relf_Cli_Args;

void relf_cli_print_usage(const Relf_Cli_Args *args);
Relf_Cli_Status relf_cli_parse_args(Relf_Cli_Args *args, int argc,
                                    const char *argv[]);
