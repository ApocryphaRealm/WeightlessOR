// weightless.state (rules 31 and 64): op state (default) - per category: on, items, weightless; op set {category,
// on} - switch one as the page would (saved, applied on the next frame); op apply - set the weights again. Every
// accessor used here is thread-safe, so the handler answers on TestBench's own thread.
#include "Settings.h"
#include "TestBenchAPI.h"
#include "Weights.h"

namespace tool
{
	namespace
	{
		TestBenchAPI::ITestBenchInterface001* g_tb = nullptr;

		void Write(void* a_sink, TestBenchAPI::WriteFn a_write, const json& a_j) { a_write(a_sink, a_j.dump().c_str()); }

		json State()
		{
			const auto s = weights::GetStatus();
			json cats = json::object();
			for (std::size_t i = 0; i < settings::kCategoryCount; ++i) {
				cats[settings::kCategoryKeys[i] + 1] = { { "on", settings::IsOn(i) }, { "items", s.items[i] }, { "weightless", s.weightless[i] } };
			}
			return { { "version", WL_VERSION }, { "data_loaded", s.dataLoaded }, { "applies", s.applies }, { "problem", s.problem }, { "categories", cats } };
		}

		void Tool(void*, const char* a_args, void* a_sink, TestBenchAPI::WriteFn a_write)
		{
			json args = json::parse(a_args ? a_args : "{}", nullptr, false);
			if (args.is_discarded() || !args.is_object()) args = json::object();
			const std::string op = args.value("op", "state");
			if (op == "set") {
				const std::string name = args.value("category", std::string());
				for (std::size_t i = 0; i < settings::kCategoryCount; ++i) {
					if (name == settings::kCategoryKeys[i] + 1 || name == settings::kCategoryKeys[i]) {
						settings::SetOn(i, args.value("on", true));
						settings::Save();
						weights::RequestApply();
						Write(a_sink, a_write, { { "ok", true }, { "set", name }, { "on", settings::IsOn(i) }, { "note", "applied on the next frame" } });
						return;
					}
				}
				Write(a_sink, a_write, { { "ok", false }, { "error", "category: Books, Scrolls, Potions, Food, Ingredients, Apparatus, SoulGems, SigilStones, Misc, Keys, Arrows, Lights, Jewelry, Clothing, Armor, Weapons" } });
				return;
			}
			if (op == "apply") {
				weights::RequestApply();
				Write(a_sink, a_write, { { "ok", true }, { "note", "applied on the next frame" } });
				return;
			}
			if (op != "state") {
				Write(a_sink, a_write, { { "ok", false }, { "error", "op: state | set {category, on} | apply" } });
				return;
			}
			json out = State();
			out["ok"] = true;
			Write(a_sink, a_write, out);
		}
	}

	bool Register()
	{
		if (g_tb) return true;
		HMODULE tb = ::GetModuleHandleW(L"TestBench.dll");
		auto get = tb ? reinterpret_cast<void* (*)(unsigned)>(::GetProcAddress(tb, "TestBench_GetInterface")) : nullptr;
		g_tb = get ? static_cast<TestBenchAPI::ITestBenchInterface001*>(get(1)) : nullptr;
		if (!g_tb) return false;
		g_tb->RegisterTool("weightless.state",
			R"({"description":"Weightless: op state (default) - per category on / items / weightless; op set {category, on} - switch one as the page would; op apply - set the weights again","inputSchema":{"type":"object","properties":{"op":{"type":"string"},"category":{"type":"string"},"on":{"type":"boolean"}}}})",
			&Tool, nullptr);
		logger::info("TestBench tool registered: weightless.state");
		return true;
	}
}
