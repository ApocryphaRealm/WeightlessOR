// Weightless Menu for Oblivion Remastered - items weigh nothing by category, with a settings page on the Apocrypha Menu
// Framework. A clean rebuild: Skyrim's Weightless NG (VersuchDrei) is the reference for what it does, not for how.
// Plan: 4. plans\weightless-oblivion\PLAN.md.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Page.h"
#include "Settings.h"
#include "Tick.h"
#include "Weights.h"

namespace tool { bool Register(); }

namespace
{
	// every frame (from the message pump, on the game thread)
	void OnFrame()
	{
		weights::Tick();
		static bool          toolRegistered = false;
		static std::uint64_t n = 0;
		if (!toolRegistered && ++n % 60 == 0) {
			toolRegistered = tool::Register();
		}
	}

	void OnMessage(OBSE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg) {
			return;
		}
		switch (a_msg->type) {
		case OBSE::MessagingInterface::kPostLoad:
			tick::Install(&OnFrame);
			page::Register();
			break;
		case OBSE::MessagingInterface::kDataLoaded:
			logger::info("the game's data is loaded - the weights are set on the next frame");
			weights::OnDataLoaded();   // may arrive on the loading thread: only a flag here
			break;
		default:
			break;
		}
	}
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse);
	settings::Load();
	const auto level = static_cast<spdlog::level::level_enum>(std::clamp(settings::Get().logLevel, 0, 6));
	logger::set_level(level, level);
	// rule 14: the log names its level and how to get everything
	logger::info("Weightless Menu {} loaded (Oblivion Remastered) - log level {}; set uLogLevel=0 in Weightless.ini to capture everything",
		WL_VERSION, settings::Get().logLevel);
	if (auto* messaging = OBSE::GetMessagingInterface(); !messaging || !messaging->RegisterListener(&OnMessage)) {
		logger::error("OBSE messaging unavailable - the weights will not be set");
	}
	return true;
}
