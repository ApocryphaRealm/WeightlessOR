#pragma once

// Weightless.ini beside the plugin. The compiled defaults are the shipped INI's values (rule 16), checked at load;
// Save() rewrites only this plugin's keys in place with ordinary file I/O (never WritePrivateProfileString), so
// comments and unknown lines survive and a change on the page persists.

namespace settings
{
	// The item categories, in page order. Oblivion's own item types (it has no keywords: gems, ores and clutter are all
	// Misc); the INI key of each is kCategoryKeys[i].
	enum Category : std::size_t
	{
		kBooks, kScrolls, kPotions, kFood, kIngredients, kApparatus, kSoulGems, kSigilStones,
		kMisc, kKeys, kArrows, kLights, kJewelry, kClothing, kArmor, kWeapons,
		kCategoryCount
	};
	inline constexpr std::array<const char*, kCategoryCount> kCategoryKeys{
		"bBooks", "bScrolls", "bPotions", "bFood", "bIngredients", "bApparatus", "bSoulGems", "bSigilStones",
		"bMisc", "bKeys", "bArrows", "bLights", "bJewelry", "bClothing", "bArmor", "bWeapons"
	};
	// The defaults: everything weightless but what you wear or wield (the owner's own Weightless NG choice in Skyrim:
	// weapons and armour off).
	inline constexpr std::array<bool, kCategoryCount> kCategoryDefaults{
		true, true, true, true, true, true, true, true,
		true, true, true, true, true, false, false, false
	};

	struct Values
	{
		std::array<bool, kCategoryCount> categories = kCategoryDefaults;   // [Categories]
		int         logLevel = 2;   // [Log] uLogLevel (rule 14: shipped at info)
	};

	Values& Get();                          // load and start-up only; afterwards the accessors below

	// A category switch, from any thread (the page draws on AMF's render thread, the weights are set on the game
	// thread): read and set under one lock.
	bool IsOn(std::size_t a_index);
	void SetOn(std::size_t a_index, bool a_on);
	void    Load();
	bool    Save();
	std::filesystem::path PluginFolder();   // ...\OblivionRemastered\Binaries\Win64\OBSE\Plugins
	std::filesystem::path IniPath();
}
