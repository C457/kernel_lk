/*
 * Copyright (c) 2021, MOBIS.co.kr
 *
 * Signature Verification for UPDATE IMAGE
 */

#include <common.h>
#include <u-boot/sha256.h>
#include <u-boot/rsa.h>
#include <u-boot/rsa-mod-exp.h>

#include <mobis/verify_update_image.h>

#ifdef CONFIG_USB_UPDATE
#include <mach/sdmmc/emmc.h>
#include <usb.h>
#endif


#if defined(UPDATE_DEBUG_LOG_FBCON)
#define fbout(fmt, args...) fbprintf(fmt, ##args)
#else
#define fbout(fmt, args...) do {} while (0)
#endif

#define DEBUG_LOG
#ifdef DEBUG_LOG
#define update_debug(fmt, args...)      \
    do {                    \
        printf(fmt, ##args);        \
        fbout(fmt, ##args);     \
    } while (0)
#else
#define update_debug(fmt, args...) do {} while (0)
#endif

#define UPDATE_SIGNATURE_FILE_NAME "update.sig"

#define SIG_FILE_SIZE		256


int usb_fat_file_exists(const char *file)
{
	if (fs_set_blk_dev("usb", "0:1", 1))	// FS_TYPE_FAT
		return 0;

	return file_exists("usb", "0:1", file, 1);
}

unsigned long usb_fat_file_size(const char*file)
{
	loff_t size = 0;

	if (fs_set_blk_dev("usb", "0:1", 1))	// FS_TYPE_FAT
		return 0;

	if (fs_size(file, &size) < 0)
		return 0;
	return (unsigned long)size;
}

unsigned long usb_fat_read_with_filename(char *filename, unsigned long addr, unsigned long bytes, unsigned long pos)
{
	int ret = -1;
	loff_t len_read = 0;

	if (fs_set_blk_dev("usb", "0:1", 1))
		return -1;

	ret = fs_read(filename, addr, (loff_t)pos, (loff_t)bytes, (loff_t*)&len_read);
	if (ret < 0) {
		 update_debug("[ERROR:%s] read data ret %d \n",__func__,ret);
		 len_read = -1;
	}

	return (unsigned long)len_read;
}



//#define __VU_PRINT_HEX_DUMP__

#define __HASH_LEN__ (SHA256_SUM_LEN) // SHA256 32bytes
#define __EM_LEN__   (256) // RSA2048
typedef struct ST_DigestInfo{
	uint8_t hash[__HASH_LEN__];
	uint8_t em[__EM_LEN__];
} ST_DigestInfo;

static int _verify_update_sig(struct key_prop *prop, const uint8_t *sig,
			  const uint32_t sig_len, ST_DigestInfo *digest);
static void _encode_digest_pkcs1_5(ST_DigestInfo *pDigest);
#ifdef __VU_PRINT_HEX_DUMP__
static void __print_hexdump(uint8_t *src, int len);
#endif

void mobis_verify_image_init(sha256_context *ctx){
#ifdef __VU_PRINT_HEX_DUMP__
	printf("## SHA256 INIT\n");
#endif
	sha256_starts(ctx);
}

void mobis_verify_image_update(sha256_context *ctx, uint8_t *data, int len){
#ifdef __VU_PRINT_HEX_DUMP__
//	__print_hexdump(data,len);
#endif
	printf(".");
	sha256_update(ctx, data, len);
}

int mobis_verify_image_final(sha256_context *ctx, uint8_t *sig, int len){
	ST_DigestInfo digest;
	int ret;
	struct key_prop pubkey;
	/////////////////////////////////////////
	// Public Key info (from extractPubKeyValue tool)
	/////////////////////////////////////////
	const int _rsa_bits = 2048;
	const uint8_t _e[]={0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01}; // public exponent
	const uint32_t _n0inv = 0x38d6b0af;
	const uint8_t _modulus[]={
	 0xb8,0x28,0xb3,0xfa,0x6a,0xa0,0x84,0x28,0x00,0xb8,0x1b,0xb2,0x80,0xbe,0x2f,0x3e
	,0xeb,0xc4,0x77,0xe6,0x5e,0x87,0x74,0xdb,0x23,0x38,0x47,0x88,0xa3,0xf0,0xe5,0xfe
	,0x52,0x57,0x04,0x48,0x2b,0x5d,0x48,0xff,0x9f,0x46,0xf7,0x53,0x95,0xce,0xa0,0x9c
	,0x78,0xe3,0xd9,0x44,0x21,0x8e,0xbc,0xca,0x3b,0x41,0xdc,0x11,0x03,0x7a,0x4e,0x76
	,0x5f,0xdc,0x3c,0x81,0x32,0x0c,0x3d,0x5c,0x38,0x04,0xd8,0x51,0x16,0xe9,0x9a,0x49
	,0x55,0x99,0x45,0x80,0x3f,0xdb,0x1b,0x76,0x0d,0xf9,0x81,0x10,0x4e,0xd2,0x61,0xe0
	,0xf0,0x67,0x83,0x84,0x4d,0xe1,0xde,0xab,0x13,0x28,0xfb,0x97,0xc8,0x8c,0x2d,0xbd
	,0xc0,0x41,0xa2,0x36,0x11,0x9d,0xc3,0xff,0x15,0x28,0xdb,0xe5,0x46,0x43,0xc4,0xb3
	,0xe5,0x3f,0xb8,0x64,0xd9,0xa6,0xc4,0x9c,0x5b,0xc2,0xfe,0xdb,0xd1,0x0d,0x8f,0xef
	,0x89,0xa8,0x41,0x44,0x2f,0xac,0x62,0x94,0x32,0x48,0x2b,0x21,0xd7,0xc8,0xbe,0xd7
	,0x03,0x9b,0x98,0x80,0x7b,0x61,0x64,0x13,0x00,0x72,0x80,0xe7,0xb0,0x7a,0x4e,0xca
	,0x9b,0x55,0x5a,0x25,0x7c,0xce,0x96,0x76,0x72,0x9d,0xdc,0x00,0x86,0xb8,0xd8,0x2e
	,0xb2,0x47,0x3c,0x25,0x92,0x1f,0xb3,0x29,0x61,0x41,0xde,0x4c,0x77,0x0a,0x1b,0xcb
	,0x54,0x0d,0xf6,0x8d,0xfa,0x14,0xd3,0xc2,0x60,0xd8,0x54,0xc4,0x80,0x9d,0x38,0xa3
	,0xe7,0xbc,0x84,0x8f,0x37,0x6f,0x0a,0x96,0x51,0x5c,0x01,0x55,0x3c,0xd5,0x9e,0x9d
	,0xe4,0x07,0xe7,0x9f,0x9b,0xaa,0x7d,0x9b,0x00,0xe2,0x70,0xec,0xac,0xcd,0x59,0xb1
	};
	const uint8_t _rr[]={
	 0x0f,0x36,0x23,0x3f,0xbc,0x1a,0xb7,0x0b,0x38,0xff,0xa0,0xac,0xe7,0x72,0x6d,0x3f
	,0x48,0x4f,0xa9,0x2f,0xcf,0x59,0x00,0xe0,0x01,0xfc,0x50,0xe7,0x75,0xd0,0x86,0xd3
	,0x39,0x37,0x80,0xe4,0x86,0xbb,0xbc,0x98,0x30,0x93,0xf3,0x8c,0x1f,0x2f,0xf1,0x2a
	,0xb9,0xc3,0x9c,0x07,0x5c,0x7e,0xe9,0x7e,0x97,0xdc,0xb2,0xbc,0x22,0x4a,0x2a,0x2a
	,0xb7,0x21,0xef,0x28,0x2a,0x0c,0xf9,0x18,0xa5,0x06,0x44,0xbe,0xf4,0x0a,0xc0,0x3a
	,0xff,0xa2,0xd8,0x34,0x63,0xcf,0x48,0x14,0x5a,0x07,0x97,0x88,0x8d,0x07,0x28,0x4e
	,0xf7,0x77,0x12,0x09,0x90,0xeb,0xc2,0xe2,0xa8,0x42,0x04,0x6d,0xd2,0x74,0xb8,0xae
	,0x92,0x25,0x2e,0xfb,0x97,0x8b,0xf8,0x4f,0x5b,0xff,0x7b,0xfb,0xff,0xe0,0xaa,0xdf
	,0x27,0x58,0x26,0xc1,0xeb,0x34,0x9c,0x0a,0xe1,0x0c,0xb4,0xd2,0x70,0xd5,0x15,0xb7
	,0x89,0xe3,0x16,0xaf,0x67,0x0c,0xa3,0xe8,0x1b,0xfe,0x79,0x7b,0x4d,0x65,0xe9,0x32
	,0x89,0x5f,0xae,0xf5,0x8c,0x6c,0xb9,0x1c,0x67,0xa3,0x75,0xbc,0xc8,0x76,0x75,0x5e
	,0xf4,0x46,0x1d,0x81,0xa6,0xc6,0xe5,0xf6,0x5e,0x57,0xa7,0x43,0x24,0x41,0x57,0x2c
	,0xd9,0x59,0xbe,0x41,0x93,0x73,0x04,0x76,0x59,0x21,0xc0,0x84,0x11,0xe6,0xa8,0x85
	,0x3b,0x94,0xde,0x28,0x85,0x20,0xd6,0xc5,0xad,0xb4,0x6e,0x62,0x1d,0xa2,0x4c,0x97
	,0x96,0x00,0x49,0x48,0x60,0x7c,0x55,0x67,0x8b,0x54,0x0d,0x2f,0x23,0x66,0x97,0x33
	,0xdf,0x45,0xe3,0x4f,0x20,0xb5,0xb5,0xa8,0x80,0x27,0xbb,0x77,0xe9,0x5f,0xc3,0xea
	};


	// key setting
	/*
	struct key_prop {
		const void *rr;		// R^2 can be treated as byte array
		const void *modulus;	// modulus as byte array
		const void *public_exponent; // public exponent as byte array
		uint32_t n0inv;		// -1 / modulus[0] mod 2^32
		int num_bits;		// Key length in bits
		uint32_t exp_len;	// Exponent length in number of uint8_t
	};
	*/
	pubkey.rr = _rr;
	pubkey.modulus = _modulus;
	pubkey.public_exponent = _e;
	pubkey.n0inv = _n0inv;
	pubkey.num_bits = _rsa_bits;
	pubkey.exp_len = sizeof(_e);
	
	// finalize hash
	sha256_finish(ctx, digest.hash);
#ifdef __VU_PRINT_HEX_DUMP__
	printf("\n## Calculated SHA256 :\n");
	__print_hexdump(digest.hash,__HASH_LEN__);
#endif
	// encode digest DER
	_encode_digest_pkcs1_5(&digest);

	// verify signature
	if((ret=_verify_update_sig(&pubkey, sig, len, &digest))!=0){
		printf("Signature Verification Failed!!!\n");
	}else{
		printf("Signature Verification OK!!!\n");
	}

	return ret;
}

int mobis_verify_image(unsigned long loadaddr){
	sha256_context ctx;
	int  i;
	int ret = 0;
	unsigned long read_size = 0;
	unsigned long file_size = 0;
	unsigned long file_offset = 0;
	static uint8_t sig[SIG_FILE_SIZE] ={ 0, };

	ret = usb_fat_file_exists(UPDATE_FILE_NAME);
	if (ret <=0)
	{
		update_debug("\n## file not exits %s\n", UPDATE_FILE_NAME);
		return -1;
	}
	ret = usb_fat_file_exists(UPDATE_SIGNATURE_FILE_NAME);
	if (ret <=0)
	{
		update_debug("\n## file not exits %s\n", UPDATE_SIGNATURE_FILE_NAME);
		return -1;
	}
	file_size = usb_fat_file_size(UPDATE_SIGNATURE_FILE_NAME);
	if (file_size == 0)
	{
		update_debug("\n## file size error %s\n", UPDATE_SIGNATURE_FILE_NAME);
		return -1;
	}
	file_size = usb_fat_file_size(UPDATE_FILE_NAME);
	if (file_size == 0)
	{
		update_debug("\n## file size error %s\n", UPDATE_FILE_NAME);
		return -1;
	}

	read_size = usb_fat_read_with_filename(UPDATE_SIGNATURE_FILE_NAME, loadaddr, SIG_FILE_SIZE, 0);
	if(read_size != SIG_FILE_SIZE)
	{
		update_debug("\n## file read size error %s\n", UPDATE_SIGNATURE_FILE_NAME);
		return -1;
	}
	memcpy(sig, loadaddr, SIG_FILE_SIZE);

	update_debug("\nSignature Verification start!!! Please wait about 10 minutes...\n",__FUNCTION__,__LINE__);
	mobis_verify_image_init(&ctx);
	while(file_offset <= file_size)
	{
		memset(loadaddr, 0x00, UPDATE_BUFFER_SIZE);
		read_size = usb_fat_read_with_filename(UPDATE_FILE_NAME, loadaddr, UPDATE_BUFFER_SIZE, file_offset);
		if(read_size > UPDATE_BUFFER_SIZE)
		{
			update_debug("file I/O error!!! read size = %u, file size=%u\n", read_size, file_size);
			return -1;
		}
		file_offset+=read_size;
		mobis_verify_image_update(&ctx, loadaddr, read_size);
		if (read_size < UPDATE_BUFFER_SIZE)
			break;
	}
	if (file_offset != file_size)
	{
		update_debug("file read error!!! read size = %u, file size=%u\n", file_offset, file_size);
		return -1;
	}
	update_debug("file read success!!! read size = %u, file size=%u\n", file_offset, file_size);
	return mobis_verify_image_final(&ctx, sig, sizeof(sig));
}

/////////////////////////////////////////
// static functions
/////////////////////////////////////////


/* 
  PKCS #1: RSA Cryptography Specifications Version 2.2
  https://tools.ietf.org/html/rfc8017#section-9.2
  9.2.  EMSA-PKCS1-v1_5
  
  1.  Apply the hash function to the message M to produce a hash
	  value H:

		 H = Hash(M).

	  If the hash function outputs "message too long", output
	  "message too long" and stop.

  2.  Encode the algorithm ID for the hash function and the hash
	  value into an ASN.1 value of type DigestInfo (see
	  Appendix A.2.4) with the DER, where the type DigestInfo has
	  the syntax

		   DigestInfo ::= SEQUENCE {
			   digestAlgorithm AlgorithmIdentifier,
			   digest OCTET STRING
		   }

	  The first field identifies the hash function and the second
	  contains the hash value.  Let T be the DER encoding of the
	  DigestInfo value (see the notes below), and let tLen be the
	  length in octets of T.

  3.  If emLen < tLen + 11, output "intended encoded message length
	  too short" and stop.

  4.  Generate an octet string PS consisting of emLen - tLen - 3
	  octets with hexadecimal value 0xff.  The length of PS will be
	  at least 8 octets.

  5.  Concatenate PS, the DER encoding T, and other padding to form
	  the encoded message EM as

		 EM = 0x00 || 0x01 || PS || 0x00 || T.

  6.  Output EM.

SHA-256: DER encoding (fixed OID prefix and hash length)
       30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05
       00 04 20 || H.

hash example
                c7 75 e7 b7 57 ed e6 30 cd 0a a1 11 3b
       d1 02 66 1a b3 88 29 ca 52 a6 42 2a b7 82 86 2f
	   26 86 46

T example
0000 - 30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05
0010 - 00 04 20 c7 75 e7 b7 57 ed e6 30 cd 0a a1 11 3b
0020 - d1 02 66 1a b3 88 29 ca 52 a6 42 2a b7 82 86 2f
0030 - 26 86 46

*/
static void _encode_digest_pkcs1_5(ST_DigestInfo *pDigest){
	static const uint8_t digestinfo[]={
		0x30,0x31,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01,0x05,0x00,0x04,0x20
	}; // fixed DER prefix for SHA256
	int i=0;
	int tLen, psLen;

	tLen=sizeof(digestinfo)+__HASH_LEN__; // DER encoding length : 19+32 = 51
	psLen=__EM_LEN__-tLen-3; // emLen - tLen - 3 = 256-51-3 = 202

	// EM = 0x00 || 0x01 || PS || 0x00 || T.
	pDigest->em[i++]=0x00;
	pDigest->em[i++]=0x01;
	memset(pDigest->em+i, 0xff, psLen);
	i+=psLen;
	pDigest->em[i++]=0x00;
	memcpy(pDigest->em+i,digestinfo,sizeof(digestinfo));
	i+=sizeof(digestinfo);
	memcpy(pDigest->em+i,pDigest->hash,__HASH_LEN__);
}

#ifdef __VU_PRINT_HEX_DUMP__
static void __print_hexdump(uint8_t *src, int len){
	int i;
	
	for(i=0 ; i<len ; i++){
		if(i && (i%16==0)) printf("\n");
		printf("0x%02x ",src[i]);
	}
	printf("\n");
}
#endif

static int _verify_update_sig(struct key_prop *prop, const uint8_t *sig,
			  const uint32_t sig_len, ST_DigestInfo *digest)
{
	uint8_t buf[__EM_LEN__];
	int ret;

	if (!prop || !sig || !digest)
		return -EIO;

	if (sig_len != (prop->num_bits / 8)) {
		debug("Signature is of incorrect length %d\n", sig_len);
		return -EINVAL;
	}

	/* Sanity check for stack size */
	if (sig_len > __EM_LEN__) {
		debug("Signature length %u exceeds maximum %d\n", sig_len,__EM_LEN__);
		return -EINVAL;
	}
	ret = rsa_mod_exp_sw(sig, sig_len, prop, buf);
#ifdef __VU_PRINT_HEX_DUMP__
	printf("## SHA256-Calculated and DER encoded DigestInfo :\n");
	__print_hexdump(digest->em,__EM_LEN__);
	printf("## PublicKey-Decrypted Sig :\n");
	__print_hexdump(buf,__EM_LEN__);
#endif
	if (ret) {
		debug("Error in Modular exponentation\n");
		return ret;
	}
	/* Check encoded digest */
	if (memcmp(buf, digest->em, __EM_LEN__)) {
		debug("In verifying update sig : Hash check failed!\n");
		return -EACCES;
	}

	return 0;
}


