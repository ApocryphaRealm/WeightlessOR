#pragma once

// The "Weightless" page on the Apocrypha Menu Framework: one switch per item category, in groups, each with the number
// of items it covers. It is drawn on AMF's render thread, so it never touches a game form: a switch is saved and the
// weights are set again on the game thread (weights::RequestApply).

namespace page
{
	inline constexpr const char* kModName = "Weightless";
	void Register();
}
