# 02 — The fork's package format (and why UModel/FModel are moot)

## The twist

Every package (`.u`/`.upk`/`.umap`) opens with:

```
[tag 0x9E2A83C1][u32 stamp = 0x00090364]
```

- `0x9E2A83C1` = the STANDARD UE3 package tag (UE4 reuses the same value — do not read it as "UE4").
- `0x00090364` (590,692) = a **fork constant in the version slot, identical in every file**.
- Then a per-file u32 (TotalHeaderSize-class: 10,164,919 / 122,266 / 15,458 / 9,144 — each duplicated at [44:48]) · FString folder **"None"** · a **double-serialized count table** (a [29:33] value repeated at [112:116]).

That is a UE4-style FPackageFileSummary inside a UE3.5 build: the BAC fork re-tagged/extended the format.

## The fork's FPackageFileSummary (consistent across ALL sizes — 525 B `.upk` ↔ 298 MB `.u`)

tag · stamp · per-file size [8:12] (duplicated [53:57]) · folder "None" [12:21] · flags [21:25] · u16-padded count/offset pairs · 16-byte per-file ID [69:85].

**NO name table — names are stored INLINE** (verified: `Core`/`Class`/`Package`/`Engine`/`ObjectReferencer`/`ArrayProperty` as raw strings + script opcodes in the small .upks). ⇒ No standard UE3 reader (UModel/FModel/USDec) can parse any of these files. The magic-swap "Gaba" experiment is retired.

## Plaintext vs encrypted split

- **Small packages (~500 B .upk: Knife_01_Anim / Pliers_01 / SelfRevive_01) = plaintext.**
- **Large packages (all 15 .u, most maps, the big .upks) = uniformly 7.6–8.0 entropy, zero plaintext strings, no zlib/bzip2/xz/lz4/zstd signatures** ⇒ encrypted by the exe's cipher (docs/03).

## The compressed region (what the loader actually consumes — full contract in docs/04)

The loader's `FulfillCompressedRead` (tag fn 0x140604560) reads, per region:

```
[u32 tag 0x9E2A83C1][u32 chunkSize]          ; SDPCEngine.u: chunkSize = 0x20000
n * 8B table entries { word0 = on-disk size, word1 = logical size }
```

- entry[0] = SUMMARY: word1 must = Σ per-chunk word1s (else "incorrect uncompressed size" error); `summary.word0 + 8 + table` must fit the file (the undershoot bound).
- per chunk: straight file read at the current stream position (size = word0) → the FMemory wrapper transform (flags = ONE per-request field) → dest offset advances += word1.
- **SDPCEngine.u: region starts at file offset 145; table = 2 entries — summary {30595, 35906} + ONE data chunk {30595, 35906}; 145 + 8 + 16 + 30595 = 30764 = file size, zero slack.** The whole payload [169:30764) is a single read+transform.

## Header correction of record (10-19, supersedes the 10-18 "count=2 / hash 35890" note)

The region descriptor [0x91:0xA9) = `[tag c1 83 2a 9e][chunkSize 00 00 02 00] + entries {83 77 00 00 | 42 8c 00 00}×2`:

- "count=2" was a misread of `0x20000` (bytes `00 00 02 00`).
- "hash 35890" = a decimal mis-transcription of `0x8C42` = **35,906 = the declared logical (EXPANDED) size (word1), not a hash** ⇒ the 10-18 hash battery (12 hashes × 7 ranges, zero hits) was chasing a size = moot.
- The loader's bookkeeping "expects" 30,595→35,906; the proven transform is same-size (docs/03) ⇒ the 5,311 B difference is a dead buffer tail, not an expansion (docs/04).

## The (a,b) header table = a section map, but not of the .u payload

On the **plaintext upk** (Rosetta stone): the header's count/offset pairs form a **cumulative (start,len) section map** — 7 boundaries landing on 7 distinct content classes (inline "ArrayProperty" name @651 · a name table · strings · script bytes `0f 22 2d 26` · a 4-byte word table). On the **decrypted .u**: NO structure at any table-derived offset (uniform e≈7.9; 55 ascii-runs≥6 all garbage) and the cumulative chain OVERFLOWS (27304+39353=66657 > 30764) ⇒ the table does not describe the decrypted .u payload. The .u content layer (docs/03) is the one open MODDING item.
