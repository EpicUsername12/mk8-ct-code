#include "crypto_engine.hpp"

ctgp::crypto::CryptoEngine* ctgp::crypto::CryptoEngineInstance = NULL;

void ctgp::crypto::CryptoEngine::Initialize() {
    CryptoEngineInstance = this;
    register_cipher(&aes_desc);
}

void ctgp::crypto::CryptoEngine::AES_Encrypt16(uint8_t* key, uint8_t* iv, uint8_t* source, uint8_t* dest, size_t length) {
    symmetric_CBC cbc;
    cbc_start(find_cipher("aes"), static_cast<const uint8_t*>(iv), static_cast<const uint8_t*>(key), 16, 0, &cbc);
    cbc_encrypt(source, dest, length, &cbc);
    cbc_done(&cbc);
}

void ctgp::crypto::CryptoEngine::AES_Encrypt32(uint8_t* key, uint8_t* iv, uint8_t* source, uint8_t* dest, size_t length) {
    symmetric_CBC cbc;
    cbc_start(find_cipher("aes"), static_cast<const uint8_t*>(iv), static_cast<const uint8_t*>(key), 32, 0, &cbc);
    cbc_encrypt(source, dest, length, &cbc);
    cbc_done(&cbc);
}

void ctgp::crypto::CryptoEngine::AES_Decrypt16(uint8_t* key, uint8_t* iv, uint8_t* source, uint8_t* dest, size_t length) {
    symmetric_CBC cbc;
    cbc_start(find_cipher("aes"), static_cast<const uint8_t*>(iv), static_cast<const uint8_t*>(key), 16, 0, &cbc);
    cbc_decrypt(source, dest, length, &cbc);
    cbc_done(&cbc);
}

void ctgp::crypto::CryptoEngine::AES_Decrypt32(uint8_t* key, uint8_t* iv, uint8_t* source, uint8_t* dest, size_t length) {
    symmetric_CBC cbc;
    cbc_start(find_cipher("aes"), static_cast<const uint8_t*>(iv), static_cast<const uint8_t*>(key), 32, 0, &cbc);
    cbc_decrypt(source, dest, length, &cbc);
    cbc_done(&cbc);
}

void ctgp::crypto::CryptoEngine::RC4_EncryptDecrypt(uint8_t* key, size_t keylen, uint8_t* source, uint8_t* dest, size_t data_len, size_t offset) {
    internal_rc4_state st;
    internal_rc4_init(&st, key, keylen), internal_rc4_crypt(&st, source, dest, data_len, offset);
}

static __inline void swap_bytes(uint8_t* a, uint8_t* b) {
    uint8_t temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void internal_rc4_init(internal_rc4_state* state, uint8_t* key, int keylen) {
    uint8_t j;
    int i;

    for (i = 0; i < 256; i++)
        state->perm[i] = (uint8_t)i;

    state->index1 = 0;
    state->index2 = 0;

    for (j = i = 0; i < 256; i++) {
        j += state->perm[i] + key[i % keylen];
        swap_bytes(&state->perm[i], &state->perm[j]);
    }
}

void internal_rc4_crypt(internal_rc4_state* state, uint8_t* inbuf, uint8_t* outbuf, int buflen, uint32_t start_off) {
    int i;
    uint8_t j;

    for (uint32_t i = 0; i < start_off; i++) {

        state->index1++;
        state->index2 += state->perm[state->index1];
        swap_bytes(&state->perm[state->index1], &state->perm[state->index2]);
        j = state->perm[state->index1] + state->perm[state->index2];
    }

    for (i = 0; i < buflen; i++) {

        state->index1++;
        state->index2 += state->perm[state->index1];

        swap_bytes(&state->perm[state->index1], &state->perm[state->index2]);
        j = state->perm[state->index1] + state->perm[state->index2];

        outbuf[i] = inbuf[i] ^ state->perm[j];
    }
}