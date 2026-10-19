// cpu_ref2.c v5 — direction/struct matrix: which (round fn, struct) pair yields UE3 structure?
// A: round 0x1405CCAA0 + swap struct   (= game decrypt driver path, control)
// B: round 0x1405CCAA0 + plain struct
// C: round 0x1405CDB50 + plain struct  (= game encrypt driver path)
// D: round 0x1405CDB50 + swap struct
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
static void *game_call(void *, void *, void *, void *, void *);
static void hex16(const unsigned char *p, int n){ for(int i=0;i<n;i++) printf("%02x ",p[i]); printf("\n"); }

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: cpu_ref2 <exe> <payload_u>\n"); return 2; }
    setvbuf(stdout, 0, _IONBF, 0);
    unsigned char *f = mmap(0,0x2500000,PROT_READ,MAP_PRIVATE,open(argv[1],O_RDONLY),0);
    if(f==MAP_FAILED){perror("mmap");return 2;}
    static char *B;
    #define IB 0x140000000UL
    B = mmap(0, 0x2400000, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (B == MAP_FAILED) { perror("mmap img"); return 2; }
    memcpy(B+(0x140001000UL-IB), f+0x400, 0x01685829);
    memcpy(B+(0x141687000UL-IB), f+0x1685e00, 0x009635bc);
    memcpy(B+(0x141FEB000UL-IB), f+0x1fe9400, 0x000e3400);
    memcpy(B+(0x142264000UL-IB), f+0x20cc800, 0x00138e7c);
    *(unsigned long long*)(B+(0x141FEBBA8UL-IB)) = (unsigned long long)(B+(0x1416B10C0UL-IB));

    struct stat st; fstat(open(argv[2],O_RDONLY),&st);
    long filepay = st.st_size - 169;
    long paylen  = (filepay / 16) * 16;
    unsigned char orig[32768];
    int ufd = open(argv[2],O_RDONLY);
    pread(ufd, orig, filepay, 169);
    unsigned char *key = (unsigned char*)(B+(0x1416B10C0UL-IB));

    unsigned char *sp_plain = (unsigned char*)(B+(0x141FEB000UL-IB))+0x2000;
    unsigned char *sp_swap  = (unsigned char*)(B+(0x141FEB000UL-IB))+0x1000;
    memset(sp_swap,0,240); memset(sp_plain,0,240);
    int Np = (int)(unsigned long long)game_call(B+(0x1405CEE40UL-IB), sp_plain, key, (void*)0x100, 0);
    int Ns = (int)(unsigned long long)game_call(B+(0x1405CEC30UL-IB), sp_swap,  key, (void*)0x100, 0);
    printf("N_plain=%d N_swap=%d\n", Np, Ns);

    struct { const char *name; char *rn; unsigned char *sp; int n; } V[4] = {
        {"A dec-round+swap ", B+(0x1405CCAA0UL-IB), sp_swap,  Ns},
        {"B dec-round+plain", B+(0x1405CCAA0UL-IB), sp_plain, Np},
        {"C enc-round+plain", B+(0x1405CDB50UL-IB), sp_plain, Np},
        {"D enc-round+swap ", B+(0x1405CDB50UL-IB), sp_swap,  Ns},
    };
    unsigned char P[32768];
    for (int v = 0; v < 4; v++) {
        memcpy(P, orig, paylen);
        for (long b = 0; b < paylen/16; b++)
            game_call(V[v].rn, V[v].sp, (void*)(unsigned long long)(unsigned int)V[v].n, P+b*16, P+b*16);
        // stats
        static int freq[256]; memset(freq,0,sizeof freq);
        for (long i=0;i<paylen;i++) freq[P[i]]++;
        double ent=0; for(int i=0;i<256;i++) if(freq[i]){double p=freq[i]/(double)paylen; ent-=p*log2(p);}
        int pr=0; for(long i=0;i<paylen;i++) if(32<=P[i]&&P[i]<127) pr++;
        printf("%s head16: ", V[v].name); hex16(P,16);
        printf("%s entropy=%.4f printable=%.3f 0f0e=%s\n", V[v].name, ent, (double)pr/paylen,
               (P[0]==0x0f&&P[1]==0x0e&&P[2]==0&&P[3]==0&&P[4]==0)?"YES":"no");
        // name scans
        int found=0;
        const char *names[] = {"ArrayProperty","StructProperty","ObjectProperty","ByteProperty","IntProperty","None","SDPCEngine"};
        for (unsigned ni=0; ni<7; ni++) {
            int a=0, u=0;
            for (long i=0;i+8<paylen;i++) {
                if (!a && !memcmp(P+i, names[ni], strlen(names[ni])) && a==0 && (i==0||P[i-1]==0)) { a=i; }
                if (!u) {
                    int ok=1; for(size_t c=0;c<strlen(names[ni]);c++) if(P[i+2*c]!=(names[ni][c]&0xff)||P[i+2*c+1]!=0){ok=0;break;}
                    if (ok) u=i;
                }
                if (a&&u) break;
            }
            if (a||u) { printf("%s   name %-16s ascii@%d u16le@%d\n", V[v].name, names[ni], a, u); found=1; }
        }
        if (!found) printf("%s   no UE3 names\n", V[v].name);
        char fn[128]; snprintf(fn,sizeof fn,"workspace_temp/cda/work/v5_%c.bin", 'A'+v);
        FILE *of=fopen(fn,"wb"); fwrite(P,1,paylen,of); fwrite(orig+paylen,1,filepay-paylen,of); fclose(of);
    }
    return 0;
}
