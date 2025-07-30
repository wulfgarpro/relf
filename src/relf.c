#include "relf.h"
#include <stdio.h>

void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr) {
  printf("e_ident_ei_mag: 0x%02x 0x%02x 0x%02x 0x%02x\n",
         ehdr->e_ident[EI_MAG0], ehdr->e_ident[EI_MAG1], ehdr->e_ident[EI_MAG2],
         ehdr->e_ident[EI_MAG3]);
  printf("e_ident_ei_class: 0x%02x\n", ehdr->e_ident[EI_CLASS]);
  printf("e_ident_ei_data: 0x%02x\n", ehdr->e_ident[EI_DATA]);
  printf("e_ident_ei_version: 0x%02x\n", ehdr->e_ident[EI_VERSION]);
  printf("e_ident_ei_osabi: 0x%02x\n", ehdr->e_ident[EI_OSABI]);
  printf("e_ident_ei_abiversion: 0x%02x\n", ehdr->e_ident[EI_ABIVERSION]);
  printf("e_type: 0x%04x\n", ehdr->e_type);
  printf("e_machine: 0x%04x\n", ehdr->e_machine);
  printf("e_version: 0x%08x\n", ehdr->e_version);
}
