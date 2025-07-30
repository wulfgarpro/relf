#include "relf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const char *program_name = argv[0];

  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <input>\n", program_name);
    fprintf(stderr, "ERROR: no input provided\n");
    return EXIT_FAILURE;
  }

  FILE *elf_file = fopen(program_name, "rb");
  if (!elf_file) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  Relf_Elf64_Ehdr ehdr;
  size_t n_bytes_read = fread(&ehdr, 1, sizeof(ehdr), elf_file);
  if (n_bytes_read != sizeof(ehdr)) {
    fprintf(stderr, "Error reading ELF header\n");
    fclose(elf_file);
    return EXIT_FAILURE;
  }

  relf_print_elf64_header(&ehdr);

  fclose(elf_file);

  return EXIT_SUCCESS;
}
