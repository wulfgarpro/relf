# ELF

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

## Summary

When executed, the OS loader maps segments (e.g. PT_LOAD) to memory using the Program Header Table.

The segments are mapped at `p_vaddr` with perms `p_flags`, coping `p_filesz` bytes from `p_offset`
and more.

It doesn't look at the Section Header Table at all.

Sections are just labels for tooling such as linkers, `objdump`, debuggers, etc. For example,
`.text`, `.rodata`, etc.

The sections that must exist in memory are `SHF_ALLOC` and are laid out in the file o they fall within
loadable segments.

Sections outside loadable segments, e.g. debug info, can be stripped.

The linker produces the arrangement. It merges input sections from `.o` files into output sections,
assigns file offsets and virtual addresses, then emits the segments (page-aligned ranges with R/W/X
flags) that cover all runtime bytes plus special segments like `PT_INTERP`, `PT_DYNAMIC`, `PT_NOTE`,
etc.

`PT` is Program Header Type, i.e. `p_type` in a program header table entry.

`SHF` is Section Header Flag, i.e. `sh_flags` in a section header table entry.

### ELF Header

- Identifies class (i.e. 32-bit or 64-bit), endianness, file type, machine, entry point,
offsets/counts/sizes of PHT/SHT.

### Program Header Table

- Array of `p_*` entries the loader uses: `PT_LOAD`, etc.
- Each describes a page-aligned file to memory mapping and its permissions.

### Section Header Table

- Array of `sh_*` entries that tools use: names via `.shstrtab`, types, and flags.
- Present in `.o` files; may be stripped in executables/shared objects.
