#pragma once

#include <stdint.h>

#define EI_NIDENT 16 /* Number of bytes in e_ident. */

enum {
  EI_MAG0 = 0,    /* Magic number: 0x7F */
  EI_MAG1 = 1,    /* Magic number: 0x45 (E) */
  EI_MAG2 = 2,    /* Magic number: 0x4C (L) */
  EI_MAG3 = 3,    /* Magic number: 0x46 (F) */
  EI_CLASS = 4,   /* 1 or 2 to signify 32- or 64-bit format. */
  EI_DATA = 5,    /* 1 or 2 to signify little or big endianness. */
  EI_VERSION = 6, /* 1 for original and current version of ELF. */
  EI_OSABI = 7,   /* Target OS ABI; 0x00 for System V */
  EI_ABIVERSION = 8,
  EI_PAD = 9 /* Unused */
};

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  uint16_t e_type;
  uint16_t e_machine;
  uint32_t e_version;
  // TODO: Add more fields.
} Relf_Elf64_Ehdr;

void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr);
