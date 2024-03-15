#ifndef __VERIFY_UPDATE_IMAGE_H__
#define __VERIFY_UPDATE_IMAGE_H__

#include <inttypes.h>
#include <u-boot/sha256.h>

/**
 * mobis_verify_image_init() - init context for update image verification.
 *
 * @ctx:	SHA256 context for iterative hash operations
 * @return  void
 */
void mobis_verify_image_init(sha256_context *ctx);

/**
 * mobis_verify_image_update() - update hash value for update image verification.
 *                               data can be splited into any size(not 0) blocks, and
 *                               MUST be inserted sequencially.
 *
 * @ctx:	SHA256 context for iterative hash operations
 * @data:   partial or full part of contents for signature verification.
 * @len:    the size of data in bytes
 * @return  void
 */
void mobis_verify_image_update(sha256_context *ctx, uint8_t *data, int len);

/**
 * mobis_verify_image_init() - verify signature.
 *
 * @ctx:	SHA256 context for iterative hash operations
 * @sig:    Signature data from MOBIS update image signing tool
 * @len:    the size of Signature data in bytes
 * @return  0 if the signature is valid, otherwise error.
 */
int mobis_verify_image_final(sha256_context *ctx, uint8_t *sig, int len);

int mobis_verify_image(unsigned long loadaddr);


#endif
