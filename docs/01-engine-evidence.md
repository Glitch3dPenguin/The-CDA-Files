# 01 — Engine identification: UE3 (late 3.5-generation, Win64)

**Verdict:** Dirty Bomb (2015 Steam, app 333930) runs on **Unreal Engine 3, late 3.5 generation, 64-bit Windows** — not UE2, not UE4. The build is a Splash Damage fork (project code name **BAC**) that re-tagged the package format (docs/02) and rolled its own payload cipher (docs/03).

## Evidence (all file-verified 10-14)

- **The exe's own build paths:** `c:\gamedev\bac\floating\16adb548cc484f24\game\UnrealEngine3\Development\Src\...` — 553 "UnrealEngine3" string hits in `ShooterGame-Win32-Shipping.exe`, 0 UE4 tokens in 64,052 total strings; source files cite the UE3 layout (`UnBulkData.cpp`, `UnArchive.cpp`, `SavePackage.cpp`, `UnSkeletalMesh.cpp`, `core\inc\Array.h` — the latter visible in the assert sites of every decompiled loader function).
- **Binaries/Win64:** msvcr90 (MSVC 2008) · cudart64_30_9 (CUDA 3.0) · APEX_Destructible/Clothing · PhysX*64 · BugSplat64 · CoherentUI64 · fireline.dll · p4api (Perforce) · SharpSvn (the updater) · d3dcompiler_43/46.
- **The inis:** `SeekFreePCPaths`/`SeekFreePCExtensions` (UE3 cooked keys) · `[Engine.Engine]` ScoutClassName/GameEngine/`DefaultOnlineSubsystemName` = the `ShooterGame.SG*` C++ family · `GameName=Dirty Bomb` (+`_RUS`).
- **Public record:** Worthplaying 2015-01-06 ("Unreal Engine 3-powered") + the Steam engine threads.

## Wrong reads on record (do not repeat)

- 10-08 note said "UE 2.02" — dead.
- 10-14 morning "UE4 confirmed" — pattern-matched the `.upk`/`.umap` extensions; corrected in-channel the same day. The package tag `0x9E2A83C1` is the standard UE3 tag (the same value UE4 reuses) — its presence proves nothing about UE4.

## Config facts of record (DefaultEngine.ini)

`[URL]` MapExt=umap · DevEntryMap=Frontend.umap · TestingEntryMap=FuncTest_EmptyMap.umap · EXEName=ShooterGame.exe · GameName=Dirty Bomb (+_RUS) · `[Engine.Engine]` SG* family: SGEngine/SGWorldInfo/SGConsole/SGGameViewportClient/**SGOnlineSubsystemFireline**/PVEScout + Post_Process.PPC_Default · `[Core.System]` MaxObjectsNotConsideredByGC=156612 + the DebugStats* suppress list (the XP/kill/ability telemetry the GDD's badges run on).

## Network/auth context (owner intel, 10-08)

Networking = Steam common works · auth centrally managed by Splash Damage · **a hidden debug connect command exists** · wire-sniffing viable. Targets = libSDMatchmakingSystem.dll (159,744 B) + fireline.dll (503,296 B) + the exe.
