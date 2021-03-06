/*
 * SHA-256
 */

#include <stdint.h>
#include <string.h>

#include "configuration.c"
#include "byteaccess.c"


typedef struct {
	uint32_t total[2];			/*!< number of bytes processed  */
	uint32_t state[8];			/*!< intermediate digest state  */
	unsigned char buffer[64];	/*!< data block being processed */
} sha256_context;

typedef sha256_context sha224_context;


static void sha256x_init(sha256_context *ctx) {
	memset(ctx, 0, sizeof(sha256_context));
	ctx->total[0] = 0;
	ctx->total[1] = 0;
}

#if (!DISABLE_SHA256)
API_MODIFIER void sha256_init(sha256_context *ctx) {
	sha256x_init(ctx);
	/* SHA-256 */
	ctx->state[0] = 0x6A09E667;
	ctx->state[1] = 0xBB67AE85;
	ctx->state[2] = 0x3C6EF372;
	ctx->state[3] = 0xA54FF53A;
	ctx->state[4] = 0x510E527F;
	ctx->state[5] = 0x9B05688C;
	ctx->state[6] = 0x1F83D9AB;
	ctx->state[7] = 0x5BE0CD19;
}
#endif	/* (!DISABLE_SHA256) */

#if (!DISABLE_SHA224)
API_MODIFIER void sha224_init(sha224_context *ctx) {
	sha256x_init(ctx);
	/* SHA-224 */
	ctx->state[0] = 0xC1059ED8;
	ctx->state[1] = 0x367CD507;
	ctx->state[2] = 0x3070DD17;
	ctx->state[3] = 0xF70E5939;
	ctx->state[4] = 0xFFC00B31;
	ctx->state[5] = 0x68581511;
	ctx->state[6] = 0x64F98FA7;
	ctx->state[7] = 0xBEFA4FA4;
}
#endif	/* (!DISABLE_SHA224) */

static const uint32_t K[] = {
	0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
	0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
	0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
	0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
	0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
	0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
	0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
	0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
	0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
	0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
	0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
	0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
	0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
	0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
	0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
	0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};


#define SHR(x, n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x, n) (SHR(x, n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) ((x & y) | (z & (x | y)))
#define F1(x, y, z) (z ^ (x & (y ^ z)))


/*
 * data is a pointer which reference to a memory block consists at least 64 elements
 */
static void sha256x_process(sha256_context *ctx, const unsigned char * data) {
	uint32_t W[64];
	uint32_t A[8];

	for(int i = 0; i < 8; i++) {
		A[i] = ctx->state[i];
	}

	for(int i = 0; i < 16; i++) {
		GET_UINT32_BE(W[i], data, 4 * i);
	}

#define R(t) (W[t] =						\
				S1(W[t - 2]) + W[t - 7] +	\
				S0(W[t - 15]) + W[t - 16])

#define P(a, b, c, d, e, f, g, h, x, K) {		\
	uint32_t temp1, temp2;						\
	temp1 = h + S3(e) + F1(e, f, g) + K + x;	\
	temp2 = S2(a) + F0(a, b, c);				\
	d += temp1;									\
	h = temp1 + temp2;							\
}

	for(int i = 0; i < 16; i += 8) {
		P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i + 0], K[i + 0]);
		P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i + 1], K[i + 1]);
		P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i + 2], K[i + 2]);
		P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i + 3], K[i + 3]);
		P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i + 4], K[i + 4]);
		P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i + 5], K[i + 5]);
		P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i + 6], K[i + 6]);
		P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i + 7], K[i + 7]);
	}

	for(int i = 16; i < 64; i += 8) {
		P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], R(i + 0), K[i + 0]);
		P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], R(i + 1), K[i + 1]);
		P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], R(i + 2), K[i + 2]);
		P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], R(i + 3), K[i + 3]);
		P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], R(i + 4), K[i + 4]);
		P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], R(i + 5), K[i + 5]);
		P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], R(i + 6), K[i + 6]);
		P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], R(i + 7), K[i + 7]);
	}

#undef P
#undef R

	for(int i = 0; i < 8; i++) {
		ctx->state[i] += A[i];
	}
	return;
}


#undef F1
#undef F0
#undef S3
#undef S2
#undef S1
#undef S0
#undef ROTR
#undef SHR


API_MODIFIER void sha256_update(sha256_context *ctx, const void *input, size_t ilen) {
	size_t fill;
	uint32_t left;

	if( ilen == 0 ) {
		return;
	}

	left = ctx->total[0] & 0x3F;
	fill = 64 - left;

	ctx->total[0] += (uint32_t)(ilen);
	ctx->total[0] &= 0xFFFFFFFF;

	if(ctx->total[0] < (uint32_t)(ilen)) {
		ctx->total[1]++;
	}

	if(left && (ilen >= fill)) {
		memcpy((void *)(ctx->buffer + left), input, fill);
		sha256x_process(ctx, (const unsigned char *)(ctx->buffer));
		input = (unsigned char *)(input) + fill;
		ilen -= fill;
		left = 0;
	}

	while(ilen >= 64) {
		sha256x_process(ctx, (const unsigned char *)(input));
		input = (unsigned char *)(input) + 64;
		ilen -= 64;
	}

	if(ilen > 0) {
		memcpy((void *)(ctx->buffer + left), input, ilen );
	}
}

#if (!DISABLE_SHA224)
API_MODIFIER void sha224_update(sha224_context *ctx, const void *input, size_t ilen) {
	sha256_update((sha256_context *)(ctx), input, ilen);
}
#endif	/* (!DISABLE_SHA224) */

static const unsigned char sha256x_padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


static void sha256x_finish(sha256_context *ctx) {
	uint32_t last, padn;
	uint32_t high, low;
	unsigned char msglen[8];

	high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
	low  = (ctx->total[0] << 3);

	PUT_UINT32_BE(high, msglen, 0);
	PUT_UINT32_BE(low,  msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	sha256_update(ctx, sha256x_padding, padn);
	sha256_update(ctx, msglen, 8);
}

#if (!DISABLE_SHA256)
API_MODIFIER void sha256_finish(sha256_context *ctx, unsigned char output[32]) {
	sha256x_finish(ctx);
	PUT_UINT32_BE(ctx->state[0], output,  0);
	PUT_UINT32_BE(ctx->state[1], output,  4);
	PUT_UINT32_BE(ctx->state[2], output,  8);
	PUT_UINT32_BE(ctx->state[3], output, 12);
	PUT_UINT32_BE(ctx->state[4], output, 16);
	PUT_UINT32_BE(ctx->state[5], output, 20);
	PUT_UINT32_BE(ctx->state[6], output, 24);
	PUT_UINT32_BE(ctx->state[7], output, 28);
}
#endif	/* (!DISABLE_SHA256) */

#if (!DISABLE_SHA224)
API_MODIFIER void sha224_finish(sha224_context *ctx, unsigned char output[28]) {
	sha256x_finish((sha256_context *)(ctx));
	PUT_UINT32_BE(ctx->state[0], output,  0);
	PUT_UINT32_BE(ctx->state[1], output,  4);
	PUT_UINT32_BE(ctx->state[2], output,  8);
	PUT_UINT32_BE(ctx->state[3], output, 12);
	PUT_UINT32_BE(ctx->state[4], output, 16);
	PUT_UINT32_BE(ctx->state[5], output, 20);
	PUT_UINT32_BE(ctx->state[6], output, 24);
}
#endif	/* (!DISABLE_SHA224) */



// vim: ts=4 sw=4 ai nowrap
