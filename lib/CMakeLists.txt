file(GLOB SOURCES
        basic/atca_basic.c
        basic/atca_basic_aes.c
        basic/atca_basic_checkmac.c
        basic/atca_basic_counter.c
        basic/atca_basic_derivekey.c
        basic/atca_basic_ecdh.c
        basic/atca_basic_gendig.c
        basic/atca_basic_genkey.c
        basic/atca_basic_hmac.c
        basic/atca_basic_info.c
        basic/atca_basic_kdf.c
        basic/atca_basic_lock.c
        basic/atca_basic_mac.c
        basic/atca_basic_nonce.c
        basic/atca_basic_privwrite.c
        basic/atca_basic_random.c
        basic/atca_basic_read.c
        basic/atca_basic_secureboot.c
        basic/atca_basic_selftest.c
        basic/atca_basic_sha.c
        basic/atca_basic_sign.c
        basic/atca_basic_updateextra.c
        basic/atca_basic_verify.c
        basic/atca_basic_write.c
        atca_cfgs.c
        atca_command.c
        atca_device.c
        atca_execution.c
        atca_iface.c
        crypto/atca_crypto_sw_sha2.c
        basic/atca_helpers.c
        host/atca_host.c
        hal/atca_hal.c
        hal/hal_linux_i2c_userspace.c
        crypto/hashes/sha1_routines.c
        crypto/hashes/sha2_routines.c
)

add_library(cryptoauthdyn SHARED ${SOURCES} )
add_library(cryptoauth STATIC ${SOURCES} )

target_include_directories(cryptoauth PUBLIC
    .
)
target_include_directories(cryptoauthdyn PUBLIC
    .
)