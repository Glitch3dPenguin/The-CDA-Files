# AGENTS.md — agent contract for this repo

## What this is

Reverse-engineering record for **Dirty Bomb** (2015 Steam release, Splash Damage, project code name BAC, Steam app 333930). Engine = UE3 (late 3.5-generation, Win64) with a fork-extended package format and a hand-rolled payload cipher. End goal (owner-locked 10-14): run our OWN multiplayer servers + modded lobbies — a reimplemented Fireline lobby so the stock client points at our instance.

**Project state: SHELVED 2026-10-19** (owner decision — the project may be pulled away for a while). This repo is the most complete state capture, built so an agent can resume cold.

## Read order

1. This file
2. **PROGRESS.md** — the rolling log + the EXACT resume point (read this before anything else when resuming)
3. The `docs/` file(s) for the arc your task touches
4. The `data/` artifacts those docs cite (decompiles are named by function VA)

## Layout

| Path | Contents |
|---|---|
| `PROGRESS.md` | Rolling status log (10-14 → shelve) + exact resume point |
| `docs/01-engine-evidence.md` | UE3 3.5-gen Win64 identification + evidence |
| `docs/02-package-format.md` | The fork's re-tagged package format + compressed-region contract |
| `docs/03-cipher.md` | The hand-rolled block cipher: map, key, tables, real-CPU proof |
| `docs/04-loader.md` | The FMemory wrapper contract + FulfillCompressedRead + what "expansion" was |
| `docs/05-fireline-protocol.md` | Matchmaking state machine, in-process emulator, manifest chain, DAT resolution, service registration (the forgery seam) |
| `docs/06-eac.md` | EAC integration: loader/callback decompiles, soft/hard verdict, liveness, patch targets |
| `docs/07-extraction-ledger.md` | Census + extracted files + sizes + sha256 (game binaries are NOT in this repo) |
| `docs/08-toolchain-recipes.md` | Ghidra headless, REA bridge, real-CPU harness, PE mappers, scanner discipline |
| `scripts/` | `pe_map.py`/`pe_dd.py` (validated PE mappers) · `cpu_ref*.c` + `game_call.S` (real-CPU harness + shadow-safe thunk) · `hash_battery.py` · `sect_probe.py` |
| `data/decompiles/` | Ghidra 12.1.4 pseudocode, named `rea-<job>-FUN_<VA>.c.txt` / `rea-<job>-seqNN.code.txt` |
| `data/` | `tables.bin` (cipher tables) · `dec169_real.bin` (real-CPU decrypted SDPCEngine.u payload) · `eac_strrefs.txt` / `eac_url_strings.txt` · `fireline_imports.txt` (159 ordinals) · `fmem_calls.txt` (wrapper callers) · `ghidra-functions.txt` / `ghidra-strings.txt` (full exe dumps) · `zip-listing.txt` (all 17,502 depot entries) |

## Hard rules

- **No game binaries in this repo, ever** (exe / .u / maps / DLLs are copyrighted game content the owner has from his own install). `docs/07` carries names + sizes + sha256 so artifacts are re-verifiable from a fresh extraction. Same for the GDD (145 MB PDF) and any credentials.
- The cipher key string in docs/03 is fine to keep in the repo: it is a public fact of the shipped binary.
- One commit per coherent state change; commit messages state what changed and the evidence.
- When you extend the analysis: append a dated section to PROGRESS.md (the exact resume point must ALWAYS be current) + extend the relevant docs/ file. Keep the agent-first property: a cold agent should be able to resume from PROGRESS.md alone.

## Toolchain gotchas (the short list — full recipes in docs/08)

- **1 vCPU box**: a REA exe decompile is ~16–30 min; run heavy jobs in the background with a wake-based collect. Use the snapshot fast path (open with `snapshot_path`) or every step re-analyzes from scratch.
- **MSVC code executes only through the shadow-safe thunk** (`scripts/game_call.S`) — calling `call *%reg` from GCC C corrupts the caller frame silently (the emulator's root-cause bug).
- **PE mapping is non-identity for .data** (rawptr = VA − 0x1C00; section VAs are RVAs + ImageBase). Use `scripts/pe_map.py` / `pe_dd.py`; never hand-compute file offsets from VAs.
- **Every raw-byte scanner must be unit-tested against a known hit** before a zero result is believed (objdump cross-verifies every hit).
- **Disk is 20 G and the depot is NOT re-pullable** — never fetch game files again; work from the ledger.

## Provenance

Analysis produced by the Ash agent (e3ndr, Qwen3.8-27b) under the owner's work orders, 2026-10-08 → 2026-10-19. Decompiles = Ghidra 12.1.4 output via the REA (morluto/rea) MCP bridge. Channel history for the findings lives on Discord (#the-cda-files); this repo is the durable record.
