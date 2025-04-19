# Memory Card Annihilator

A powerful tool for memory card management.

Originally created by ffgriever (with GFX design by berion)

Project revised by El_isra

## Changes

### The following changes were applied:

- Compilable on PS2DEV v1.0
- Native support for the exFAT Filesystem, no more messing with the "no IOP reset" flag
- Supports ProtoKernel PS2s (`SCPH-10000` & `SCPH-15000`)
- Alternative builds, each capable of handling one of the following:
  - Handling Arcade Memory Card  (`COH-H10020`)
  - Handling Developer Memory Card (`SCPH-10020 T`)
  - Handling Prototype Memory Card (?)

## Release package

The release package contains several folders. Here's what each one offers:

- `mca_legacy`: MCA with the original modules. It's the most similar to older versions.
- `mca_new`: USB drivers replaced with modules from the latest SDK to provide exFAT USB support. It also uses homebrew memory card modules instead of console OSD modules, making the program compatible with ProtoKernel PS2s.
- `mca_Arcade`: Supports exFAT USB and reads arcade memory cards instead of standard ones.
- `mca_Prototype`: Supports exFAT USB and reads prototype memory cards instead of normal ones.
- `mca_Developer`: Supports exFAT USB and reads developer memory cards (and TDB StartUp Cards) instead of standard ones.

## Translators and proofreaders

- Brazilian Portuguese: Venão
- English: ffgriever
- French: Gnom, unversdelait
- German: H4ndy, TnA
- Italian: xeeynamo
- Polish: ffgriever
- Spanish: El_Patas, HWNJ
- Turkish: Grey_Wolf, Rockman98
- Russian: dimm0505, alex_wallace
- Japanese: ffgriever

An alternative Brazilian Portuguese translation provided by:
- Germanno, DanielSantos and gledson999

## How to use language files

MCA automatically detects your console's language setting (system locale) and attempts to load the corresponding language file.

If you want to override this automatic detection (for example, to use English in the app while your console is set to Spanish), or if your desired language cannot be set via the PS2's OSDSYS menu, you can manually specify a language file. To do this, rename the language file you wish to use (e.g., `lang_es.lng`, `lang_de.lng`) to `lang.lng`.

The `lang.lng` file, if present in the same directory as the MCA executable, will always be loaded, taking priority over the system locale detection.
