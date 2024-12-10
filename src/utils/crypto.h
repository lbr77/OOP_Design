#ifndef _CRYPTO_H_
#define _CRYPTO_H_
#pragma once
#include"openssl/md5.h"
//base64
#include"openssl/bio.h"
#include"openssl/evp.h"
#include"openssl/buffer.h"
#include<string>

std::string md5(const std::string& str) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str.c_str(), str.length());
    MD5_Final(digest, &ctx);
    
    char mdString[33];
    for(int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    return std::string(mdString);
}
std::string base64_encode(const std::string &str){
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, str.c_str(), str.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return result;
}

std::string base64_decode(const std::string &str){
    BIO *bio, *b64;
    char buffer[1024];
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(str.c_str(), str.length());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int length = BIO_read(bio, buffer, str.length());
    BIO_free_all(bio);
    return std::string(buffer, length);
}

#endif