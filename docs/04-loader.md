# 04 — The package loader (wrapper contract + FulfillCompressedRead)

## The FMemory wrapper — `0x1406813A0` (contract decoded 100%)

| Arg | Meaning |
|---|---|
| rcx (1st) | `flags`. `flags & 0xF`: **0 = memcpy · 1 = check fn · 2 = check fn · ≥3 = assert** · **bit 9 = the ONLY cipher path** (roundup16 → alloc → memset → driver-decrypt in place → copy back) |
| rdx (2nd) | data |
| 5th arg | `len` (32-bit) |

**NO decompression anywhere** — every branch is same-size (the cipher is in-place).

### Callers (retraction of record, 10-19)

The 10-18 "zero E8 — pointer-table call" claim was **WRONG** (retracted in-channel 1561625225001574402). The wrapper has **7 DIRECT E8 callers**, zero indirect refs:

`0x14060002F` (40B trampoline) · `0x140604E10` / `0x140604F25` / `0x140604F99` (all inside tag fn `0x140604560`) · `0x14060D308` (inside tag fn `0x14060CFC0` — 3 tag hits + a driver-enc call = the package I/O cluster) · `0x14098EF28` · `0x141115E1E`. (`data/fmem_calls.txt` = the full verified site list.)

## Tag fn `0x140604560` = the fork's FArchiveCompressed::FulfillCompressedRead

(REA decompile: `data/decompiles/rea-seqtag1-seq01.code.txt`, 17,257 chars canonical; scope strings `L"FulfillCompressedRead"` / `L"FAsyncUncompress"` in-body; asserts cite `c:\gamedev\bac\...\core\inc\Array.h`.)

Flow:
1. 8B region header `[u32 tag][u32 chunkSize]` at `*(p2+0x1c)` (tag `0x9E2A83C1`; bswap32'd `0xC1832A9E` = the BE path).
2. `n = ceil(requested/chunkSize)` table entries `{word0 = on-disk size, word1 = logical size}` read up-front (n*8 bytes); entry[0] = the SUMMARY (undershoot bound + Σ check, docs/02).
3. Per chunk: straight file read at the current stream position (the `[rax+0x68]` vtable read, size = word0) into double buffers (one-chunk lookahead) → wrapper call with **flags = `*(p2+0x30)` — ONE field of the caller's request struct, re-read every iteration = constant per request, NEVER per-chunk ⇒ the chunk-flags hypothesis is DEAD** → dest base `*(p2+0x28)`, offset += word1; sync and async (task-graph) paths make the identical call.

**Consequence:** SDPCEngine.u's table = summary + ONE data chunk ⇒ the single-shot full-payload decrypt (docs/03) is EXACTLY what the loader does — same region, length, wrapper, flags.

## The "30,595 → 35,906 expansion" = a bookkeeping artifact (CIPHER ARC CLOSED, 10-19)

The three expansion candidates were decompiled (REA exe2 seq, `data/decompiles/rea-exe2-seq0[123].code.txt`):

- `0x140191500` (check fn 1) = a version/semver helper (parses the literal "1.2.3"; error codes -3/-5).
- `0x1406814E0` (check fn 2) = one-shot table init `FUN_1401819c0(0x2020,2,4,4,4,8,8,8,8,0x30)` gated on `DAT_142106E14==0`.
- `0x14060B860` (dispatch fn) = the package file-list loop (76B/0x4c entries; per-entry plain read vs FulfillCompressedRead vs name-lookup; **never sets the wrapper flags field**).

No size-changing branch exists anywhere in the load path ⇒ the tag fn simply advances dest by the DECLARED logical size; the transform is same-size (real-CPU-proven) ⇒ **5,311 bytes = dead buffer tail in the logical allocation.**

## Sibling functions

- fnB `0x140678440` = encrypted **UTF-16LE string loader** (a driver-dec caller; first-zero-u16 scan) — NOT the payload path (its 3 callers all sit in `0x141030C90`).
- fnD `0x14127BDB0` = fixed 1056B transform.
- Tag fn 2 `0x14060CFC0` = 10 real E8 call sites (0x1405DB2B6 · 0x1405E2747 · 0x14060427B · 0x14060CBD3 · 0x1406404F1 · 0x14064060D · 0x1406407B2 · 0x140651622 · 0x141293815 · 0x141295D41) — the package I/O cluster.
- Tag fn 1 `0x140604560` = **ZERO direct E8 + zero data pointers (VA and RVA forms, whole file)** = pure virtual dispatch (vtable slot built at runtime); the dispatch path reaches it through `FUN_14060B860`.
