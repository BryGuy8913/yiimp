#include "sk1024.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sk1024/skein.h"
//#include "../LLC/hash/SK.h"
#include "sk1024/uint1024.h"


//if sk1024 runs as C++ and not C, tell the compiler explicitly that KeccakHash is C
extern "C" {
    #include "sk1024/KeccakHash.h"
}


#include "common.h"

void sk1024_hash(const char* input, char* output, uint32_t len)
{
    static unsigned char pblank[1];

    uint1024 skein;
    Skein1024_Ctxt_t ctx;
    Skein1024_Init(&ctx, 1024);
    //Skein1024_Update(&ctx, (pbegin == pend ? pblank : (unsigned char*)&pbegin[0]), (pend - pbegin) * sizeof(pbegin[0]));
    Skein1024_Update(&ctx, (input == output ? pblank : (uint8_t*)&input[0]), (output - input) * sizeof(input[0]));
    Skein1024_Final(&ctx, (unsigned char *)&skein);

    uint1024 keccak;
    Keccak_HashInstance ctx_keccak;
    Keccak_HashInitialize(&ctx_keccak, 576, 1024, 1024, 0x05);
    Keccak_HashUpdate(&ctx_keccak, (unsigned char *)&skein, 1024);
    Keccak_HashFinal(&ctx_keccak, (unsigned char *)&keccak);

    //return keccak;
    memcpy(output, (unsigned char *)&keccak, sizeof(output));
}

/*
void bcd_hash(const char* input, char* output, uint32_t len)
{
   
    sph_skein512_context     ctx_skein;
    sph_keccak512_context    ctx_keccak;

    uint32_t hashA[16], hashB[16];
    
    sph_skein512_init(&ctx_skein);
    //int Skein_512_Update(Skein_512_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
    sph_skein512 (&ctx_skein, hashA, 64);
    sph_skein512_close (&ctx_skein, hashB);

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, hashA, 64);
    sph_keccak512_close(&ctx_keccak, hashB);
  
    memcpy(output, hashA, 32);

}
*/

