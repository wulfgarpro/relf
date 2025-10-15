#pragma once

#include <stdint.h>

// Number of bytes in e_ident.
#define EI_NIDENT 16

typedef enum {
  RELF_CLASS_NONE = 0,
  RELF_E_CLASS_32 = 1,
  RELF_E_CLASS_64 = 2
} Relf_E_Class;

typedef enum {
  RELF_E_DATA_NONE = 0,
  RELF_E_DATA_LE = 1,
  RELF_E_DATA_BE = 2,
} Relf_E_Data;

enum {
  EI_MAG0 = 0,       // Magic number: 0x7F
  EI_MAG1 = 1,       // Magic number: 0x45 (E)
  EI_MAG2 = 2,       // Magic number: 0x4C (L)
  EI_MAG3 = 3,       // Magic number: 0x46 (F)
  EI_CLASS = 4,      // 1 or 2 to signify 32- or 64-bit format.
  EI_DATA = 5,       // 1 or 2 to signify little or big endianness.
  EI_VERSION = 6,    // 1 for original and current version of ELF.
  EI_OSABI = 7,      // Target OS ABI; 0x00 for System V
  EI_ABIVERSION = 8, //
  EI_PAD = 9         //
};

typedef struct {
  uint8_t e_ident[EI_NIDENT];
} Relf_E_Ident;

// ELF Header
typedef struct {
  Relf_E_Ident e_ident; /* 16 */
  uint16_t e_type;      /* 24 */
  uint16_t e_machine;   /* 32 */
  uint32_t e_version;   /* 40 */
  uint64_t e_entry;     /* 48 */
  uint64_t e_phoff;
  uint64_t e_shoff;
  uint32_t e_flags;
  uint16_t e_ehsize;
  uint16_t e_phentsize;
  uint16_t e_phnum;
  uint16_t e_shentsize;
  uint16_t e_shnum;
  uint16_t e_shstrndx;
} Relf_Elf64_Ehdr;

typedef struct {
  Relf_E_Class relf_e_class; // 32 or 64 bit
  Relf_E_Data relf_e_data;   // LE or BE
  union {
    Relf_Elf64_Ehdr elf64_ehdr;
    // Relf_Elf32_Ehdr elf32_ehdr;
  } ehdr;
} Relf_Elf_Ehdr;

// Program Header
typedef struct {
  uint32_t p_type;
  uint32_t p_flags;
  uint64_t p_offset;
  uint64_t p_vaddr;
  uint64_t p_paddr;
  uint64_t p_filesz;
  uint64_t p_memsz;
  uint64_t p_align;
} Relf_Elf64_Phdr;

int relf_is_valid_elf_magic(const Relf_E_Ident *e_ident);
Relf_E_Class relf_determine_elf_class(const Relf_E_Ident *e_ident);
Relf_E_Data relf_determine_elf_data(const Relf_E_Ident *e_ident);
void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr);
