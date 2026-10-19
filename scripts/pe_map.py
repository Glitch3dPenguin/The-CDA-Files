import struct
P = 'workspace_temp/cda/ext/Binaries/Win64/ShooterGame-Win32-Shipping.exe'
f = open(P, 'rb').read()
e = struct.unpack_from('<I', f, 0x3C)[0]
assert f[e:e+4] == b'PE\x00\x00', f[e:e+4]
nsec = struct.unpack_from('<H', f, e+6)[0]
sopt = struct.unpack_from('<H', f, e+20)[0]
secs = []
o = e + 24 + sopt
for i in range(nsec):
    name = f[o:o+8].rstrip(b'\0').decode('latin1')
    vsize, vaddr, rawsize, rawptr = struct.unpack_from('<IIII', f, o+8)
    secs.append((name, vaddr, vsize, rawptr, rawsize))
    o += 40
ibase = struct.unpack_from('<Q', f, e+24+24)[0]  # PE32+ ImageBase (section VA fields are RVAs)
secs = [(n, ibase + v, vs, rp, rs) for n, v, vs, rp, rs in secs]
print('e_lfanew=%#x  nsec=%d  sopt=%d  imagebase=%#x' % (e, nsec, sopt, ibase))
for n, v, vs, rp, rs in secs:
    tag = '  [identity: rawptr == vaddr]' if v == rp else ''
    print('%-12s VA=%#012x vsize=%#010x rawptr=%#012x rawsize=%#010x%s' % (n, v, vs, rp, rs, tag))

def va2off(va):
    for n, v, vs, rp, rs in secs:
        if v <= va < v + max(vs, rs):
            return rp + (va - v), n
    return None, None

tests = [
    (0x1405CC7E3, 'known reader instr'),
    (0x141FEBBA8, 'key-pointer global'),
    (0x140604560, 'tag fn (FulfillCompressedRead)'),
    (0x1416B10C0, '32B ascii key'),
    (0x140618300, 'image key value (code?)'),
    (0x14060CFC0, 'tag fn #2'),
]
for va, label in tests:
    off, n = va2off(va)
    print('%-32s VA=%#012x -> file=%#012x  sec=%s  first8=%s' % (label, va, off, n, f[off:off+8].hex()))

off, n = va2off(0x141FEBBA8)
val = struct.unpack_from('<Q', f, off)[0]
print('key global image value = %#x (sec %s)' % (val, va2off(val)[1]))
if 0x140000000 <= val < 0x142200000:
    off2, n2 = va2off(val)
    print('32B at image key value: %s' % f[off2:off2+32].hex())
    print('  printable: %r' % bytes(b for b in f[off2:off2+32] if 32 <= b < 127))
