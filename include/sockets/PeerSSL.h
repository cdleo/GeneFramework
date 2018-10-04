/* 
 * File:   PeerSSL.h
 * Author: cleo
 *
 * Created on May 17, 2014, 5:29 PM
 */

#ifndef PEERSSL_H
#define	PEERSSL_H

#include <iostream>

#include <sockets/PeerIN.h>
#include <sockets/ISync.h>

#include <openssl/bio.h> 
#include <openssl/ssl.h> 
#include <openssl/err.h> 
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/engine.h>

class PeerSSL : public PeerIN, public ISync {
public:
    PeerSSL(const char* certificateFilePath, const char* keyFilePath);
    virtual ~PeerSSL() {
        DestroySSL();
    };
    
    virtual bool connectToHost(const char* ip, const int port, const int timeoutms);
    virtual int setFD(int fd);

    virtual SocketReturnCode gRead(void* buff, const size_t buffSize, const int timeoutms);
    virtual SocketReturnCode gReadMore(void* buff, const size_t buffSize);
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize, const int timeoutms) { return SocketReturnCode_Error; }
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize, const int timeoutms);
    
private:
    static void initializeSSL() {
        if (!sslInitialized_) {
            sslInitialized_ = true;
            SSL_library_init();
            SSL_load_error_strings();            
        }
        OpenSSL_add_all_algorithms();     
    }
    
    SSL_CTX* initCTX(const SSL_METHOD* method) {
        SSL_CTX* sslctx = SSL_CTX_new( method );
        SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE | SSL_OP_NO_COMPRESSION | SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS | SSL_MODE_RELEASE_BUFFERS);
        SSL_CTX_use_certificate_file(sslctx, certificate_.c_str() , SSL_FILETYPE_PEM);
        SSL_CTX_use_PrivateKey_file(sslctx, key_.c_str(), SSL_FILETYPE_PEM);    
        
        /* verify private key */
        if ( !SSL_CTX_check_private_key(sslctx) ){
            std::cout << "Private key does not match the public certificate" << std::endl;
            abort();
        }        
        
        return sslctx;
    }
    
    void DestroySSL() {
        CONF_modules_free();
        ERR_remove_state(0);
        ENGINE_cleanup();
        CONF_modules_unload(1);
        ERR_free_strings();
        EVP_cleanup();
        CRYPTO_cleanup_all_ex_data();
    }
    
    void ShutdownSSL() {
        if (ssl_) {
            //DestroySSL();
            
            SSL_shutdown(ssl_);
            SSL_free(ssl_);
            ssl_ = NULL;
        }
    }    
    
protected:
    static bool                 sslInitialized_;
    
    SSL*                        ssl_;
    std::string                 certificate_;
    std::string                 key_;
};

#endif	/* PEER_H */