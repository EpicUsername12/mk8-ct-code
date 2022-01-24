#ifndef _WIIU_CRYPTO_H
#define _WIIU_CRYPTO_H

#include "utils.hpp"
#include <tomcrypt.h>

namespace ctgp::crypto {
class CryptoEngine : public ctgp::common::RootClass {
public:
  CryptoEngine() {
    this->setName("ctgp::crypto::CryptoEngine");
    return;
  }

  /**
   * @brief Initialize the crypto lib
   *
   */
  virtual void Initialize();

  /**
   * @brief  Encrypt data with AES-128-CBC
   * @note
   * @param  key: Pointer to the 16-byte key
   * @param  iv: Pointer to the 16-byte IV
   * @param  source: Pointer to the encrypted data (should be aligned 16 bytes)
   * @param  dest: Pointer to the decrypted data (can be the same as source)
   * @param  length: Size of the data
   * @retval None
   */
  void AES_Encrypt16(uint8_t *key, uint8_t *iv, uint8_t *source, uint8_t *dest,
                     size_t length);

  /**
   * @brief  Decrypt data with AES-128-CBC
   * @note
   * @param  key: Pointer to the 16-byte key
   * @param  iv: Pointer to the 16-byte IV
   * @param  source: Pointer to the encrypted data (should be aligned 16 bytes)
   * @param  dest: Pointer to the decrypted data (can be the same as source)
   * @param  length: Size of the data
   * @retval None
   */
  void AES_Decrypt16(uint8_t *key, uint8_t *iv, uint8_t *source, uint8_t *dest,
                     size_t length);

  /**
   * @brief  Encrypt data with AES-256-CBC
   * @note
   * @param  key: Pointer to the 32-byte key
   * @param  iv: Pointer to the 32-byte IV
   * @param  source: Pointer to the encrypted data (should be aligned 32 bytes)
   * @param  dest: Pointer to the decrypted data (can be the same as source)
   * @param  length: Size of the data
   * @retval None
   */
  void AES_Encrypt32(uint8_t *key, uint8_t *iv, uint8_t *source, uint8_t *dest,
                     size_t length);

  /**
   * @brief  Decrypt data with AES-256-CBC
   * @note
   * @param  key: Pointer to the 32-byte key
   * @param  iv: Pointer to the 32-byte IV
   * @param  source: Pointer to the encrypted data (should be aligned 32 bytes)
   * @param  dest: Pointer to the decrypted data (can be the same as source)
   * @param  length: Size of the data
   * @retval None
   */
  void AES_Decrypt32(uint8_t *key, uint8_t *iv, uint8_t *source, uint8_t *dest,
                     size_t length);

  /**
   * @brief  Encrypt or decrypt (stream cipher) data with RC4
   * @note
   * @param  key: RC4 key
   * @param  keylen: RC4 key size
   * @param  source: Pointer to the source buffer
   * @param  dest: Pointer to the destination buffer (can be the same as source)
   * @param  data_len: Size of the data
   * @param  offset: Starting offset (defaults to 0)
   * @retval None
   */
  void RC4_EncryptDecrypt(uint8_t *key, size_t keylen, uint8_t *source,
                          uint8_t *dest, size_t data_len, size_t offset = 0);

  /**
   * @brief Create a Instance of the CryptoEngine and initializes it
   *
   * @return ctgp::crypto::CryptoEngine*
   */
  static inline ctgp::crypto::CryptoEngine *createInstance() {
    ctgp::crypto::CryptoEngine *inst = new ctgp::crypto::CryptoEngine();
    inst->Initialize();
    return inst;
  }
};

/// CryptoEngine instance
extern CryptoEngine *CryptoEngineInstance;
} // namespace ctgp::crypto

typedef struct __attribute__((packed)) internal_rc4_state {
  uint8_t perm[256];
  uint8_t index1;
  uint8_t index2;
} internal_rc4_state;

void internal_rc4_init(internal_rc4_state *state, uint8_t *key, int keylen);
void internal_rc4_crypt(internal_rc4_state *state, uint8_t *inbuf,
                        uint8_t *outbuf, int buflen, uint32_t start_off);

#endif