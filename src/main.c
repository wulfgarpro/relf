#include "relf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void relf_pht64_free(Relf_Elf64_Phdr_Table *phdrtab) {
  if (!phdrtab)
    return;
  free(phdrtab->phdrs.elf64_phdr);
  phdrtab->phdrs.elf64_phdr = NULL;
  phdrtab->phnum = 0;
}

static inline void relf_sht64_free(Relf_Elf64_Shdr_Table *shdrtab) {
  if (!shdrtab)
    return;
  free(shdrtab->shdrs.elf64_shdr);
  shdrtab->shdrs.elf64_shdr = NULL;
  shdrtab->shnum = 0;
}

int main(int argc, char *argv[]) {
  int rc = EXIT_FAILURE;

  const char *argv0 = argv[0];
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <input>\n", argv0);
    return EXIT_FAILURE;
  }

  const char *argv1 = argv[1];
  FILE *fd = fopen(argv1, "rb");
  if (!fd) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  Relf_E_Ident ident;
  size_t nbytes = fread(&ident, 1, sizeof(ident), fd);
  if (nbytes != sizeof(ident)) {
    fprintf(stderr, "ERROR: reading ELF ident\n");
    goto cleanup;
  }

  if (!relf_ident_is_elf(&ident)) {
    fprintf(stderr, "ERROR: invalid ELF file\n");
    goto cleanup;
  }

  Relf_Elf_Ehdr ehdr = {0};
  ehdr.relf_e_class = relf_ident_class(&ident);
  ehdr.relf_e_data = relf_ident_data(&ident);

  Relf_Elf64_Phdr_Table phdrtab = {0};
  Relf_Elf64_Shdr_Table shdrtab = {0};

  rewind(fd);

  // TODO: Implement BE support
  if (ehdr.relf_e_data != RELF_E_DATA_LE) {
    fprintf(stderr, "ERROR: only LE ELF supported\n");
    goto cleanup;
  }

  // TODO: Implement 32-bit support.
  switch (ehdr.relf_e_class) {
  case RELF_E_CLASS_64:
    // Read ELF Header
    nbytes = fread(&ehdr.ehdr.elf64_ehdr, 1, sizeof(ehdr.ehdr.elf64_ehdr), fd);
    if (nbytes != sizeof(ehdr.ehdr.elf64_ehdr)) {
      fprintf(stderr, "ERROR: reading 64-bit ELF header\n");
      goto cleanup;
    }

    // Read Program Header Table
    u16 phentsize = ehdr.ehdr.elf64_ehdr.e_phentsize;
    if (phentsize != sizeof(Relf_Elf64_Phdr)) {
      fprintf(stderr,
              "ERROR: unexpected 64-bit program header table entry size\n");
      goto cleanup;
    }

    fseek(fd, ehdr.ehdr.elf64_ehdr.e_phoff, SEEK_SET);
    // TODO: handle error

    size_t phnum = ehdr.ehdr.elf64_ehdr.e_phnum;
    phdrtab.phnum = phnum;
    phdrtab.phdrs.elf64_phdr = calloc(phnum, sizeof(*phdrtab.phdrs.elf64_phdr));
    if (!phdrtab.phdrs.elf64_phdr) {
      fprintf(stderr, "ERROR: allocating memory for program header table\n");
      goto cleanup;
    }

    size_t nitems = fread(phdrtab.phdrs.elf64_phdr,
                          sizeof(*phdrtab.phdrs.elf64_phdr), phnum, fd);
    if (nitems != phnum) {
      fprintf(stderr, "ERROR: reading 64-bit ELF program header table\n");
      goto cleanup;
    }

    // Read Section Header Table
    u16 shentsize = ehdr.ehdr.elf64_ehdr.e_shentsize;
    if (shentsize != sizeof(Relf_Elf64_Shdr)) {
      fprintf(stderr,
              "ERROR: unexpected 64-bit section header table entry size\n");
      goto cleanup;
    }

    fseek(fd, ehdr.ehdr.elf64_ehdr.e_shoff, SEEK_SET);
    // TODO: handle error

    size_t shnum = ehdr.ehdr.elf64_ehdr.e_shnum;
    shdrtab.shnum = shnum;
    shdrtab.shdrs.elf64_shdr = calloc(shnum, sizeof(*shdrtab.shdrs.elf64_shdr));
    if (!shdrtab.shdrs.elf64_shdr) {
      fprintf(stderr, "ERROR: allocating memory for section header table\n");
      goto cleanup;
    }

    nitems = fread(shdrtab.shdrs.elf64_shdr, sizeof(*shdrtab.shdrs.elf64_shdr),
                   shnum, fd);
    if (nitems != shnum) {
      fprintf(stderr, "ERROR: reading 64-bit ELF section header table\n");
      goto cleanup;
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
    relf_print_elf64_phdr_table(&phdrtab);
    relf_print_elf64_shdr_table(&shdrtab);
  }

  rc = EXIT_SUCCESS;

cleanup:
  if (fd) {
    fclose(fd);
  }
  relf_pht64_free(&phdrtab);
  relf_sht64_free(&shdrtab);

  return rc;
}
