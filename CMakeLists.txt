#cmake_minimum_required(VERSION 3.11.4)
project (cryptoauth)

set(TARGET_GROUP main CACHE STRING "Group to build")

if(STATIC_ANALYSIS)
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



if(TARGET_GROUP STREQUAL main)
    add_subdirectory(lib)
elseif(TARGET_GROUP STREQUAL all)
    add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -Werror -O1 --coverage -fsanitize=address -fno-omit-frame-pointer)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
    SET(CMAKE_EXE_LINKER_FLAGS "-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra -Werror -pedantic -O1 --coverage -fsanitize=address -fno-omit-frame-pointer ${CMAKE_EXE_LINKER_FLAGS}")
    add_subdirectory(lib)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()