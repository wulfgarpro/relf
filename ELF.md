# ELF

- <https://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/docs/elf.pdf>
- <https://en.wikipedia.org/wiki/Executable_and_Linkable_Format>
- <https://llvm.org/doxygen/BinaryFormat_2ELF_8h_source.html>

## Overview

Executable and Linkable Format (ELF) is the standard file format for executable files, object code,
shared libraries, and core dumps on Linux.

## File Layout

An ELF file is made up for *one* ELF header, followed by file data.

The file data can include:

- Program Header Table (PHT)
- Section header table (SHT)
- Data referred to by entries in the PHT / SHT

## Format

The ELF format has the following high-level layout:

```
+ -------------------- +
| ELF Header           | -> e_* fields
+ -------------------- +
| Program Header Table | -> p_* fields
+ -------------------- +
| File data:           |
|   .text              |
|   .rodata            |
|   ...                |
|   .data              |
+ -------------------- +
| Section Header Table | -> sh_ fields
+ -------------------- +
```

The order/placement isn't fixed in the specification.

## Elements

**ELF Header:**

Identifies the file as an ELF object and describes its overall layout.

**Program Header Table (Segments):**

Describes how the OS loader should load the ELF into memory at runtime. Defines segments, not
sections.

**Section Header Table (Sections):**

Describes how the file is organised for linkers and tooling. Defines sections such as `.text`,
`.data`, `.bss`, `.rodata`, `.symtab`, `.debug`, and more!

**Data:**

The raw bytes of the file.

## Summary

When executed, the OS loader only looks at ELF Header and Program Header Table (the segments). It
iterates over entries with `p_type=PT_LOAD` and maps their data to memory.

The segments are mapped at `p_vaddr` with perms `p_flags`, coping `p_filesz` bytes from `p_offset`
and more.

The loader doesn't care about the Section Header Table (the sections) at all.

Sections are just labels for tooling such as linkers, `objdump`, debuggers, etc. For example,
`.text`, `.rodata`, etc.

Any sections that must exist in memory have `s_flag=SHF_ALLOC`, and are laid out in the file so they
fall within loadable segments (i.e. `ptype=PT_LOAD`).

Sections outside loadable segments, e.g. debug info, can be stripped, and the binary will still
execute. See `man strip`.

The linker produces the arrangement. It merges input sections from `.o` files into output sections,
assigns file offsets and virtual addresses, then emits the segments (page-aligned ranges with R/W/X
flags) that cover all runtime bytes, plus special segments like `PT_INTERP`, `PT_DYNAMIC`,
`PT_NOTE`, etc.

### ELF Header

- Identifies class (i.e. 32-bit or 64-bit), endianness, file type, machine, entry point,
offsets/counts/sizes of PHT/SHT.

### Program Header Table

- Array of `p_*` entries the OS loader uses: `p_type=PT_LOAD`, etc.
- Each describes a page-aligned file -> memory mapping and its permissions.

### Section Header Table

- Array of `sh_*` entries that tools use: names via `.shstrtab`, types, and flags.
- Present in `.o` files; may be stripped in executables/shared objects.
