#include "relf.h"
#include <stdint.h>
#include <stdio.h>

int relf_is_valid_elf_magic(const Relf_E_Ident *e_ident) {
  const uint8_t *ident = e_ident->e_ident;
  return ident[EI_MAG0] == 0x7F && ident[EI_MAG1] == 'E' &&
         ident[EI_MAG2] == 'L' && ident[EI_MAG3] == 'F';
}

Relf_E_Class relf_determine_elf_class(const Relf_E_Ident *e_ident) {
  const uint8_t *ident = e_ident->e_ident;

  switch (ident[EI_CLASS]) {
  case 1:
    return RELF_E_CLASS_32;
  case 2:
    return RELF_E_CLASS_64;
  default:
    return RELF_CLASS_NONE;
  }
}

Relf_E_Data relf_determine_elf_data(const Relf_E_Ident *e_ident) {
  const uint8_t *ident = e_ident->e_ident;

  switch (ident[EI_DATA]) {
  case 1:
    return RELF_E_DATA_LE;
  case 2:
    return RELF_E_DATA_BE;
  default:
    return RELF_E_DATA_NONE;
  }
}

void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr) {
  const uint8_t *ident = ehdr->e_ident.e_ident;
  printf("e_ident_ei_mag: 0x%02x 0x%02x 0x%02x 0x%02x\n", ident[EI_MAG0],
         ident[EI_MAG1], ident[EI_MAG2], ident[EI_MAG3]);
  printf("e_ident_ei_class: 0x%02x\n", ident[EI_CLASS]);
  printf("e_ident_ei_data: 0x%02x\n", ident[EI_DATA]);
  printf("e_ident_ei_version: 0x%02x\n", ident[EI_VERSION]);
  printf("e_ident_ei_osabi: 0x%02x\n", ident[EI_OSABI]);
  printf("e_ident_ei_abiversion: 0x%02x\n", ident[EI_ABIVERSION]);

  printf("e_type: 0x%04x\n", ehdr->e_type);
  printf("e_machine: 0x%04x\n", ehdr->e_machine);
  printf("e_version: 0x%08x\n", ehdr->e_version);
  printf("e_entry: 0x%016lx\n", ehdr->e_entry);
  printf("e_phoff: 0x%016lx\n", ehdr->e_phoff);
  printf("e_shoff: 0x%016lx\n", ehdr->e_shoff);
  printf("e_flags: 0x%08x\n", ehdr->e_flags);
  printf("e_ehsize: 0x%04x\n", ehdr->e_ehsize);
  printf("e_phentsize: 0x%04x\n", ehdr->e_phentsize);
  printf("e_phnum: 0x%04x\n", ehdr->e_phnum);
  printf("e_shentsize: 0x%04x\n", ehdr->e_shentsize);
  printf("e_shnum: 0x%04x\n", ehdr->e_shnum);
  printf("e_shstrndx: 0x%04x\n", ehdr->e_shstrndx);
}
