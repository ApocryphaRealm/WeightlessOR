#include "Weights.h"

namespace weights
{
	namespace
	{
		using settings::Category;

		std::atomic<bool>                        g_dataLoaded{ false };
		std::mutex                               g_statusLock;   // the page and the tool read the status from their threads
		std::atomic<bool>                        g_wanted{ false };
		Status                                   g_status;
		std::unordered_map<RE::TESFormID, float> g_original;   // every item's own weight, kept the first time it is seen

		// the record flags that split a form type (UESP's Oblivion record format): BOOK DATA 0x01 = scroll; ALCH ENIT
		// 0x02 = food item; CLOT biped slots 6 and 7 = rings, 8 = amulet
		constexpr std::uint8_t  kBookScroll = 0x01;
		constexpr std::uint8_t  kAlchemyFood = 0x02;
		constexpr std::uint16_t kJewelrySlots = (1u << 6) | (1u << 7) | (1u << 8);

		std::optional<Category> CategoryOf(RE::TESForm* a_form)
		{
			using T = RE::FormType;
			switch (a_form->GetFormType()) {
			case T::Book:
				return (static_cast<RE::TESObjectBOOK*>(a_form)->data.flags & kBookScroll) ? settings::kScrolls : settings::kBooks;
			case T::AlchemyItem:
				return (static_cast<RE::AlchemyItem*>(a_form)->data.flags & kAlchemyFood) ? settings::kFood : settings::kPotions;
			case T::Ingredient: return settings::kIngredients;
			case T::Apparatus: return settings::kApparatus;
			case T::SoulGem: return settings::kSoulGems;
			case T::SigilStone: return settings::kSigilStones;
			case T::Misc: return settings::kMisc;
			case T::KeyMaster: return settings::kKeys;
			case T::Ammo: return settings::kArrows;
			case T::Light: return settings::kLights;
			case T::Clothing:
				return (static_cast<RE::TESObjectCLOT*>(a_form)->bipedModelData.bipedObjectSlots & kJewelrySlots) ? settings::kJewelry : settings::kClothing;
			case T::Armor: return settings::kArmor;
			case T::Weapon: return settings::kWeapons;
			default: return std::nullopt;
			}
		}

		// The player's carried weight is cached (InventoryChanges::totalWeight / wornWeight); -1 makes the game count
		// it again, so a category switched mid-game shows in the burden at once.
		void RecountBurden()
		{
			auto* player = RE::PlayerCharacter::GetSingleton();
			auto* xc = player ? player->extra.GetExtraData<RE::ExtraContainerChanges>() : nullptr;
			if (!xc || !xc->changes) {
				logger::debug("weights: no player inventory yet - nothing to recount");
				return;
			}
			logger::debug("weights: the player's burden recounted (was {:.1f}, worn {:.1f})", xc->changes->totalWeight, xc->changes->wornWeight);
			xc->changes->totalWeight = -1.0f;
			xc->changes->wornWeight = -1.0f;
		}

		void Apply()
		{
			auto* forms = RE::TESForm::GetAllForms();
			if (!forms) {
				std::scoped_lock l(g_statusLock);
				g_status.problem = "the game's form table was not found";
				logger::error("weights: {}", g_status.problem);
				return;
			}
			std::array<bool, settings::kCategoryCount> on{};
			for (std::size_t i = 0; i < settings::kCategoryCount; ++i) {
				on[i] = settings::IsOn(i);
			}
			Status s;
			s.dataLoaded = true;
			s.applies = GetStatus().applies + 1;
			std::uint32_t changed = 0, noWeight = 0;
			for (auto& item : *forms) {
				auto* form = item.second;
				if (!form || form->IsDeleted()) {
					continue;
				}
				const auto cat = CategoryOf(form);
				if (!cat) {
					continue;
				}
				auto* w = RE::oblivion_cast<RE::TESWeightForm*>(form);
				if (!w) {
					logger::debug("weights: {:08X} (type {}) has no weight component - left alone", form->GetFormID(), static_cast<int>(form->GetFormType()));
					continue;
				}
				auto [orig, fresh] = g_original.try_emplace(form->GetFormID(), w->weight);
				const float original = orig->second;
				++s.items[*cat];
				if (original <= 0.0f) {
					++noWeight;   // weightless already (gold, most keys): nothing to do either way
					continue;
				}
				const float target = on[*cat] ? 0.0f : original;
				if (on[*cat]) {
					++s.weightless[*cat];
				}
				if (w->weight != target) {
					w->weight = target;
					++changed;
				}
			}
			{
				std::scoped_lock l(g_statusLock);
				g_status = s;
			}
			std::string counts;
			for (std::size_t i = 0; i < settings::kCategoryCount; ++i) {
				counts += std::format("{}{} {}/{}", counts.empty() ? "" : ", ", settings::kCategoryKeys[i] + 1, s.weightless[i], s.items[i]);
			}
			logger::info("weights: applied ({} changed, {} items weigh nothing by themselves) - weightless/found: {}", changed, noWeight, counts);
			if (s.applies > 1 && changed > 0) {
				RecountBurden();
			}
		}
	}

	void OnDataLoaded()
	{
		g_dataLoaded = true;
		g_wanted = true;
	}

	void RequestApply()
	{
		g_wanted = true;
	}

	void Tick()
	{
		// the data-loaded message is the signal; the form table filling up is the fallback (rule 17: never rely on
		// one moment during start-up)
		if (!g_dataLoaded) {
			// the table must have stopped growing (five checks a few seconds apart with the same size) - walking it
			// while the loader still adds forms on its own thread would race
			static std::uint64_t n = 0;
			static std::uint32_t lastSize = 0, steady = 0;
			if (++n % 500 != 0) {
				return;
			}
			auto* forms = RE::TESForm::GetAllForms();
			if (!forms || !RE::TESForm::LookupByID(0x00000007)) {   // the player's base form: the master file is in
				return;
			}
			const auto size = forms->size();
			steady = (size == lastSize && size > 0) ? steady + 1 : 0;
			lastSize = size;
			if (steady < 5) {
				return;
			}
			logger::info("weights: the game's forms are loaded ({} forms, steady; no data-loaded message seen) - applying", size);
			OnDataLoaded();
		}
		if (g_wanted.exchange(false)) {
			Apply();
		}
	}

	Status GetStatus()
	{
		std::scoped_lock l(g_statusLock);
		return g_status;
	}
}
