/*
 * macros to access integers from byte array
 */

#include <stdint.h>
#include <arpa/inet.h>

#ifndef GET_UINT32_BE
/* GET_UINT32_BE(n, b, i): get uint32_t n from given byte array b with offset i
 * n: uint32_t to keep loaded result
 * b: pointer to byte array
 * i: offset from b to fetch uint32_t from
 */
#define GET_UINT32_BE(n, b, i)						\
	do {											\
		uint32_t aux_n = *((uint32_t *)(b + i));	\
		n = ntohl(aux_n);							\
	} while(0)
/*
	do {										\
		(n) = ( (uint32_t) (b)[(i) + 0] << 24 )	\
			| ( (uint32_t) (b)[(i) + 1] << 16 )	\
			| ( (uint32_t) (b)[(i) + 2] <<  8 )	\
			| ( (uint32_t) (b)[(i) + 3]);		\
	} while(0)
*/
#endif	/* GET_UINT32_BE */


#ifndef PUT_UINT32_BE
/* PUT_UINT32_BE(n, b, i): put uint32_t into given byte array b with offset i
 * n: uint32_t to push
 * b: pointer to byte array
 * i: offset from b to place uint32_t into
 */
#define PUT_UINT32_BE(n, b, i)			\
	do {								\
		uint32_t aux_n = htonl(n);		\
		*((uint32_t *)(b + i)) = aux_n;	\
	} while(0)
/*
	do {												\
		(b)[(i) + 0] = (unsigned char) ( (n) >> 24 );	\
		(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );	\
		(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );	\
		(b)[(i) + 3] = (unsigned char) ( (n)       );	\
	} while(0)
*/
#endif


