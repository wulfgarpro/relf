#include "relf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const char *program_name = argv[1];

  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <input>\n", program_name);
    return EXIT_FAILURE;
  }

  FILE *elf_file = fopen(program_name, "rb");
  if (!elf_file) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  Relf_E_Ident e_ident;
  size_t n_bytes_read = fread(&e_ident, 1, sizeof(e_ident), elf_file);
  if (n_bytes_read != sizeof(e_ident)) {
    fprintf(stderr, "ERROR: reading ELF ident\n");
    fclose(elf_file);
    return EXIT_FAILURE;
  }

  if (!relf_is_valid_elf_magic(&e_ident)) {
    fprintf(stderr, "ERROR: invalid ELF file\n");
    fclose(elf_file);
    return EXIT_FAILURE;
  }

  Relf_Elf_Ehdr ehdr;
  ehdr.relf_e_class = relf_determine_elf_class(&e_ident);
  ehdr.relf_e_data = relf_determine_elf_data(&e_ident);

  rewind(elf_file);

  switch (ehdr.relf_e_class) {
  case RELF_E_CLASS_32:
    // n_bytes_read =
    //     fread(&ehdr.ehdr.elf32_ehdr, 1, sizeof(ehdr.ehdr.elf32_ehdr),
    //     elf_file);
    // if (n_bytes_read != sizeof(ehdr)) {
    //   fprintf(stderr, "ERROR: reading 32-bit ELF header\n");
    //   fclose(elf_file);
    //   return EXIT_FAILURE;
    // }
    break;
  case RELF_E_CLASS_64:
    n_bytes_read =
        fread(&ehdr.ehdr.elf64_ehdr, 1, sizeof(ehdr.ehdr.elf64_ehdr), elf_file);
    if (n_bytes_read != sizeof(ehdr.ehdr.elf64_ehdr)) {
      fprintf(stderr, "ERROR: reading 64-bit ELF header\n");
      fclose(elf_file);
      return EXIT_FAILURE;
    }
    break;
  default:
    fprintf(stderr, "ERROR: unsupported ELF class\n");
    fclose(elf_file);
    return EXIT_FAILURE;
  }

  if (ehdr.relf_e_class == RELF_E_CLASS_32) {
    // relf_print_elf32_header(&ehdr);
  } else {
    relf_print_elf64_header(&ehdr.ehdr.elf64_ehdr);
  }

  fclose(elf_file);

  return EXIT_SUCCESS;
}
