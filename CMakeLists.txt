cmake_minimum_required(VERSION 3.11.4)
project (cryptoauth)

set(TARGET_GROUP main CACHE STRING "Group to build")

if(STATIC_ANALYSIS)
   # find_program(
        #CLANG_TIDY_EXE
        #NAMES "clang-tidy"
        #DOC "Path to clang-tidy executable"
    #)
    #if(NOT CLANG_TIDY_EXE)
        #message(FATAL_ERROR "clang-tidy not found.")
    #else()
        #message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        #set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}" -checks=*,-clang-analyzer-alpha.* -warnings-as-errors=* -fix)
    #endif()

    find_program(
        CMAKE_C_CPPCHECK
        NAMES "cppcheck"
        DOC "Path to cppcheck executable"
    )
    if(NOT CMAKE_C_CPPCHECK)
        message(FATAL_ERROR "cppcheck not found.")
    else()
        message(STATUS "cppcheck found: ${CMAKE_C_CPPCHECK}")
        set(CMAKE_C_CPPCHECK "${CMAKE_C_CPPCHECK}" --enable=warning,performance,portability,style --error-exitcode=1 --force --quiet)
    endif()
endif()


add_definitions(
    -DATCA_COMPILE
    -DATCA_HAL_I2C
    -DATCAPRINTF
    )

#add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -Werror -O1 --coverage -fsanitize=address -fno-omit-frame-pointer)
add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -O1 --coverage -fsanitize=address -fno-omit-frame-pointer)
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
#SET(CMAKE_EXE_LINKER_FLAGS "-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -Werror -pedantic -O1 --coverage -fsanitize=address -fno-omit-frame-pointer ${CMAKE_EXE_LINKER_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -pedantic -O1 --coverage -fsanitize=address -fno-omit-frame-pointer ${CMAKE_EXE_LINKER_FLAGS}")

file(GLOB SOURCES "lib/*.c" "lib/atcacert/*.c" "lib/basic/*.c" "lib/crypto/*.c" "lib/host/*.c"  "lib/jwt/*.c"
    "lib/hal/atca_hal.c"
    "lib/hal/atca_i2c_bitbang.c"
)

add_library(cryptoauthdyn SHARED ${SOURCES} )

add_library(cryptoauthst STATIC ${SOURCES} )

target_include_directories(cryptoauthst PUBLIC
    lib
)
target_include_directories(cryptoauthdyn PUBLIC
    lib
)
#add_subdirectory(Unity)

include(CTest)
file( GLOB TEST_SOURCES "test/*.c" "test/atcacert/*.c" "test/jwt/*.c" )

add_executable(tests
    ${TEST_SOURCES}
)

add_definitions(
    -DUSE_ETHEREUM 
    -DRAND_PLATFORM_INDEPENDENT 
    -DUSE_HAL_DRIVER 
    -DSTM32L475xx
    -DUSBD_LPM_ENABLED 
    -DPB_FIELD_16BIT
    -DATCA_COMPILE
    -DATCA_HAL_I2C
    -DATCAPRINTF
    -DuECC_VLI_NATIVE_LITTLE_ENDIAN=0
    -DNO_PIN
    )
target_include_directories(tests PUBLIC test  )
target_include_directories(tests PUBLIC ./  )
target_include_directories(tests PUBLIC test/atcacert  )


target_link_libraries(tests
    cryptoauthst
)

target_compile_features(tests PRIVATE
    c_std_99
)
#add_test(${test_name} ${test_name})




