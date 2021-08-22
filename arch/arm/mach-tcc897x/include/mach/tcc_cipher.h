#ifndef _TCC_CIPHER_H_
#define _TCC_CIPHER_H_

enum {
	CIPHER_AES_ECB = 0,
	CIPHER_AES_CBC,
	MAX_CIPHER_AES_OPMODE
};// CIPHER_AES_OPMODE;

enum {
	CIPHER_SELECT_BYPASS = 0,
	CIPHER_SELECT_AES,
	CIPHER_SELECT_DES,
	CIPHER_SELECT_MULTI2,
	CIPHER_SELECT_CSA2,
	CIPHER_SELECT_CSA3,
	CIPHER_SELECT_HASH,
	MAX_CIPHER_SELECT
};// CIPHER_SELECT;

#define CIPHER_OTPKEY_FLAG      0x40000000
#define CIPHER_ADDFUNC_FLAG     0xF0000000  // (CIPHER_BYPASS_FLAG|CIPHER_OTPKEY_FLAG)

void	tcc_cipher_AES(unsigned int encode, unsigned int opmode, unsigned int size,
		unsigned int sbase, unsigned int dbase, unsigned int *key, unsigned int *iv);

#endif /*_TCC_CIPHER_H_*/
