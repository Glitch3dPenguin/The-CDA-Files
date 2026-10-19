// cpu_ref.c v4 — full-payload round-trip: the definitive correctness test.
// Decrypt the whole payload with the DECRYPT driver's struct (wrapper output),
// then re-encrypt with the ENCRYPT driver's struct (builder output). If the
// result equals the original ciphertext, the decrypt is provably correct
// (the encrypt driver is the decrypt driver's exact inverse in the game).
// Also: single-block determinism + struct-mutation + global-state probes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ucontext.h>
#include <sys/reg.h>
static char *B;
static void segh(int s, siginfo_t *i, void *uc) {
    ucontext_t *c = (ucontext_t *)uc;
    long long *g = (long long *)c->uc_mcontext.gregs;
    unsigned long long rip = (unsigned long long)g[16];
    fprintf(stderr, "SIGSEGV si_addr=%p rip=0x%llx\n", i->si_addr, rip);
    if (B && rip >= (unsigned long long)B && rip < (unsigned long long)B + 0x2400000UL)
        fprintf(stderr, "  RIP is GAME CODE @ image-relative 0x%llx (fileoff 0x%llx)\n",
                rip - (unsigned long long)B, rip - (unsigned long long)B + 0x140000000ULL - 0x140000000ULL);
    fprintf(stderr, "  rcx=0x%llx rdx=0x%llx r8=0x%llx r9=0x%llx rsp=0x%llx rax=0x%llx r10=0x%llx r11=0x%llx rbx=0x%llx\n",
            (unsigned long long)g[14], (unsigned long long)g[12],
            (unsigned long long)g[0], (unsigned long long)g[1],
            (unsigned long long)g[15], (unsigned long long)g[13],
            (unsigned long long)g[18], (unsigned long long)g[19], (unsigned long long)g[11]);
    fprintf(stderr, "  stack @rsp: ");
    for (int k = 0; k < 12; k++)
        fprintf(stderr, "%016llx ", (unsigned long long)g[15 + k]);
    fprintf(stderr, "\n");
    _exit(139);
}
#define IB 0x140000000UL
static void hex16(const unsigned char *p, int n){ for(int i=0;i<n;i++) printf("%02x ",p[i]); printf("\n"); }
static unsigned long long game_call(unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long);
/* Windows-ABI call via game_call.S thunk: the game's MSVC prologues spill
   rbx/rbp/rsi/rdi into the caller's 32-byte shadow area (legal under MSVC,
   fatal for GCC frames that keep live slots there). The thunk's own frame
   absorbs the clobber window. */
#define WIN_CALL_RAX(addr, a0, a1, a2, a3) game_call((unsigned long long)(addr), (unsigned long long)(a0), (unsigned long long)(a1), (unsigned long long)(a2), (unsigned long long)(a3))

static unsigned long long *canary;
static int canary_ok(const char *tag) {
    for (int i = 0; i < 64; i++)
        if (canary[i] != 0xC0FFEE0000000000ULL + i) {
            fprintf(stderr, "CANARY FAIL after %s @i=%d val=0x%llx\n", tag, i, (unsigned long long)canary[i]);
            return 0;
        }
    return 1;
}
int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: cpu_ref <exe> <payload_u>\n"); return 2; }
    setvbuf(stdout, 0, _IONBF, 0);
    struct sigaction sa; memset(&sa,0,sizeof sa); sa.sa_sigaction=segh; sa.sa_flags=SA_SIGINFO; sigaction(SIGSEGV,&sa,0);
    int fd = open(argv[1], O_RDONLY); if (fd<0){perror("open exe");return 2;}
    unsigned char *f = mmap(0,0x2500000,PROT_READ,MAP_PRIVATE,fd,0); if(f==MAP_FAILED){perror("mmap exe");return 2;}
    size_t sz = 0x2400000;
    B = mmap(0,sz,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0); if(B==MAP_FAILED){perror("mmap img");return 2;}
    memcpy(B+(0x140001000UL-IB), f+0x400, 0x01685829);
    memcpy(B+(0x141687000UL-IB), f+0x1685e00, 0x009635bc);
    memcpy(B+(0x141FEB000UL-IB), f+0x1fe9400, 0x000e3400);
    memcpy(B+(0x142264000UL-IB), f+0x20cc800, 0x00138e7c);
    unsigned long long *kg = (unsigned long long*)(B+(0x141FEBBA8UL-IB));
    *kg = (unsigned long long)(B+(0x1416B10C0UL-IB));
    canary = (unsigned long long *)B;
    for (int i = 0; i < 64; i++) canary[i] = 0xC0FFEE0000000000ULL + i;

    int ufd = open(argv[2], O_RDONLY); if (ufd<0){perror("open u");return 2;}
    struct stat st; fstat(ufd,&st);
    long filepay = st.st_size - 169;           // 30595
    long paylen  = (filepay / 16) * 16;        // 30592 = 1912 blocks
    unsigned char orig[32768];
    pread(ufd, orig, filepay, 169);
    unsigned char *key = (unsigned char*)(B+(0x1416B10C0UL-IB));
    printf("key=%.*s  filepay=%ld paylen=%ld blocks=%ld\n", 32, key, filepay, paylen, paylen/16);

    unsigned char *sp_swap  = (unsigned char*)(B+(0x141FEB000UL-IB))+0x1000;
    unsigned char *sp_plain = (unsigned char*)(B+(0x141FEB000UL-IB))+0x2000;
    unsigned char *sp_probe = (unsigned char*)(B+(0x141FEB000UL-IB))+0x3000;
    memset(sp_swap,0,240); memset(sp_plain,0,240);
    int Np = (int)WIN_CALL_RAX(B+(0x1405CEE40UL-IB), sp_plain, key, 0x100, 0);
    int Ns = (int)WIN_CALL_RAX(B+(0x1405CEC30UL-IB), sp_swap,  key, 0x100, 0);
    printf("N_plain=%d N_swap=%d\n", Np, Ns);
    fflush(stdout); { void *q = malloc(0x100000); memset(q, 1, 0x100000); free(q); } canary_ok("two builds");

    // ---- probe: does ONE round call mutate the struct? ----
    unsigned char snap[240]; memcpy(snap, sp_swap, 240);
    unsigned char t[16]; memcpy(t, orig, 16);
    WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp_swap, (unsigned long long)(unsigned int)Ns, t, t);
    canary_ok("round1"); printf("round fn mutates swap-struct: %s\n", memcmp(sp_swap,snap,240)==0?"NO":"YES");
    if (memcmp(sp_swap,snap,240)!=0){
        for (int s=0;s<15;s++) if (memcmp(sp_swap+s*16,snap+s*16,16)) printf("  slot%02d changed\n", s);
        memcpy(sp_swap, snap, 240); // restore
    }
    // ---- probe: single-block determinism (fresh struct each time) ----
    unsigned char *sp_a = (unsigned char*)(B+(0x142000000UL-IB))+0x4000; // scratch .data BSS, not near glibc heap
    unsigned char *sp_b = (unsigned char*)(B+(0x142000000UL-IB))+0x5000;
    unsigned char d1[16], d2[16];
    memset(sp_a,0,240); memset(sp_b,0,240);
    fprintf(stderr, "P: build sp_a\n");
    WIN_CALL_RAX(B+(0x1405CEC30UL-IB), sp_a, key, 0x100, 0);
    fprintf(stderr, "P: build sp_b\n");
    WIN_CALL_RAX(B+(0x1405CEC30UL-IB), sp_b, key, 0x100, 0);
    memcpy(d1, orig, 16); memcpy(d2, orig, 16);
    fprintf(stderr, "P: round a\n");
    WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp_a, (unsigned long long)(unsigned int)Ns, d1, d1);
    fprintf(stderr, "P: round b (sp_b=%p Ns=%d)\n", (void*)sp_b, Ns);
    WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp_b, (unsigned long long)(unsigned int)Ns, d2, d2);
    printf("single-block deterministic (two fresh structs): %s\n", memcmp(d1,d2,16)==0?"YES":"NO");
    printf("  d1=%02x %02x %02x %02x ...\n  d2=%02x %02x %02x %02x ...\n",
        d1[0],d1[1],d1[2],d1[3], d2[0],d2[1],d2[2],d2[3]);

    // ---- FULL round-trip: decrypt(swap) then encrypt(plain) over the whole payload ----
    unsigned char P[32768]; memcpy(P, orig, paylen);
    for (long b=0; b<paylen/16; b++) {
        unsigned char *blk = P + b*16;
        WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp_swap, (unsigned long long)(unsigned int)Ns, blk, blk);
    }
    printf("PLAINTEXT first16: "); hex16(P,16);
    int pr=0; for(long i=0;i<paylen;i++) if(32<=P[i]&&P[i]<127) pr++;
    printf("plaintext printable: %.3f\n", (double)pr/paylen);

    unsigned char C[32768]; memcpy(C, P, paylen);
    for (long b=0; b<paylen/16; b++) {
        unsigned char *blk = C + b*16;
        WIN_CALL_RAX(B+(0x1405CDB50UL-IB), sp_plain, (unsigned long long)(unsigned int)Np, blk, blk);
    }
    printf("FULL round-trip (dec swap -> enc plain): %s\n", memcmp(C, orig, paylen)==0?"MATCH":"FAIL");
    if (memcmp(C, orig, paylen)!=0){
        long first=-1; for(long i=0;i<paylen;i++) if(C[i]!=orig[i]){first=i;break;}
        printf("  first diff @ %ld (0x%lx): ct=%02x got=%02x\n", first, first, orig[first], C[first]);
        printf("  C  first16: "); hex16(C,16);
    }

    // save the plaintext
    FILE *of = fopen("workspace_temp/cda/work/dec169_real.bin","wb");
    fwrite(P,1,paylen,of); fwrite(orig+paylen,1,filepay-paylen,of); fclose(of);
    printf("saved dec169_real.bin (%ld + %ld tail = %ld)\n", paylen, filepay-paylen, filepay);
    return 0;
}
