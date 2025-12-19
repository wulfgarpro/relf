#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Number of bytes in e_ident.
#define EI_NIDENT 16

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum {
  RELF_E_CLASS_NONE = 0,
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
  EI_OSABI = 7,      // Target OS ABI
  EI_ABIVERSION = 8, // ABI version; meaning depends on EI_OSABI.
                     //   Usually 0 for System V / GNU/Linux.
  EI_PAD = 9         // Start of padding; zeroed.
};

typedef struct {
  u8 ident[EI_NIDENT];
} Relf_E_Ident;

// ELF Header
typedef struct {
  Relf_E_Ident e_ident; /* 16 */
  u16 e_type;           /* 18 */
  u16 e_machine;        /* 20 */
  u32 e_version;        /* 24 */
  u64 e_entry;          /* 32 */
  u64 e_phoff;          /* 40 */
  u64 e_shoff;          /* 48 */
  u32 e_flags;          /* 52 */
  u16 e_ehsize;         /* 54 */
  u16 e_phentsize;      /* 56 */
  u16 e_phnum;          /* 58 */
  u16 e_shentsize;      /* 60 */
  u16 e_shnum;          /* 62 */
  u16 e_shstrndx;       /* 64 */
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
  u32 p_type;   /* 4 */
  u32 p_flags;  /* 8 */
  u64 p_offset; /* 16 */
  u64 p_vaddr;  /* 24 */
  u64 p_paddr;  /* 32 */
  u64 p_filesz; /* 40 */
  u64 p_memsz;  /* 48 */
  u64 p_align;  /* 56 */
} Relf_Elf64_Phdr;

// Program Header Table
typedef struct {
  size_t phnum;
  union {
    Relf_Elf64_Phdr *elf64_phdr;
    // Relf_Elf32_Phdr* elf32_phdr;
  } phdrs;
} Relf_Elf64_Phdr_Table;

// Section Header
typedef struct {
  u32 sh_name;      /* 4 */
  u32 sh_type;      /* 8 */
  u64 sh_flags;     /* 16 */
  u64 sh_addr;      /* 24 */
  u64 sh_offset;    /* 32 */
  u64 sh_size;      /* 40 */
  u32 sh_link;      /* 44 */
  u32 sh_info;      /* 48 */
  u64 sh_addralign; /* 56 */
  u64 sh_entsize;   /* 64 */
} Relf_Elf64_Shdr;

// Section Header Table
typedef struct {
  size_t shnum;
  union {
    Relf_Elf64_Shdr *elf64_shdr;
    // Relf_Elf32_Shdr* elf32_shdr;
  } shdrs;
} Relf_Elf64_Shdr_Table;

bool relf_ident_is_elf(const Relf_E_Ident *eid);
Relf_E_Class relf_ident_class(const Relf_E_Ident *eid);
Relf_E_Data relf_ident_data(const Relf_E_Ident *eid);
void relf_print_elf64_header(const Relf_Elf64_Ehdr *ehdr);
void relf_print_elf64_phdr_table(const Relf_Elf64_Phdr_Table *phdrtab);
void relf_print_elf64_shdr_table(const Relf_Elf64_Shdr_Table *shdrtab);
void relf_pht64_destroy(Relf_Elf64_Phdr_Table *phdrtab);
void relf_sht64_destroy(Relf_Elf64_Shdr_Table *shdrtab);
