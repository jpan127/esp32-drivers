#include <mbedtls/net.h>
#include <mbedtls/ssl.h>

void tls_init()
{
    // drbg = deterministic random byte generator
    // ctr  = counter mode
    
    // Initialize network context
    mbedtls_net_context net_context;
    mbedtls_net_init(&net_context);
    // Initialize ssl context
    mbedtls_ssl_context ssl_context;
    mbedtls_ssl_init(&ssl_context);
    // Initialize ssl config
    mbedtls_ssl_config ssl_config;
    mbedtls_ssl_config_init(&ssl_config);
    // Not sure
    mbedtls_ctr_drbg_context ctr_drbg_context;
    mbedtls_ctr_drbg_init(&ctr_drbg_context);
    // Not sure
    mbedtls_entropy_context entropy_context;
    mbedtls_entropy_init(&entropy_context);
    // Not sure
    mbedtls_x509_crt x509_crt;
    mbedtls_x509_crt_init(&x509_crt);
    // Initialize psuedo RNG
    mbedtls_ctr_drbg_seed();

    // Connect
    tls_connect()
}

void tls_connect()
{
    medtls_net_connect()
}