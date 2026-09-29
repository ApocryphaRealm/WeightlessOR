#pragma once

#include "Settings.h"

// ============================================================================================================
// Items weigh nothing by category (the owner, 2026-09-29: "a settings page that classifies items by what category
// they're already in and sets them to have no weight in the inventory. And that's it."). Plan:
// 4. plans\weightless-oblivion\PLAN.md.
//
// Once the game's data has loaded, every item form (TESForm::GetAllForms) is put in its category - Oblivion's own item
// types, with scrolls, food and jewellery told apart by the record's own flags - and its TESWeightForm (found through
// the game's RTTI, so every item type is covered) is set to 0 when that category is on. Each item's original weight is
// kept, so switching a category off puts every weight back exactly. Nothing is written to a plugin or a save: the
// weights live in memory and are set again at every start. After a change mid-game the player's burden is recounted.
// ============================================================================================================

namespace weights
{
	void OnDataLoaded();   // any thread: the game's forms are loaded - apply on the next frame
	void RequestApply();   // a category was switched: apply again on the next frame
	void Tick();           // game thread, every frame

	struct Status
	{
		bool          dataLoaded = false;
		std::uint64_t applies = 0;
		std::array<std::uint32_t, settings::kCategoryCount> items{};        // item forms found per category
		std::array<std::uint32_t, settings::kCategoryCount> weightless{};   // of those, weightless now (a weight above 0 originally)
		std::string   problem;
	};
	Status GetStatus();
}
