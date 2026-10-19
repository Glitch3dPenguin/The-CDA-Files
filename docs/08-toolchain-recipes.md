# 08 — Toolchain recipes (and the gotchas that cost real time)

Environment: **1 vCPU (i5-10210U), 4 GB RAM, 20 G disk (93% used at shelve — game files are NOT re-pullable)**. Everything heavy runs serial; REA-class jobs run in the background with a wake-based collect.

## Ghidra 12.1.4 headless

Installed at `cda/tools/ghidra_12.1.4_PUBLIC` on the analysis box (NOT in this repo). Full exe analysis = ~25 min. Gotchas (all hit, all verified):

- The project **parent dir must pre-exist**.
- Raw `-postScript` paths outside script dirs are NOT compiled — use `-scriptPath` + a CLEAN dir (the scanner recurses; never point it at the distro). The script file must be named `<PublicClass>.java`.
- The GhidraScript program accessor is `getCurrentProgram()` (FlatProgramAPI, Base.jar) — no `getProgram()`, no bare `getMemory()`/`getFunctionManager()`.
- `Memory.getBytes` has no 3-arg overload; the `ghidra.program.model.search` package is gone from the script classpath (manual block-walk works).
- The headless JVM can **hang after success** — wrap in a timeout.
- `-process` RE-analyzes (use `-noanalysis` for script-only re-runs).
- The "X not found by <hash>" error class = a compile failure — manual `javac` against the distro jars is the fast diagnosis.
- `analyzeHeadless` arg-parse **rejects JVM flags like -Xmx** (the wrapper forwards them to the option parser) — launch via `java` + `GhidraClassLoader` instead.
- The 36 MB exe needs `-Xmx4G` (1536 M OOMs).
- Headless SCRIPT classpath limits (inherited interface methods like `Function.getSize`, DecompInterface) make some post-script dump scripts dead — the working full dumps (`data/ghidra-functions.txt` 146,995 lines · `data/ghidra-strings.txt` 96,475 lines) were produced by the validated postScript on 10-14.

## REA (morluto/rea) — the decompile engine

`npm rea-agents@6.7.0` (version-pinned) = the stdio MCP server; the analysis box bridges it behind a python seq runner (`tools/rea_mcp.py` on the box — modes list/call/schema/seq; NOT in this repo, the shape is below). Native provider = the existing Ghidra 12.1.4 (`doctor --provider ghidra` 9/9; REA never installs engines). 207 tools.

**Seq pattern (what was used for every decompile in data/):**
- seq file = JSON list of `[tool, args]` pairs. Arg keys: `procedure_*` take `procedure`, `search_strings` takes `pattern`, `xrefs` takes `address`.
- 3-step fast path: `open_binary` **with `snapshot_path`** (the saved analysis cache) → `procedure_pseudo_code` → `close_binary` (snapshot overwrite=true).
- Results = **flat JSON files, `REA_OUT` is a filename PREFIX**: `<prefix>-seqNN-<tool>.json` (NOT a directory). Each = MCP response; the code is the largest long-string field (`/raw_result/value` in the evidence shape). The runner prints `STEP n <tool> -> <file> (N chars) isError=None` lines.
- Env (all required on 1 vCPU): `REA_TIMEOUT=7200 REA_XMX=4G REA_GHIDRA_STARTUP_TIMEOUT_MS=7200000`. **Bridge gotcha (durable):** the bridge once hard-pinned the startup timeout to 900000, silently overriding the launch env — every exe step burned a 15-min fresh re-analysis. The timeout MUST be env-overridable.
- **Architecture (verified):** the Ghidra provider is STATELESS PER CALL (`-import` + `-deleteProject` on every op = fresh import + auto-analysis each tool call). The snapshot (`close_binary` `snapshot_path`) = REA's own **exact-match result cache** (target bytes + op + params + provider + settings must all match) — NOT a saved provider analysis. ⇒ the snapshot fast path is what makes 16-min exe calls possible; without it, every step re-analyzes the 36 MB exe from scratch.
- Budgets (1 vCPU): exe decompile ≈ 16–30 min · small DLL ≈ 8 min · recon (search+xrefs) ≈ 5 min. Run in background; monitor via result-file appearance + process liveness. **NEVER age-sweep a cooking `rea-*.log`** — a quiet-but-alive log has a stale mtime (one was deleted by a tidy sweep; the job survived but the lesson is the rule).

## The real-CPU harness (scripts/cpu_ref.c + game_call.S + cpu_ref2/3.c)

The only trusted executor for the game's own code (docs/03 for the results):

1. mmap the exe; copy `.text`/`.rdata`/`.data-raw`/`.pdata` at `B+(VA−0x140000000)` — relative placement ⇒ rip-relative refs resolve exactly as in the non-PIE image.
2. Patch the globals you need (e.g. the key pointer `[B+0x1FEBBA8]` → `B+0x16B10C0`).
3. Execute the game's OWN bytes for the transform.
4. **Every game-code call goes through the shadow-safe Windows-ABI thunk** (`game_call.S`): its own frame (`push rbx/r12/r13` + `sub $0x30,%rsp` = 16-aligned at the call), staging SysV `rdi/rsi/rdx/rcx/r8` → Windows `rcx/rdx/r8/r9`, `rax` out. **Never `call *%reg` from C** — MSVC prologues spill rbx/rbp/rsi/rdi into the caller's 32 B shadow area (legal MSVC; GCC SysV frames keep live locals there) = silent frame corruption. That was the emulator's root-cause bug (N=14 + full round-trip only became true through the thunk).
5. Verify with a FULL round-trip (decrypt via the swap-struct path → encrypt back via the plain-struct path → byte-compare), never with a prefix check.

## PE mappers (scripts/pe_map.py, scripts/pe_dd.py) — the validated pair

The 10-19 mapping of record (ImageBase 0x140000000; section VA fields are RVAs):

| Section | VA | raw ptr | file offset formula |
|---|---|---|---|
| .text | 0x140001000 | 0x400 | identity |
| .rdata | 0x141687000 | 0x1685e00 | file = VA − 0x140001200 |
| .data | 0x141feb000 | 0x1fe9400 | **file = VA − 0x1C00 (NON-identity)** |
| .pdata | 0x142264000 | 0x20cc800 | — |
| _RDATA | 0x14239e000 | 0x2205a00 | — |
| .rsrc | 0x1423a7000 | 0x220e200 | — |
| .reloc | 0x1423e1000 | 0x2247c00 | — |

- `imp_exp.py`'s `get_imports` is **RVA-broken** (`off()` wants VAs, it feeds RVAs → empty result) — the 44-import-DLL enumeration was re-done standalone; don't reach for it.

## Scanner discipline (the rule that saved the key hunt)

Every raw-byte scanner (E8/E9 callers, rip-relative reads/writes, LEA-rip, data pointers) MUST be **unit-tested against a known hit before any zero result is believed**:

- Known-reader gate: `0x1405CC7E3` = `48 8b 15 be f3 a1 01` → `0x141FEBBA8` (objdump-verified).
- Every hit cross-verified with objdump (AT&T prints resolved `# 0x...` comments) — the flat-scan adjacent-byte dup and non-instruction-boundary false positives are both caught this way.
- The failure mode this prevents (both hit, 10-19): an ununit-tested scanner + a wrong section-mapping assumption produced a phantom "key global = code address" and voided a whole session's scan results.

## scripts/ inventory

| File | Purpose |
|---|---|
| `pe_map.py` / `pe_dd.py` | validated PE section mappers (above) |
| `cpu_ref.c` + `game_call.S` (+`cpu_ref2/3.c`) | the real-CPU harness + shadow-safe thunk (above) |
| `hash_battery.py` | 12 candidate 16-bit hashes × N payload ranges — the generic "is this field a standard hash?" probe |
| `sect_probe.py` | table-model discriminator: cumulative (start,len) section map vs flat list, judged against a known-plaintext package (the upk Rosetta stone) |

## Ops notes

- 1 vCPU: serialize everything; background long jobs + arm a wake for the collect (the collect wake doubles as the next decision point).
- Disk: 20 G. Game-zip-class artifacts never sit on this box long-term; the depot is not re-pullable — work from `data/` + the ledger.
- Discord findings channel = #the-cda-files (one post per completed item; the channel is the human-facing log, this repo is the durable record).
