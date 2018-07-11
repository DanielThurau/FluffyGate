/**
 * Copyright (C) 2018 David C. Harrison - All Rights Reserved.
 * You may not use, distribute, or modify this code without
 * the written permission of the copyright holder.
 *
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 *
 * Compile any program including this file with -lssl -lcrypto
 */

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#include "crypto.h"

int handleErrors(void){
    ERR_print_errors_fp(stderr);
    // abort();
    return -1;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext){
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) 
        return handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        return handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        return handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) 
        return handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext){
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) 
        return handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        return handleErrors();
    
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        return handleErrors();
    plaintext_len = len;
    
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) 
        return handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

void demo(){
    printf("** DEMO MODE ONLY, NOT AN ACTUAL ENCRYPTION OR DECRYPTION ***\n");

    /* A 128 bit key */
    unsigned char *key1 = (unsigned char *)"770A8A65DA156D24EE2A093277530142";
    unsigned char *key2 = (unsigned char *)"770A8A65DA156D24EE2A093277530142";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"E7C6357864F322AF2EC892CC2D4E4EBA";

    /* Message to be encrypted */
    unsigned char *plaintext = (unsigned char *)"CMPS122 is an awesome class!";

    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
    unsigned char ciphertext[128];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key1, iv, ciphertext);

    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key2, iv, decryptedtext);
    if(decryptedtext_len == -1){
        printf("Incorrect key used to decrypt: %s\n",key2);
        exit(1);
    }
    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);
}


int test_iv_key(unsigned char *key, unsigned char *iv){
    /* Message to be encrypted */
    unsigned char *plaintext = (unsigned char *)"CMPS122 is an awesome class!";

    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
    unsigned char ciphertext[128];

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
    if(decryptedtext_len == -1){
        exit(1);
    }
    decryptedtext[decryptedtext_len] = '\0';

    if(strcmp(plaintext, decryptedtext) == 0){
        return 0;
    }else{
        return -1;
    }
}



