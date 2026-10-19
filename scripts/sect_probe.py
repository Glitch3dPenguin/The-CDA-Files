#!/usr/bin/env python3
# sect_probe.py — discriminate the (a,b) table model: dump section boundaries in the
# plaintext upk (Rosetta stone) + candidate regions in the decrypted .u payload
import math

u = open('workspace_temp/cda/ext/ShooterGame/CookedPC/SDPCEngine.u', 'rb').read()
upk = open('workspace_temp/cda/ext/ShooterGame/CookedPC/AkAudio_LOC_INT.upk', 'rb').read()
dec = open('workspace_temp/cda/work/dec169_real.bin', 'rb').read()  # 30595B = dec[30592] + tail 3

def ent(b):
    if not b:
        return 0.0
    from collections import Counter
    c = Counter(b)
    n = len(b)
    return -sum(v/n * math.log2(v/n) for v in c.values())

def show(name, data, off, n=24):
    seg = data[off:off+n]
    asc = ''.join(chr(b) if 32 <= b < 127 else '.' for b in seg)
    hexs = ' '.join('%02x' % b for b in seg[:16])
    print('  %s @%d (e=%.2f): %s | %s' % (name, off, ent(seg), hexs, asc))

print('=== 1. UPK (plaintext) section boundaries per the table ===')
# entries (a,b): (648,123),(0,457),(0,75),(0,3736),(0,6),(0,3568),(0,8844),(9144,0),(0,0)x5
bounds = []
a, b0 = 648, 123
bounds.append((648, 771))
cur = 771
for (a, bl) in [(0,457),(0,75),(0,3736),(0,6),(0,3568),(0,8844)]:
    bounds.append((cur, cur+bl))
    cur += bl
print('  model A (a=start,b=len, cumulative):', bounds)
for s, e in bounds:
    print('   [%d,%d) len=%d e=%.2f' % (s, e, e-s, ent(upk[s:e])))
    show('upk', upk, s, 24)

print()
print('=== 2. decrypted .u payload — candidate windows ===')
show('dec', dec, 0, 32)
show('dec', dec, 83, 16)
show('dec', dec, 129, 16)
show('dec', dec, 193, 16)
show('dec', dec, 6984, 16)
show('dec', dec, 9308, 16)
show('dec', dec, 22432, 16)
print('  e0 candidate (start=27304 abs,len=224) -> dec[27135:27359): e=%.3f' % ent(dec[27135:27359]))
show('dec', dec, 27135, 32)
print('  e0 candidate (len=27304,start=224 abs) -> dec[55:27359): e=%.3f' % ent(dec[55:27359]))
show('dec', dec, 55, 32)
print('  dec[0:23204): e=%.3f   dec[23204:]: e=%.3f' % (ent(dec[:23204]), ent(dec[23204:])))
show('dec', dec, 23204, 32)
# printable census per 16KB block
print('  block entropy (16KB):')
for i in range(0, 30592, 16384):
    print('    dec[%d:%d): e=%.3f' % (i, i+16384, ent(dec[i:i+16384])))
# ascii runs >= 6 in the whole dec payload
runs = []
i = 0
n = len(dec)
while i < n:
    j = i
    while j < n and 32 <= dec[j] < 127:
        j += 1
    if j - i >= 6:
        runs.append((i, dec[i:j][:40]))
    i = max(j, i+1)
print('  ascii-runs >= 6 in dec payload: %d' % len(runs))
for i, s in runs[:40]:
    print('    @%d: %r' % (i, s))
print('done')
