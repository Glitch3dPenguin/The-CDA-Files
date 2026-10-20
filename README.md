# The CDA Files — Dirty Bomb decompile

Reverse-engineering record for the 2015 Steam release of **Dirty Bomb** (Splash Damage, internal project name BAC) — a UE3 3.5-generation Win64 build with a fork-extended package format. What's here: the engine identification, the fork's package format, the hand-rolled payload cipher (proven on a real CPU against the game's own bytes), the package loader, the Easy Anti-Cheat integration (verdict + patch targets), and the Fireline/matchmaking protocol surface (the "point the stock client at our instance" chain).

**End goal (owner-locked):** our own multiplayer servers + modded lobbies — reimplement the Fireline lobby so the stock client can be pointed at our instance.

> **Note:** this repository is an analysis record, not a game. It deliberately contains **no game binaries** (exe / .u / maps / DLLs) and no design documents — those are copyrighted content the owner holds from his own install. `data/ledger-sha256.txt` carries name + size + sha256 for every extracted file so the source inventory can be re-verified byte for byte.
>
> **The repo is organized agent-first.** Read [`AGENTS.md`](AGENTS.md) first, then [`PROGRESS.md`](PROGRESS.md) — the rolling status and the exact resume point. The docs are written so an agent can resume the project cold; people will almost never need to dig deeper than this file.

## Status

| Layer | Status | Record |
| --- | --- | --- |
| Engine identification | Done | UE3 (late 3.5-generation, Win64) fork — build paths, configs, public record | [`docs/01`](docs/01-engine-evidence.md) |
| Package format | Done | The fork re-tags the package summary (no name table; standard UE3 readers cannot parse it) | [`docs/02`](docs/02-package-format.md) |
| Payload cipher | **Proven on a real CPU** | Hand-rolled T-table block cipher (fixed key in the image); full 30,592-byte round trip through the game's own bytes | [`docs/03`](docs/03-cipher.md) |
| Package loader | Done | Chunked-read contract (`FulfillCompressedRead`-class): per-chunk on-disk/logical sizes, one flags field per request | [`docs/04`](docs/04-loader.md) |
| Anti-cheat (EAC) | Verdict in (static + liveness probes) | The game itself `LoadLibrary`s EAC: soft-queues on connectivity, aborts the process on init/auth failure; patch targets pinned; a 2015 client against the 2026 backend = needs a live capture | [`docs/06`](docs/06-eac.md) |
| Fireline / matchmaking | Chain mapped, wire spec in flight | Matchmaking state machine + in-process Fireline emulator in the client DLL; manifest `services` schema `{name, url, data(+client)}` that registers endpoints; fireline.dll export/import map resolved (159/159 named) | [`docs/05`](docs/05-fireline-protocol.md) |
| Source inventory | Done | 79 files extracted, sha256 ledger verified 79/79 | [`docs/07`](docs/07-extraction-ledger.md) |

What the cipher proof settles: the "encrypted" packages decrypt to genuinely high-entropy content with no standard codec signature — the content layer under the cipher (if any) is the one open modding item, and it does not block the server-connection question.

**Project state: SHELVED (owner decision, 2026-10-19).** The repo holds the most complete state of the work, including the final decompile batch.

## Repository layout

| Path | What it is |
| --- | --- |
| [`AGENTS.md`](AGENTS.md) | **Read this first.** The agent contract: read order, hard rules (no game binaries ever land here; `PROGRESS.md` must always carry the resume point). |
| [`PROGRESS.md`](PROGRESS.md) | Rolling work log + **EXACT RESUME POINT** — a cold agent resumes from this file alone. |
| `docs/01`–`docs/08` | Per-layer findings: engine · package format · cipher (+ real-CPU proof) · loader · Fireline protocol · EAC · extraction ledger · toolchain recipes. |
| `scripts/` | Reusable, self-contained tooling: PE section mappers, the real-CPU cipher harness (C + a Windows-ABI thunk), hash battery, section probe. |
| `data/` | Raw artifacts: decompiles, cipher tables, decrypted samples, string/import dumps, the sha256 ledger. |

## Resuming the project

1. Read `AGENTS.md`, then the "exact resume point" section of `PROGRESS.md`.
2. The open items there are ordered by dependency (wire spec → live manifest capture → the .u content layer → anti-cheat liveness).
3. Keep `PROGRESS.md`'s resume point exact — that is the point of the layout.

## Contributing

This is an agent-first work record: agents do the work under the owner's orders, and `PROGRESS.md` is the single source of truth for state. If you pick it up — human or agent — `AGENTS.md` is the contract, and any work should land as a docs/data/scripts addition that keeps the resume point exact.

## Additional Information

### Which build is this?

The 2015 Steam release (app 333930), English, 64-bit shipping build — `ShooterGame-Win32-Shipping.exe` ("Win32" in the name is a fork naming quirk; the binary is x86-64). Verify your copy against the checksums below; the full 79-file inventory is in [`data/ledger-sha256.txt`](data/ledger-sha256.txt).

- `ShooterGame-Win32-Shipping.exe` `sha256: f4a52334e28ac3a32831e37a74caf2f50f572ce69f849a6c792a952cf15dabaa`
- `libSDMatchmakingSystem.dll` `sha256: c4f43cecc8c0e530b86faf94b04451164cc96949351f015f7716b3101c905549`
- `EasyAntiCheat_x64.dll` `sha256: 0a4e07fb45e6414ffbd769435601166507ac6dfdea6782cc5552aaa1b75fe808`

### Provenance

The analysis was produced by the Ash agent (e3ndr) under the owner's work orders, October 2026.
