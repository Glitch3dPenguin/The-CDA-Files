# 06 — Easy Anti-Cheat (verdict + patch targets + liveness)

## Integration model: the GAME loads EAC, not the other way around

The exe does **NOT statically import** EasyAntiCheat_x64.dll (all 44 import DLLs enumerated via a fresh PE walk — `pe_dd.py`/`pe_map.py`; fireline.dll + libSDMatchmakingSystem.dll + steam_api64.dll + WINHTTP + libcurl64 ARE static; EAC is not). Instead the exe carries its own EAC strings (`data/eac_strrefs.txt`, 3 ASCII + 16 W16, VAs recorded) and does it itself:

`LoadLibraryA("EasyAntiCheat_x64.dll")` (global 0x141FEFDD0) → `GetProcAddress("CreateGameClient")` → `CreateGameClient("GameClientInterfaceV011")` ← the EAC interface version string, pinned in the exe.

The three string-pointer globals (0x141FEBF14 AuthFailure · 0x141FEBF28 KickClient · 0x141FEFDD0 EAC_x64.dll) sit in ONE .data cluster with the cipher key global 0x141FEBBA8.

## Loader `FUN_1406BE2D0` (decompile: `data/decompiles/rea-eac3-FUN_1406BE2D0.c.txt`)

- LoadLibrary fail → log "Failed to load library." → **return 0**
- GetProcAddress fail → log → **return 0**
- CreateGameClient fail → log "Failed to create game client." → **return 0**
- Success → gClient @ **0x1422078B0**, hMod @ 0x1422078B8, `vtable[0](gClient, poolName, FUN_1406B8150, 0)` = init-with-callback → log "Successful Init." (success logs via a different logger global 0x141FEBB78 than the failures' 0x1420DFB00) → return 1

**Every sync failure = return 0 — the loader itself never closes/kicks; the caller decides on 0, and the async failures route through the callback.**

## Callback `FUN_1406B8150` (decompile: `data/decompiles/rea-eac4-FUN_1406B8150.c.txt`)

EAC init-finished handler: param≠0 → log "InitFinished - FAILURE." → `appRequestExit(1)` → "[EasyAntiCheat] Terminate." → `vtable[1](gClient)` destroy + FreeLibrary + **`abort()`** (process death; `_set_abort_behavior(0,3)` pre-set) · param==0 → return (success).

## Verdict: soft on connectivity, HARD on auth integrity

| Path | Behavior |
|---|---|
| Load/proc/client creation fails | return 0 — soft (caller decides) |
| Async init failure | **HARD: abort()** |
| Server message while GClientEAC==NULL | "Closing game." — hard close |
| ServerConnection==NULL | "Cannot send message to server (ServerConnection == NULL). Message added to queue" — **soft queue by design** |
| Auth/integrity events | "EasyAntiCheat Kick Client" / "EasyAntiCheat Authentication Failure" / "INTEGRITY VIOLATION" |

⇒ Stock client vs a no-EAC community server: connect probably fine; init-failure or the first server message = game over. Whether a 2015 EAC client completes init against 2026 backends = a RUNTIME question (below).

## Liveness probes (T1, 2026-10-19; URL templates from inside the DLL — `data/eac_url_strings.txt`)

- `https://gossip.easyanticheat.net/api/v1/games/` (discovery) → **DNS DEAD** (curl 6, could not resolve)
- `https://download.eac-cdn.com/api/v1/games/{{gameid}}/client/{{system}}/download/?uuid={{uuid}}` (+ `download-alt.` mirror) → **ALIVE** but both answer **S3 XML `AccessDenied` on every anonymous path** (fake gameid included) ⇒ 404-vs-403 indistinguishable from this box; a real gameid + the DLL's own headers would be needed
- No app-level heartbeat in the DLL (only `GameClient_PollStatus`/`GameClientP2P_PollStatus` exports + TCP-level SO_KEEPALIVE) ⇒ session maintenance = game-driven polling
- The gameid: not in the exe, the DLL, or the 27 inis (pattern scans zero hits) ⇒ runtime-downloaded config-JSON layer (the manifest chain, docs/05)

**Tiers:** T1 liveness = done (above) · **T2 = Wine harness + tcpdump** (wine not installed on the analysis box) · **T3 = a real client on Max's Win11 + Wireshark = the definitive test** (does a 2015 client complete init in 2026? what does it poll, and how often?).

## Patch targets if it fails (ALL PINNED)

1. The 3 sync `return 0` sites in `FUN_1406BE2D0` (treat as success / skip EAC entirely)
2. The `abort()` in `FUN_1406B8150` (init-failure → no-op)
3. The GClientEAC-NULL "Closing game." close site (let the session continue)

## EAC DLL API surface (REA recon, 2,945 procs; base 0x180000000)

CreateGameClient `0x180027db0` · CreateClientAuth `0x180027e70` · CreateGameLauncher `0x180027f20` · CreateThirdPartyLauncher `0x180028040` · CreateHttpsClient `0x1800280f0` · ClientAuth_Initialize/Destroy/ClientWriteChallengeResponse `0x180028170-0x1800281a0` · GameClient_PollStatus `0x1800281d0` · GameClient_NetProtect `0x1800281f0` · GameClientP2P_* `0x180028190-0x180028280` (Begin/EndSession · Register/UnregisterPeer · PollForMessageToPeer · ReceiveMessageFromPeer · UpdatePlatformUserAuthTicket · SetLogCallback) · Cerberus_* `0x180028290-0x180028490` (BeginFrame/EndFrame · GameRoundStart/End · PlayerSpawn/Despawn/Revive/Tick/UseWeapon/TakeDamage = the telemetry API) · NetProtectClient_Protect/UnprotectMessage/GetProtectMessageOutputLength `0x1800285c0-0x180028600` · GameLauncher_StartGameA/W `0x180028610/0x180028620` · ThirdPartyLauncher_Initialize `0x180028640`. Bundles libcurl/schannel (User-Agent "EasyAntiCheat Client", `EasyAntiCheat\Cache\`, If-None-Match/ETag conditionals).
