Weightless for Oblivion
=======================
Version 1.0.0

An original, GPL-3.0-or-later OBSE64 plugin for The Elder Scrolls IV: Oblivion Remastered. Items weigh
nothing in your inventory, by category, and you choose the categories on an in-game settings page of
the Apocrypha Menu Framework.

THE CATEGORIES
--------------
  Reading            Books, Scrolls
  Alchemy            Potions, Food and drink, Ingredients, Alchemy apparatus
  Magic              Soul gems, Sigil stones
  Everyday items     Miscellaneous (gems, ores, clutter), Keys, Arrows, Torches
  Worn and wielded   Rings and amulets, Clothing, Armour, Weapons

Every item is placed by what it already is in the game's data, so items from other mods are covered
too. Out of the box everything is weightless except Clothing, Armour and Weapons.

USING IT
--------
  * Open the Apocrypha Menu Framework (F1), then Weightless. Each switch shows how many items it
    covers. A change applies at once and your burden is counted again.
  * Switching a category off gives every item back its own weight, exactly.
  * The same switches are in Weightless.ini, if you prefer to edit a file.
  * Nothing is written into a plugin or your save: remove the mod and every item weighs what it
    did before.

REQUIREMENTS
------------
  * The Elder Scrolls IV: Oblivion Remastered (Steam, runtime 1.512.105)
  * OBSE64 (Oblivion Script Extender 64)
  * Address Library for OBSE Plugins
  * Apocrypha Menu Framework for Oblivion Remastered - for the settings page (without it the INI
    still applies)

INSTALLING
----------
The plugin goes beside the game executable, in
OblivionRemastered\Binaries\Win64\OBSE\Plugins\. With Mod Organizer 2 that means the Root
folder layout (Root Builder); launch the game through OBSE64.

FILES
-----
  * OBSE/Plugins/Weightless.dll - the plugin (Weightless.pdb, its debug symbols, beside it)
  * OBSE/Plugins/Weightless.ini - the categories and the log level
  * OBSE/Plugins/ApocryphaMenuFramework/Translations/Weightless_*.txt - the page in eleven languages
  * The log is Documents/My Games/Oblivion Remastered/OBSE/Logs/Weightless.log. It lists how many
    items each category covers. It is written at info; set uLogLevel=0 in the INI for everything
    when reporting a problem.

CREDIT AND LICENCE
------------------
The idea comes from Weightless NG for Skyrim by VersuchDrei; this is a clean rebuild for Oblivion
Remastered with none of its code. GPL-3.0-or-later (LICENSE, NOTICE.md); the components it links and
their notices: THIRD_PARTY_NOTICES.md.
