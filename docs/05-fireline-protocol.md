# 05 — Fireline / matchmaking protocol surface (the "point the client at our instance" chain)

## Topology

```
exe ──(159 ORDINAL imports)──► fireline.dll            (HTTP transport: endpoints/headers/auth)
exe ──(static, named)───────► libSDMatchmakingSystem.dll (matchmaking STATE MACHINE)
```

- `data/fireline_imports.txt` = the 159 ordinal thunks (0x141FE3D50–0x141FE4248). Ordinal→name mapping needs fireline.dll's export table = **the owed drop** (503,296 B, `Binaries/Win64/fireline.dll`, ships with the base game; in the depot zip, never extracted — disk 93% = no re-pull).
- fireline.dll and libSDMatchmakingSystem.dll are static imports; EAC is not (docs/06).

## libSDMatchmakingSystem.dll (REA recon, 891 procs / 564 strings; sha256 c4f43cec…)

1. **Matchmaking STATE MACHINE:** `MMS_REQUEST_TICKET → MMS_MATCHING → MMS_MATCHED → MMS_ACCEPT_MATCH → MMS_ACCEPTED_LEADER|FOLLOWER → MMS_MATCH_STARTED_WAIT → MMS_MATCH_STARTED` + the `MME_*` error taxonomy (TICKET_EXPIRED · REQUEST_TICKET_USER_PARAMS_INCORRECT · TIMEOUT_WAITING_FOR_OTHER/LOCAL_ACCEPT · TIMEOUT_FINDING_MATCH …).
2. Drives **fireline.dll through ~15 NAMED imports** (import names stored in-binary): `fl_r_matchmaking_ticket` / `continuation_ticket` / `keepalive` / `accept` / `cancel` / `start` / `create_proof` / `check_proof` / `create_manual_proof` · `fl_client_get_identity_id` · `fl_r_mark_identity_active` · `fle_*` (the emulator twin set) · `fl_json_*` · `fl_response_get_json`/`header` · `fl_options_from_json`. JSON responses: matchable status update `{id, sequence}` · hopper info `{'items':…}` · match info join/matched/user json. Timeouts (matchmaking/accept/start) runtime-configurable (`sdmm_set_*`).
3. **A FULL IN-PROCESS FIRELINE EMULATOR is embedded:** `SDFirelineEmulator` (Client/Task/TaskMatchmakeClient/TaskAccept/TaskCancel/TaskStart/TaskMatchmakingTicket) + `Hopper` class ("Hopper does not exist - creating hopper %s" · "Generated ticket %s for flClient … and hopper %s") + `StartMatchForClient` + TASK_TYPE_MATCHMAKING_TICKET/MATCHMAKE_CLIENT/ACCEPT + the `"SetEmulator: %s Fireline emulator"` switch. The game ships a LOCAL MOCK Fireline server ⇒ the protocol semantics a compliant server must serve are readable in the game: task vocabulary ticket→keepalive→accept→start-with-join-info→proof, state transitions, JSON shapes.
4. **ZERO URLs in the DLL** — the HTTP transport (endpoints/headers/auth) is entirely fireline.dll's.
5. The init fn (`data/decompiles/rea-mms3-FUN_1800050c0.c.txt`) = client-agnostic hook-up ONLY: `if (this->fireline_client+0x40 == 0) { log("InitFireline: Cannot initialize without a fireline client"); return; }` then `fl_client_add_log_func` + `fl_client_add_msg_func` + `fl_client_start_listening` — the fireline client is built EXTERNALLY and handed in; mms carries no config. `fle_emulator_is_enabled` (`FUN_1800051e0`) = byte 0 of a singleton.

## The exe-side config chain (no URLs in the exe)

Full string sweep: the exe carries NO Fireline endpoint URLs (only Adobe/flash + OpenSSL-FAQ `http://` strings). The config = **downloaded JSON**:

- USTRUCTs: `SGServerConfiguration` / `SGLobbyServerConfiguration` / `SGMatchmakingLobbyServerConfiguration`
- `USGServerConfiguration::SetServerConfigurationFromJson` (0x141a5bcb8) · `USDEngineExtensions::GetServerConfigurationData` (0x141a62b78)
- The manifest family: `SGGlobalManifest` (0x141bdc420) + `USGGlobalManifest::IsLiveEnvironment` + `USGEngineExtensions::RefreshManifests` (0x141be0d38) + `USDOnlinePlayerInterfaceFireline::RetrieveManifest` (0x141c149e8); strings `manifests` / `retrieving_manifests` (0x141c165d8) / `manifest_refresh` (0x141c166e8, 0x141c167f0) / `missing_manifest` / `GetManifest`; "No global manifest could be found, cannot initialize matchmaking" (0x141c11de0) · "Attempting to retrieve a manifest without a valid fireline subsystem" ×2 · "Malformed manifests response. Expected 'items'" (0x141c15c70). Manifests also drive server QoS selection ("Attribute data not found in manifest %s. QoS results will not be available").
- ini `NoPatchFiles`: `ShooterGame\Config\`**FirelineConfig_WORLDWIDE_client.json** + **FirelineConfig_RUS_client.json** + **ServerConfiguration.json** + SaveFormat.json — the FirelineConfigs + ServerConfiguration are **NOT in the depot** (zip-listing verified: only SaveFormat.json, 28,056 B) ⇒ runtime-downloaded or legacy. DefaultServerConfig.ini = gameplay settings only (MaxPlayers 16 …).

## man2 — the boot state machine + manifest dispatcher (`data/decompiles/rea-man2-*.c.txt`)

- `FUN_141542680` = the **login-state name formatter** (switch on this+0x56c): 0="unauthenticated" · 1="requesting_application_ticket" · 2="authenticating" · 3="retrieving_manifests" · 4="retrieving_profile" · 5="retrieving_game_specific_data" · 6="completed" (default "unknown") — the client's online boot sequence is a readable state machine.
- `FUN_1415483e0` = the **manifest JSON dispatcher** `fn(name, json)`: "schemas" → store (0x1411fbc00) · "matchmaking" → SDEngine object at this+0x5e8 · "globals" · **"services" → per entry reads the `name` + `url` + `client` fields → FUN_1411c4620(svcCtx, name, config, client)** · "granted_rewards" → iterate `{name, blob}` → vtable+0x2d8 · then `manifest_refresh` (float s → this+0x594/0x598) + `listener_keepalive` (float×1000 → ms timer FUN_14125fa70) · tail = vtable handler+0x308 (name, json).

## DAT resolution (10-19 16:1x) — the manifest field names, cracked

All 9 field-name keys from the dispatcher = **string literals in .rdata** (the 0x141c1xxxx cluster sits fully in .rdata; file = VA − 0x140001200):

| VA | literal |
|---|---|
| 0x141c14424 | `url` |
| 0x141c15db8 · 0x141c15f98 · 0x141c165cc · 0x141c16b48 · 0x141c16eb8 | `data` (×5) |
| 0x141c16d20 · 0x141c16fec | `name` |
| 0x141c170a4 | `blob` |

⇒ **manifest "services" entry schema = `{name, url, data (+client)}`** · **"granted_rewards" entry = `{name, blob}`.** THIS is the shape to forge for "point the client at our instance". Adjacent corroborating strings in the same .rdata neighborhood: map_name · profile_retrieval_failure · challenge_timeout · prelogin_failure · "Join Failure" · "You are banned from playing on this…" · "One or more players failed to join…" · "Invalid loadout options" (the URL/auth-challenge machinery — OnURLCallback, challenge_timeout/failure — sits here too).

## FUN_1411c4620 — service registration (the last decompile before shelve; `data/decompiles/rea-svc1-FUN_1411c4620.c.txt`)

Per manifest service entry, the exe:
1. Looks the **name** up in the svcCtx registry (param_1+0xd). Duplicate → log `"Attempting to add Fireline service '%s' more than once"` + no-op (return 0).
2. Converts **url** (param_3) and **client** (param_4) ASCII→wchar.
3. Calls fireline.dll's **`fl_client_add_service(firelineClient, url_w, client_w)`** — `*param_1` = the fireline client pointer handed in at init.
4. Success → log `"Registered Fireline service: %s"` + appends the `{name, url, client}` record (three FString slots at 0x30) to the registry, returns 1. Failure (fl_client_add_service == 0) → log `"Failed to register Fireline service: %s"`, returns 0.

**Implications (the forgery seam, sharpened):**
- The **service-name vocabulary is NOT hardcoded in the exe** — the manifest defines whatever names it wants; the client registers each entry verbatim.
- The **`url` field is the endpoint passed straight into fireline.dll** = where "our instance" goes.
- Unknowns that only a live capture or fireline.dll can supply: the real service name set the MMS layer expects, and the exact `url`/`client` value format (full https URL? path? token?) — the live manifest is downloaded from the (now-questionable) production backend, so capture = T3 (docs/06) or a fireline.dll string pass.

## Status at shelve

fireline.dll (503,296 B) = the wire-spec target; drop owed from Alex ("i'll get it to her later", 10-19 16:48). Until it lands: the protocol surface is mapped end-to-end EXCEPT the transport internals (endpoints/headers/auth) and the ordinal→name map.
