# WeightlessOR - changelog

Written as changes happen, not reconstructed afterwards (rule 61). A version number is issued by the version gate
only once a build is seen working in game (rule 48); until then the work sits under Unreleased.

## Unreleased

### Added
- the owner, 2026-09-29: "a settings page that classifies items by what category they're already in and sets them to have no weight in the inventory. And that's it." Sixteen categories from Oblivion's own item types (scrolls, food and jewellery told apart by the record's flags); a category that is on sets its items' weight to 0, off restores each item's own weight exactly. Applied once the game's data has loaded, and again at once on every change.
- the Apocrypha Menu Framework page "Weightless": a switch per category in five groups, each with its item count, a reset, a status line; eleven languages.
- the player's burden is recounted after a change mid-game.
- TestBench tool weightless.state (state, set, apply).

### Known
- not tried in game yet. Open: whether the inventory screen shows the new weights (the burden is counted by the game's own forms, which this changes), and whether keys, soul gems and sigil stones carry a weight component the game's RTTI finds (the page shows each category's item count).
