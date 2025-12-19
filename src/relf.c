#include "relf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool relf_ident_is_elf(const Relf_E_Ident *eid) {
  return eid->ident[EI_MAG0] == 0x7F && eid->ident[EI_MAG1] == 'E' &&
         eid->ident[EI_MAG2] == 'L' && eid->ident[EI_MAG3] == 'F';
}

Relf_E_Class relf_ident_class(const Relf_E_Ident *eid) {
  switch (eid->ident[EI_CLASS]) {
  case 1:
    return RELF_E_CLASS_32;
  case 2:
    return RELF_E_CLASS_64;
  default:
    return RELF_E_CLASS_NONE;
  }
}

Relf_E_Data relf_ident_data(const Relf_E_Ident *eid) {
  switch (eid->ident[EI_DATA]) {
  case 1:
    return RELF_E_DATA_LE;
  case 2:
    return RELF_E_DATA_BE;
  default:
    return RELF_E_DATA_NONE;
  }
}

void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr) {
  const u8 *eid = ehdr->e_ident.ident;
  printf("e_ident_ei_mag: 0x%02x 0x%02x 0x%02x 0x%02x\n", eid[EI_MAG0],
         eid[EI_MAG1], eid[EI_MAG2], eid[EI_MAG3]);
  printf("e_ident_ei_class: 0x%02x\n", eid[EI_CLASS]);
  printf("e_ident_ei_data: 0x%02x\n", eid[EI_DATA]);
  printf("e_ident_ei_version: 0x%02x\n", eid[EI_VERSION]);
  printf("e_ident_ei_osabi: 0x%02x\n", eid[EI_OSABI]);
  printf("e_ident_ei_abiversion: 0x%02x\n", eid[EI_ABIVERSION]);

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

void relf_print_elf64_phdr_table(const Relf_Elf64_Phdr_Table *phdrtab) {
  for (size_t i = 0; i < phdrtab->phnum; i++) {
    const Relf_Elf64_Phdr *phdr = &phdrtab->phdrs.elf64_phdr[i];
    printf("p_type: 0x%08x\n", phdr->p_type);
    printf("p_flags: 0x%08x\n", phdr->p_flags);
    printf("p_offset: 0x%016lx\n", phdr->p_offset);
    printf("p_vaddr: 0x%016lx\n", phdr->p_vaddr);
    printf("p_paddr: 0x%016lx\n", phdr->p_paddr);
    printf("p_filesz: 0x%016lx\n", phdr->p_filesz);
    printf("p_memsz: 0x%016lx\n", phdr->p_memsz);
    printf("p_align: 0x%016lx\n", phdr->p_align);
  }
}

void relf_print_elf64_shdr_table(const Relf_Elf64_Shdr_Table *shdrtab) {
  for (size_t i = 0; i < shdrtab->shnum; i++) {
    const Relf_Elf64_Shdr *shdr = &shdrtab->shdrs.elf64_shdr[i];
    printf("sh_name: 0x%08x\n", shdr->sh_name);
    printf("sh_type: 0x%08x\n", shdr->sh_type);
    printf("sh_flags: 0x%016lx\n", shdr->sh_flags);
    printf("sh_addr: 0x%016lx\n", shdr->sh_addr);
    printf("sh_offset: 0x%016lx\n", shdr->sh_offset);
    printf("sh_size: 0x%016lx\n", shdr->sh_size);
    printf("sh_link: 0x%08x\n", shdr->sh_link);
    printf("sh_info: 0x%08x\n", shdr->sh_info);
    printf("sh_addralign: 0x%016lx\n", shdr->sh_addralign);
    printf("sh_entsize: 0x%016lx\n", shdr->sh_entsize);
  }
}

void relf_pht64_destroy(Relf_Elf64_Phdr_Table *phdrtab) {
  if (!phdrtab)
    return;
  free(phdrtab->phdrs.elf64_phdr);
  phdrtab->phdrs.elf64_phdr = NULL;
  phdrtab->phnum = 0;
}

void relf_sht64_destroy(Relf_Elf64_Shdr_Table *shdrtab) {
  if (!shdrtab)
    return;
  free(shdrtab->shdrs.elf64_shdr);
  shdrtab->shdrs.elf64_shdr = NULL;
  shdrtab->shnum = 0;
}
