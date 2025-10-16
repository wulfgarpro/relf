#include "relf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void relf_pht64_free(Relf_Elf64_Phdr_Table *phdr_table) {
  if (!phdr_table)
    return;
  free(phdr_table->phdrs.elf64_phdr);
  phdr_table->phdrs.elf64_phdr = NULL;
  phdr_table->phnum = 0;
}

int main(int argc, char *argv[]) {
  int rc = EXIT_FAILURE;

  const char *program_name = argv[0];
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <input>\n", program_name);
    return EXIT_FAILURE;
  }

  const char *path = argv[1];
  FILE *elf_file = fopen(path, "rb");
  if (!elf_file) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  Relf_E_Ident ident;
  size_t n_bytes_read = fread(&ident, 1, sizeof(ident), elf_file);
  if (n_bytes_read != sizeof(ident)) {
    fprintf(stderr, "ERROR: reading ELF ident\n");
    goto cleanup;
  }

  if (!relf_has_valid_magic(&ident)) {
    fprintf(stderr, "ERROR: invalid ELF file\n");
    goto cleanup;
  }

  Relf_Elf_Ehdr ehdr = {0};
  ehdr.relf_e_class = relf_ident_class(&ident);
  ehdr.relf_e_data = relf_ident_data(&ident);

  Relf_Elf64_Phdr_Table phdr_table = {0};

  rewind(elf_file);

  // TODO: Implement BE support
  if (ehdr.relf_e_data != RELF_E_DATA_LE) {
    fprintf(stderr, "ERROR: only LE ELF supported\n");
    goto cleanup;
  }

  // TODO: Implement 32-bit support.
  switch (ehdr.relf_e_class) {
  case RELF_E_CLASS_64:
    // Read ELF Header
    n_bytes_read =
        fread(&ehdr.ehdr.elf64_ehdr, 1, sizeof(ehdr.ehdr.elf64_ehdr), elf_file);
    if (n_bytes_read != sizeof(ehdr.ehdr.elf64_ehdr)) {
      fprintf(stderr, "ERROR: reading 64-bit ELF header\n");
      goto cleanup;
    }

    // Read Program Header Table
    uint16_t phentsize = ehdr.ehdr.elf64_ehdr.e_phentsize;
    if (phentsize != sizeof(Relf_Elf64_Phdr)) {
      fprintf(stderr,
              "ERROR: unexpected 64-bit program header table entry size\n");
      goto cleanup;
    }

    fseek(elf_file, ehdr.ehdr.elf64_ehdr.e_phoff, SEEK_SET);
    // TODO: handle error

    uint16_t phnum = ehdr.ehdr.elf64_ehdr.e_phnum;
    phdr_table.phnum = phnum;
    phdr_table.phdrs.elf64_phdr = calloc(phnum, sizeof(Relf_Elf64_Phdr));
    if (!phdr_table.phdrs.elf64_phdr) {
      fprintf(stderr, "ERROR: allocating memory for program header table\n");
      goto cleanup;
    }

    for (uint16_t i = 0; i < phnum; i++) {
      n_bytes_read = fread(&phdr_table.phdrs.elf64_phdr[i], 1,
                           sizeof(*phdr_table.phdrs.elf64_phdr), elf_file);
      if (n_bytes_read != sizeof(phdr_table.phdrs.elf64_phdr[i])) {
        fprintf(stderr, "ERROR: reading 64-bit ELF program header\n");
        goto cleanup;
      }
    }
    break;
  default:
    fprintf(stderr, "ERROR: unsupported ELF class\n");
    goto cleanup;
  }

  if (ehdr.relf_e_class == RELF_E_CLASS_32) {
    // TODO: Implement 32-bit support.
  } else {
    relf_print_elf64_header(&ehdr.ehdr.elf64_ehdr);
    relf_print_elf64_phdr_table(&phdr_table);
  }

  rc = EXIT_SUCCESS;

cleanup:
  relf_pht64_free(&phdr_table);
  if (elf_file) {
    fclose(elf_file);
  }

  return rc;
}
