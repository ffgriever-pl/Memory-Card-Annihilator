# Memory Card Anihilator

A powerful tool for memory card management.

Originally made by ffgriever (with GFX design by berion)

Project revised by El_isra

## Changes

> The following changes were applied:

- Compilable on PS2DEV v1.0
- Native support for EXFAT Filesystem, no more messing with the no IOP reset flag
- Supports ProtoKernel PS2s (`SCPH-10000` & `SCPH-15000`)
- Alternative builds, each capable of either:
  - Handling Arcade    Memory Card  (`COH-H10020`)
  - Handling Developer Memory Card (`SCPH-10020 T`)
  - Handling Prototype Memory Card (?)

## Release package

Inside the release package you will find several folders, here's what each one offers:

- `mca_legacy`: MCA with the original modules. nothing too different from the older versions
- `mca_new`: USB drivers replaced with modules from latest SDK to offer EXFAT USB support. uses homebrew memory card modules instead of console OSD modules, making the program compatible with ProtoKernel PS2s in the process.
- `mca_Arcade`: Supports EXFAT USB and reads arcade memory cards instead of normal ones
- `mca_Prototype`: Supports EXFAT USB and reads Prototype memory cards instead of normal ones
- `mca_Developer`: Supports EXFAT USB and reads developer memory cards (and TDB StartUp Cards) instead of normal ones
