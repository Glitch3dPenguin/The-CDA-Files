# 07 — Extraction ledger (names + sizes + sha256; game binaries intentionally NOT in this repo)

## Source

`Dirty Bomb.zip` — **6,064,626,341 B**, byte-exact to the owner's 10-09 drop; re-dropped 10-14 after a 10-13 disk-full emergency (the original was the culprit; logged in the daily with a transparency DM). DELETED 10-14 ~09:4x after the verified extraction (5.6 GB freed).

- **Census: 17,502 files / 7,167,451,720 B uncompressed**
- **`data/zip-listing.txt`** (2.0 MB) = the full per-entry listing (path + sizes) — the authoritative file inventory
- `unzip -t` = clean ("No errors detected in compressed data"; the integrity log was kept on the analysis box, not committed)

## Extracted (79 files / 993 MB; spot-stats byte-exact)

| Item | Size |
|---|---|
| 27 inis (Engine/Config 11 + ShooterGame/Config 16) | — |
| 15 `.u` class packages | **307,123,817 B** total |
| — ShooterGame.u | 298,810,970 |
| — Engine.u | 5,441,803 |
| — SDEngine.u | 1,375,027 |
| — SDPCEngine.u | 30,764 (the cipher-arc specimen; decrypted payload in `data/dec169_real.bin`) |
| — GameFramework / Core / IpDrv / OnlineSubsystemPC / AkAudio / GFxUI / GFxUIEditor / SubstanceAir / SubstanceAirEd / UnrealEd / WinDrv | (remainder) |
| `ShooterGame-Win32-Shipping.exe` ("Win32" = the fork's naming) | 36,740,096 |
| `libSDMatchmakingSystem.dll` (the matchmaking state machine, docs/05) | 159,744 |
| `EasyAntiCheat_x64.dll` (docs/06) | 872,064 |
| `steam_api64.dll` | 169,152 |
| `fireline.dll` (in the depot; **NOT extracted** — the owed drop, docs/05) | 503,296 |
| 5 `.upk`: Startup (30,456,129) · AkAudio_LOC_INT (193,419) · Knife_01_Anim / Pliers_01 / SelfRevive_01 (~500 B each, plaintext) | — |
| 28 of 175 `.umap` maps | 634 MB (every non-art map ≥12.8 MB + Frontend/Lobby/Loading = the menu flow; the menu map = Lobby.umap) |

**Skipped by design** (depot-only, never extracted): the 4 `.tfc` (2,530,652,783 B) + 15,324 `.webp` (396,628,618 B) texture/shader caches + ~1 GB of art/stitching/OOB maps.

## Integrity

- **`data/ledger-sha256.txt`** = sha256 of all 79 extracted files, generated at repo-creation time (2026-10-19) from the verified on-disk extraction.
- Decrypted artifact: `data/dec169_real.bin` sha256 `a65ba9f94213ea4f…` (full hash in the ledger) · cipher tables `data/tables.bin` (6,208 B) · mms DLL sha256 `c4f43cec…` (full in the ledger).

## Why the binaries are out

The game files are **copyrighted game content the owner holds from his own install** — they live on his machine / in his depot, not in this repo. The ledger above (names + sizes + sha256 + the full zip listing) is the re-verification path: a fresh extraction is byte-checkable against it. Same for the **GDD** (145,917,497 B PDF, 316 pp — a pure design bible: movement/weapons/abilities/augs/badges/modes, zero engine or network references; it lives on the analysis box, excluded here).
