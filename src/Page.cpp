#include "Page.h"

#include <imgui.h>

#include "AMF.h"
#include "Settings.h"
#include "Strings.h"
#include "Weights.h"

namespace page
{
	namespace
	{
		using namespace settings;

		struct Row
		{
			Category    category;
			const char* key;       // TR key
			const char* english;
		};
		struct Group
		{
			const char*         key;
			const char*         english;
			std::vector<Row>    rows;
		};

		const std::vector<Group>& Groups()
		{
			// Every TR key is written out here in full (rule 66: translation-coverage.py reads them from the source).
			static const std::vector<Group> groups{
				{ "WL_GroupReading", "Reading", {
					{ kBooks, "WL_Books", "Books" },
					{ kScrolls, "WL_Scrolls", "Scrolls" } } },
				{ "WL_GroupAlchemy", "Alchemy", {
					{ kPotions, "WL_Potions", "Potions" },
					{ kFood, "WL_Food", "Food and drink" },
					{ kIngredients, "WL_Ingredients", "Ingredients" },
					{ kApparatus, "WL_Apparatus", "Alchemy apparatus" } } },
				{ "WL_GroupMagic", "Magic", {
					{ kSoulGems, "WL_SoulGems", "Soul gems" },
					{ kSigilStones, "WL_SigilStones", "Sigil stones" } } },
				{ "WL_GroupEveryday", "Everyday items", {
					{ kMisc, "WL_Misc", "Miscellaneous (gems, ores, clutter)" },
					{ kKeys, "WL_Keys", "Keys" },
					{ kArrows, "WL_Arrows", "Arrows" },
					{ kLights, "WL_Lights", "Torches" } } },
				{ "WL_GroupWorn", "Worn and wielded", {
					{ kJewelry, "WL_Jewelry", "Rings and amulets" },
					{ kClothing, "WL_Clothing", "Clothing" },
					{ kArmor, "WL_Armor", "Armour" },
					{ kWeapons, "WL_Weapons", "Weapons" } } },
			};
			return groups;
		}

		// An on/off switch (rule 32 - never a checkbox): the framework's own design (ApocryphaMenuFrameworkOR
		// include/utils/ToggleSwitch.h, as Camera Configuration Menu draws it) - a red/green track and a white knob in fixed
		// colours. The theme's colours showed only the knob: AMF's theme leaves Button and FrameBg clear (to-do list,
		// Weightless Menu 1.0.2 follow-ups). A normal navigable item for the controller.
		bool Switch(const char* a_label, bool* a_v)
		{
			ImGui::PushID(a_label);
			const float h = ImGui::GetFrameHeight();
			const float w = h * 2.0f;
			const float rr = h * 0.5f;
			const ImVec2 p = ImGui::GetCursorScreenPos();
			const bool pressed = ImGui::InvisibleButton("##switch", ImVec2(w, h));
			if (pressed) *a_v = !*a_v;
			const bool hot = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			auto* dl = ImGui::GetWindowDrawList();
			const ImU32 track = *a_v ? (hot ? IM_COL32(92, 191, 96, 255) : IM_COL32(76, 175, 80, 255))
			                         : (hot ? IM_COL32(207, 84, 84, 255) : IM_COL32(191, 68, 68, 255));
			dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), track, rr);
			dl->AddCircleFilled(ImVec2(p.x + rr + (*a_v ? w - h : 0.0f), p.y + rr), rr - 2.0f, IM_COL32(240, 240, 240, 255), 32);
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(a_label);
			ImGui::PopID();
			return pressed;
		}

		void Changed(const char* a_what)
		{
			settings::Save();
			weights::RequestApply();
			logger::info("page: {} - saved, weights set again on the next frame", a_what);
		}

		void Draw()
		{
			if (!AMF::UseFrameworkImGui()) {
				return;
			}
			strings::Refresh();
			const auto st = weights::GetStatus();
			ImGui::TextWrapped("%s", TR("WL_Intro", "Items in a category that is switched on weigh nothing in your inventory. Changes apply at once."));
			ImGui::Spacing();
			for (const auto& g : Groups()) {
				ImGui::SeparatorText(TR(g.key, g.english));
				for (const auto& r : g.rows) {
					bool on = settings::IsOn(r.category);
					if (Switch(TR(r.key, r.english), &on)) {
						settings::SetOn(r.category, on);
						Changed(std::format("{} {}", kCategoryKeys[r.category] + 1, on ? "on" : "off").c_str());
					}
					if (st.dataLoaded) {
						ImGui::SameLine();
						ImGui::TextDisabled(TR("WL_ItemCount", "(%u items)"), st.items[r.category]);
					}
				}
			}
			ImGui::Spacing();
			ImGui::Separator();
			if (ImGui::Button(TR("WL_Reset", "Reset to the defaults"))) {
				for (std::size_t i = 0; i < kCategoryCount; ++i) {
					settings::SetOn(i, kCategoryDefaults[i]);
				}
				Changed("reset to the defaults");
			}
			ImGui::SameLine();
			if (!st.problem.empty()) {
				ImGui::TextColored(ImVec4(1.0f, 0.55f, 0.35f, 1.0f), "%s", st.problem.c_str());
			} else if (!st.dataLoaded) {
				ImGui::TextDisabled("%s", TR("WL_Waiting", "Waiting for the game's items to load."));
			} else {
				std::uint32_t total = 0;
				for (const auto n : st.weightless) total += n;
				ImGui::TextDisabled(TR("WL_Status", "%u items weigh nothing."), total);
			}
		}
	}

	void Register()
	{
		if (!AMF::IsInstalled()) {
			logger::info("the Apocrypha Menu Framework is not installed - no settings page (the INI still applies)");
			return;
		}
		if (AMF::RegisterPage(kModName, "Settings", &Draw)) {
			logger::info("AMF {}: the Weightless Menu settings page is registered", AMF::Version());
		} else {
			logger::warn("AMF refused the Weightless Menu page");
		}
	}
}
