# Weightless Menu

An OBSE64 plugin for The Elder Scrolls IV: Oblivion Remastered: items weigh nothing in your inventory, by category, set
on an in-game page of the Apocrypha Menu Framework. A clean rebuild - Skyrim's Weightless NG (VersuchDrei) is the
reference for what it does, not for how.

**Version 1.0.1.**

## What it does

When the game's data has loaded, every item form is put in its category - Oblivion's own item types - and its weight is
set to 0 when that category is on. Each item's own weight is kept, so switching a category off puts every weight back
exactly. Nothing is written to a plugin or a save; the weights are set again at every start, on every mod's items.

| Page group | Categories |
|---|---|
| Reading | Books, Scrolls (a book record flagged as a scroll) |
| Alchemy | Potions, Food and drink (a potion record flagged as food), Ingredients, Alchemy apparatus |
| Magic | Soul gems, Sigil stones |
| Everyday items | Miscellaneous (gems, ores and clutter are all Misc items in Oblivion), Keys, Arrows, Torches |
| Worn and wielded | Rings and amulets (clothing in a ring or amulet slot), Clothing, Armour, Weapons |

Defaults: everything on except Clothing, Armour and Weapons. A switch on the page saves to `Weightless.ini` and applies
at once; the player's burden is recounted.

## TestBench

`weightless.state`: op `state` (per category: on, items, weightless), op `set {category, on}`, op `apply`.

## Building

* [xmake](https://xmake.io) 3.0+, MSVC with C++23, and the submodule: `git clone --recurse-submodules`.
* `xmake f -p windows -a x64 -m releasedbg --toolchain=msvc`, then `xmake build Weightless`. Dear ImGui 1.90.8 docking is
  vendored under `extern/imgui` - the framework's own version, which `AMF::UseFrameworkImGui()` checks.
* The plugin and `dist/OblivionRemastered/Binaries/Win64/OBSE/Plugins/*` go to `OblivionRemastered\Binaries\Win64\OBSE\Plugins\`
  (Mod Organizer 2: the Root Builder layout). The translations ship in the framework's translation folder.

## Licence

GPL-3.0-or-later (`LICENSE`); it links CommonLibOB64 (GPL-3.0). The modding exception from the CommonLibOB64 plugin
template is kept as `EXCEPTIONS`. `include/AMF.h` and `include/TestBenchAPI.h` are MIT.
