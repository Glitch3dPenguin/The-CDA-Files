# Dirty Bomb decompile

Reverse-engineering record for the 2015 Steam release of **Dirty Bomb** (Splash Damage, project BAC) — a UE3 3.5-generation Win64 build with a fork-extended package format. Contents: the engine identification, the fork's package format, the hand-rolled payload cipher (proven on a real CPU against the game's own bytes), the package loader, the Easy Anti-Cheat integration (verdict + patch targets), the Fireline/matchmaking protocol surface (the "point the client at our instance" chain), and the extraction ledger.

**This repo is organized agent-first**: read [`AGENTS.md`](AGENTS.md) first, then [`PROGRESS.md`](PROGRESS.md) for the rolling status and the exact resume point. People will almost never dig through this — the docs are written so an agent can resume the project cold.

- **End goal (owner-locked):** our own multiplayer servers + modded lobbies — reimplement the Fireline lobby so the stock client points at our instance.
- **State: SHELVED 2026-10-19** (owner decision). The repo holds the most complete state, including the final decompile batch (manifest service registration).
- **Intentionally absent:** game binaries (exe/.u/maps/DLLs) and the GDD — copyrighted content the owner holds from his install. `docs/07-extraction-ledger.md` carries names + sizes + sha256 for byte-level re-verification.

One line of provenance: the analysis was produced by the Ash agent (e3ndr) under the owner's work orders, Oct 2026.
