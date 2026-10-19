// cpu_ref3.c — double-decrypt test: is the .u payload double-encrypted?
// pass1 = the proven decrypt (swap struct, round fn 0x1405CCAA0, N from wrapper).
// pass2 = the same transform applied again to pass1's output.
// If pass2 shows structure (names / 0f0e) and pass1 doesn't -> double encryption.
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
    fprintf(stderr, "SIGSEGV si_addr=%p rip=0x%llx rcx=0x%llx rdx=0x%llx\n", i->si_addr,
            (unsigned long long)g[16], (unsigned long long)g[14], (unsigned long long)g[12]);
    _exit(139);
}
#define IB 0x140000000UL
static unsigned long long game_call(unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long);
#define WIN_CALL_RAX(addr, a0, a1, a2, a3) game_call((unsigned long long)(addr), (unsigned long long)(a0), (unsigned long long)(a1), (unsigned long long)(a2), (unsigned long long)(a3))
static void dump_names(const unsigned char *p, long n) {
    long hits = 0;
    for (long i = 0; i + 8 < n && hits < 20; i++) {
        unsigned ln = (unsigned)(p[i] | p[i+1]<<8 | p[i+2]<<16 | p[i+3]<<24);
        if (ln >= 4 && ln <= 48 && i + 4 + ln < n && p[i+4+ln-1] == 0) {
            int ok = 1;
            for (unsigned k = 0; k < ln-1; k++) if (!(32 <= p[i+4+k] && p[i+4+k] < 127)) ok = 0;
            if (ok) { printf("  name @%ld len=%u: ", i, ln-1); fwrite(p+i+4, 1, ln-1, stdout); printf("\n"); hits++; }
        }
    }
    if (!hits) printf("  (no inline names)\n");
}
static void head16(const unsigned char *p) {
    for (int i = 0; i < 16; i++) printf("%02x ", p[i]);
    printf("\n");
}
int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: cpu_ref3 <exe> <u>\n"); return 2; }
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
    unsigned char *keyg = (unsigned char*)(B+(0x1416B10C0UL-IB));
    unsigned long long *kg = (unsigned long long*)(B+(0x141FEBBA8UL-IB));
    *kg = (unsigned long long)keyg;
    int ufd = open(argv[2], O_RDONLY); if (ufd<0){perror("open u");return 2;}
    struct stat st; fstat(ufd,&st);
    long filepay = st.st_size - 169;
    long paylen  = (filepay / 16) * 16;
    unsigned char P[32768];
    pread(ufd, P, filepay, 169);
    unsigned char *sp = (unsigned char*)(B+(0x141FEB000UL-IB))+0x1000;
    memset(sp, 0, 240);
    int Ns = (int)WIN_CALL_RAX(B+(0x1405CEC30UL-IB), sp, (unsigned long long)keyg, 0x100, 0);
    printf("N_swap=%d filepay=%ld paylen=%ld\n", Ns, filepay, paylen);
    fflush(stdout);
    for (long b=0; b<paylen/16; b++) WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp, (unsigned long long)(unsigned int)Ns, P+b*16, P+b*16);
    int pr=0; for(long i=0;i<paylen;i++) if(32<=P[i]&&P[i]<127) pr++;
    printf("pass1 first16: "); head16(P);
    printf("pass1 printable: %.3f\n", (double)pr/paylen);
    unsigned char Q[32768]; memcpy(Q, P, paylen);
    for (long b=0; b<paylen/16; b++) WIN_CALL_RAX(B+(0x1405CCAA0UL-IB), sp, (unsigned long long)(unsigned int)Ns, Q+b*16, Q+b*16);
    int pr2=0; for(long i=0;i<paylen;i++) if(32<=Q[i]&&Q[i]<127) pr2++;
    printf("pass2 first16: "); head16(Q);
    printf("pass2 printable: %.3f\n", (double)pr2/paylen);
    int f5 = (Q[0]==0x0f && Q[1]==0x0e && Q[2]==0 && Q[3]==0 && Q[4]==0);
    int f5p = (P[0]==0x0f && P[1]==0x0e && P[2]==0 && P[3]==0 && P[4]==0);
    printf("pass1 0f0e-head: %s  pass2 0f0e-head: %s\n", f5p?"YES":"no", f5?"YES":"no");
    printf("pass1 names:\n"); dump_names(P, paylen);
    printf("pass2 names:\n"); dump_names(Q, paylen);
    FILE *of = fopen("workspace_temp/cda/work/dec169_pass2.bin","wb");
    fwrite(Q,1,paylen,of); fwrite(P+paylen,1,filepay-paylen,of); fclose(of);
    printf("saved dec169_pass2.bin\n");
    return 0;
}
