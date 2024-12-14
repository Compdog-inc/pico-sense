cmake_minimum_required(VERSION 3.30)

# Initialize SDK config
set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

# Include SDK, kernel, and libraries (must be before project)
include("$ENV{PICO_SDK_PATH}\\external\\pico_sdk_import.cmake")
include("$ENV{FREERTOS_KERNEL_PATH}\\portable\\ThirdParty\\GCC\\RP2040\\FreeRTOS_Kernel_import.cmake")

project(pico-sense C CXX ASM)
set(CMAKE_C_STANDARD 17)        # C17
set(CMAKE_CXX_STANDARD 23)      # C++23

# Implement custom panic callback
add_compile_definitions(PICO_PANIC_FUNCTION=rtos_panic)
# Set debug level
add_compile_definitions(DEBUG_LEVEL=0)

# Verify pico SDK version
if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

# Set compile options
add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        -Wno-psabi
        )

add_executable(pico-sense
        # entry point
        src/main.cpp
        )

# Include headers
target_include_directories(pico-sense PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/
        )

# Link libraries
target_link_libraries(pico-sense
        pico_stdlib                             # stdlib
        FreeRTOS-Kernel-Heap4                   # FreeRTOS kernel and dynamic heap
        # Hardware libraries
        hardware_pwm
        hardware_pio
        )

# Include kernel header
if(MSVC)
    add_definitions(/FI"${CMAKE_CURRENT_SOURCE_DIR}/src/config/kernel.h")
else()
    # GCC or Clang
    add_definitions(-include "${CMAKE_CURRENT_SOURCE_DIR}/src/config/kernel.h")
endif()

pico_add_extra_outputs(pico-sense)