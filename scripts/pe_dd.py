import struct

def load(p):
    f = open(p, 'rb').read()
    e = struct.unpack_from('<I', f, 0x3C)[0]
    magic = struct.unpack_from('<H', f, e+24)[0]
    ibase = struct.unpack_from('<Q', f, e+24+24)[0] if magic == 0x20b else struct.unpack_from('<I', f, e+24+28)[0]
    nsec = struct.unpack_from('<H', f, e+6)[0]
    sopt = struct.unpack_from('<H', f, e+20)[0]
    secs = []
    o = e + 24 + sopt
    for i in range(nsec):
        name = f[o:o+8].rstrip(b'\0').decode('latin1')
        vsize, vaddr, rawsize, rawptr = struct.unpack_from('<IIII', f, o+8)
        secs.append((name, vaddr, vsize, rawptr, rawsize))
        o += 40
    secs = [(n, v if v >= 0x10000000 else ibase + v, vs, rp, rs) for n, v, vs, rp, rs in secs]
    return f, e, magic, ibase, secs

def off(secs, va):
    for n, v, vs, rp, rs in secs:
        if v <= va < v + max(vs, rs):
            return rp + (va - v), n
    return None, None

for p in ('workspace_temp/cda/ext/Binaries/Win64/ShooterGame-Win32-Shipping.exe',
          'workspace_temp/cda/ext/Binaries/Win64/EasyAntiCheat_x64.dll'):
    f, e, magic, ibase, secs = load(p)
    base = e + 24 + 112
    nr = struct.unpack_from('<I', f, base-4)[0]
    print('== %s ibase=%#x NRVA=%d' % (p.split('/')[-1], ibase, nr))
    for i in range(min(nr, 6)):
        r, s = struct.unpack_from('<II', f, base + 8*i)
        labels = {0: 'Export', 1: 'Import', 2: 'Resource', 3: 'Exception', 5: 'BaseReloc'}
        if not r:
            print('  DD[%d] %s: (empty)' % (i, labels.get(i, '?')))
            continue
        fo, sn = off(secs, ibase + r)
        print('  DD[%d] %-10s rva=%#x size=%#x file=%s (%s)' % (i, labels.get(i, '?'), r, s, hex(fo) if fo is not None else 'UNMAPPED', sn))
        if i == 1 and fo is not None:
            o = fo
            for k in range(3):
                ilt, ts, fc, name_rva, ilt2 = struct.unpack_from('<IIIII', f, o)
                if not (ilt or name_rva or ilt2):
                    print('    entry[%d]: null' % k)
                    break
                no, _ = off(secs, ibase + name_rva)
                end = f.find(b'\0', no) if no is not None else -1
                print('    entry[%d]: dll=%r ilt=%#x ilt2=%#x' % (k, f[no:end].decode('latin1'), ilt, ilt2))
                o += 20
