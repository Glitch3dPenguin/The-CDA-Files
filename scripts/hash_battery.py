#!/usr/bin/env python3
# hash_battery.py — calibrate/verify the .u descriptor hash 35890 (0x8C42)
import zlib, struct

def crc16(b, init, poly):
    crc = init
    for byte in b:
        crc ^= byte << 8
        for _ in range(8):
            crc = ((crc << 1) ^ poly) & 0xFFFF if crc & 0x8000 else (crc << 1) & 0xFFFF
    return crc

def sums(b):
    x = 0
    for c in b: x ^= c
    n16 = (len(b)//2)*2
    n32 = (len(b)//4)*4
    w16 = sum(struct.unpack('<%dH' % (n16//2), b[:n16])) if n16 else 0
    w32 = sum(struct.unpack('<%dI' % (n32//4), b[:n32])) if n32 else 0
    return sum(b) % 65536, x, w16 % 65536, w32 % 65536

def battery(b, name):
    s, x, w16, w32 = sums(b)
    c32 = zlib.crc32(b) & 0xFFFFFFFF
    ad = zlib.adler32(b) & 0xFFFFFFFF
    r = dict(sum16=s, xor=x, sum16w=w16, sum32w=w32,
             crc32lo=c32 & 0xFFFF, crc32hi=c32 >> 16,
             crc16ccittF=crc16(b, 0xFFFF, 0x1021), crc16ccitt0=crc16(b, 0, 0x1021),
             crc16ibm=crc16(b, 0, 0x8005), crc16usb=crc16(b, 0xFFFF, 0x8005),
             adlerlo=ad & 0xFFFF, adlerhi=ad >> 16, len16=len(b) % 65536)
    hit = [k for k, v in r.items() if v == 35890]
    print(f"{name:18s} " + " ".join(f"{k}={v}" for k, v in r.items()) + (f"  *** HIT {hit}" if hit else ""))

dec = open('workspace_temp/cda/work/dec169_real.bin', 'rb').read()
u   = open('workspace_temp/cda/ext/ShooterGame/CookedPC/SDPCEngine.u', 'rb').read()
upk = open('workspace_temp/cda/ext/ShooterGame/CookedPC/AkAudio_LOC_INT.upk', 'rb').read()

print("TARGET (the .u descriptor hash) = 35890 (0x8C42)")
battery(dec[:30592], 'dec_30592')
battery(dec, 'dec_30595')
battery(u[169:], 'ct_payload')
battery(u, 'ct_whole')
battery(upk[169:], 'upk_payload')
battery(upk, 'upk_whole')
battery(upk[648:26601], 'upk_regions_648_26601')
