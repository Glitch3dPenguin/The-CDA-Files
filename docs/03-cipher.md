# 03 — The payload cipher (hand-rolled, fixed-key, PROVEN on a real CPU)

## Verdict

The large-payload transform = a **hand-rolled T-table, AES-256-shaped, INVERSE (decrypt-direction) 16-byte block cipher inside the exe** — NOT the bundled OpenSSL 1.0.1c (red herring: zero CRYPTOGAMS calls out of 88 mapped loader-cluster targets). Fixed key; N=14 rounds; big-endian state assembly/writeback.

## The map (all VAs, ImageBase 0x140000000)

| Role | VA | Notes |
|---|---|---|
| Key (32B ASCII) | `0x1416B10C0` | `sdjJKLJsklaJSLKJDWLZMXNsldjKjalk` — consumed via the global |
| Key global | `[0x141FEBBA8]` | image value = `0x1416B10C0`; **2 readers, ZERO writers** (unit-tested rip-relative scan, objdump-verified: 0x1405CC7E3 + 0x1405CC893) ⇒ static |
| FMemory wrapper (cipher entry) | `0x1406813A0` | bit-9 of the flags arg → roundup16 → alloc → memset → driver-decrypt in place → copy back (docs/04) |
| Driver (decrypt) | `0x1405CC7A0` | builds the 240B 15-slot schedule struct, runs ceil(len/16) in-place blocks |
| Schedule builder | `0x1405cec30` | called as (struct, key-global, 0x100) — 0x100 = the 256-bit path |
| Round fn (decrypt) | `0x1405CCAA0` | AddRoundKey + N T-table rounds + final inverse-S round; big-endian in-place writeback |
| Round fn (encrypt, twin) | `0x1405CDB50` | via encrypt driver `0x1405CC850` |
| Round-count builder | `0x1405CEE40` | N=14, deterministic (the round fn does not mutate the struct) |
| Big schedule fn | `0x1405CD548` | **ZERO callers = off-path** |

Custom T-tables: `0x1416B2670` family (2A90/2ED0/3300) · std Rcon `0x1416B299C` · final-round table `DAT_1416B3740` (= file 0x16b2540, inverse-S-box family — independently re-confirmed by the REA decompile of the round fn).

## Tables (file offsets; image base 0x140000000)

S4 @`0x16b0fe0` (byte-exact standard S-box ×4) · A @`0x16b1470` · B @`0x16b1890` · C @`0x16b1cd0` · D @`0x16b2100` (inverse T-tables, 1-byte rotations of each other) · E @`0x16b2540` (inverse S-box ×4, byte-verified). **`data/tables.bin` = 6,208 B (A,B,C,D,E,S4 + the key blob).** Disasm gotcha: 64-bit table VAs print as 32-bit immediates in objdump dumps — grep `0x16b3740`, not `0x1416b3740`.

## Real-CPU proof (the emulator is RETIRED)

Method (`scripts/cpu_ref.c` + `scripts/game_call.S` + `cpu_ref2/3.c`): the exe is mmap'd, `.text`/`.rdata`/`.data-raw`/`.pdata` copied at `B+(VA−0x140000000)` (relative ⇒ rip-relative refs resolve exactly as in the non-PIE image), the key-pointer global `[B+0x1FEBBA8]` patched → `B+0x16B10C0`, and EVERY game-code call goes through the shadow-safe Windows-ABI thunk.

**Why the emulator died:** MSVC prologues spill rbx/rbp/rsi/rdi into the CALLER's 32-byte shadow area (legal MSVC; a GCC SysV frame keeps live locals there) = silent frame corruption, not a crash. The emulator also missed the wrapper's phase-2 table pass (0x1405cecb3+ over slots 01-13) + ~50 decode bugs. Never call MSVC code from GCC C without the thunk (docs/08).

Results:
- **N=14** (builder + wrapper, deterministic).
- **FULL 30,592B round-trip MATCH**: decrypt (0x1405CCAA0, swap-struct) → encrypt (0x1405CDB50, plain-struct), 1,912 blocks.
- The REX "anomaly" and N=15218609 retracted (test-battery defects: 2-CPU A/B byte-identical; garbage register read — the driver epilogue has no `mov eax,esi`).

## The artifact

**`data/dec169_real.bin`** = SDPCEngine.u payload [169:30764) = **30,595 B** (30,592 decrypted + 3 tail). sha256 `a65ba9f94213ea4f…` (full: docs/07 ledger). Entropy **7.905**, printable 0.359, NO inline names (the only 2 scan hits = coincidental 8-digit runs @30280/30309, 29 B spacing), NO compressor magic at head/tail (BZh / `78 9c` / `1f 8b` / `28 b5 2f fd` / `5d 00` / `04 22 4d 18` all absent).

## Bounded probes (negative results, on record)

- Hash battery vs 35890 (`0x8C42`): 12 hashes × 7 ranges = **zero hits** — moot anyway after the header correction (it's a declared size, not a hash).
- Double-decrypt (same transform on pass 1): no structure (printable 0.371, no names) ⇒ **no second layer under the same transform**.

## What's left

The **content layer**: the decrypted payload is genuinely high-entropy and ≠ identity (30,456/30,595 bytes differ) — a non-standard codec or intentional opacity. This is the ONE open MODDING item; the CONNECTION question (docs/05/06) does not depend on it.
