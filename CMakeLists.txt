cmake_minimum_required(VERSION 3.5)
project(FormatOutput CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#add_definitions("-std=c++11")

# if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
#   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
#   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake"
#                 "${CMAKE_BINARY_DIR}/conan.cmake"
#                 TLS_VERIFY ON)
# endif()

# include(${CMAKE_BINARY_DIR}/conan.cmake)
include(CMakePrintHelpers)

include(conan.cmake)
conan_add_remote(NAME conancenter URL https://center.conan.io)

conan_cmake_configure(
    REQUIRES
    fmt/6.1.2
    boost/1.77.0
    GENERATORS cmake_find_package
    OPTIONS
    boost:shared=False
    boost:without_atomic=True
    boost:without_chrono=True
    boost:without_container=True
    boost:without_context=True
    boost:without_contract=True
    boost:without_coroutine=True
    boost:without_date_time=True
    boost:without_exception=True
    boost:without_fiber=True
    boost:without_filesystem=True
    boost:without_graph=True
    boost:without_graph_parallel=True
    boost:without_iostreams=True
    boost:without_json=True
    boost:without_locale=True
    boost:without_log=True
    boost:without_math=True
    boost:without_mpi=True
    boost:without_nowide=True
    boost:without_program_options=False
    boost:without_python=True
    boost:without_random=True
    boost:without_regex=True
    boost:without_serialization=True
    boost:without_stacktrace=True
    boost:without_system=True
    boost:without_test=True
    boost:without_thread=True
    boost:without_timer=True
    boost:without_type_erasure=True
    boost:without_wave=True

    b2:use_cxx_env=False
)

conan_cmake_autodetect(settings)
# Debugging output
cmake_print_variables(settings CMAKE_C_COMPILER CMAKE_CXX_COMPILER)

if(ANDROID)
    set(CONAN_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/ConanToolchainWrapper.cmake")
    file(WRITE "${CONAN_TOOLCHAIN_FILE}" "set\(ANDROID_PLATFORM \"${ANDROID_PLATFORM}\"\)\n"
                                      "set\(ANDROID_ABI \"${ANDROID_ABI}\"\)\n"
                                      "set\(ANDROID_CPP_FEATURES \"${ANDROID_CPP_FEATURES}\"\)\n"
                                      "set(ANDROID_STL \"${ANDROID_STL}\"\)\n"
                                      "include\(\"${CMAKE_TOOLCHAIN_FILE}\"\)\n")


    string(TOUPPER "${TYPE}" UPPER_TYPE)
    list(APPEND settings "arch=armv8") #"${ANDROID_SYSROOT_ABI}")
    list(APPEND settings "os=Android")
    list(APPEND settings "os.api_level=${ANDROID_PLATFORM_LEVEL}")

    list(APPEND env_settings "CONAN_CMAKE_TOOLCHAIN_FILE=${CONAN_TOOLCHAIN_FILE}")
    #list(APPEND env_settings "PATH=${ANDROID_TOOLCHAIN_ROOT}/bin")
    list(APPEND env_settings "CHOST=${CMAKE_C_COMPILER_TARGET}")
    list(APPEND env_settings "AR=${CMAKE_AR}")
    list(APPEND env_settings "AS=${ANDROID_ASM_COMPILER}")
    list(APPEND env_settings "RANLIB=${CMAKE_RANLIB}")
    list(APPEND env_settings "CC=${ANDROID_TOOLCHAIN_ROOT}/bin/${CMAKE_LIBRARY_ARCHITECTURE}${ANDROID_PLATFORM_LEVEL}-clang")
    list(APPEND env_settings "CXX=${ANDROID_TOOLCHAIN_ROOT}/bin/${CMAKE_LIBRARY_ARCHITECTURE}${ANDROID_PLATFORM_LEVEL}-clang++")
    list(APPEND env_settings "CFLAGS=--sysroot=${CMAKE_SYSROOT} --gcc-toolchain=${ANDROID_TOOLCHAIN_ROOT} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${UPPER_TYPE}}") # --target=${CMAKE_C_COMPILER_TARGET}
    list(APPEND env_settings "CPPFLAGS=")
    list(APPEND env_settings "CXXFLAGS=--sysroot=${CMAKE_SYSROOT} --gcc-toolchain=${ANDROID_TOOLCHAIN_ROOT} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${UPPER_TYPE}}") # --target=${CMAKE_CXX_COMPILER_TARGET}
    list(APPEND env_settings "ASFLAGS=--sysroot=${CMAKE_SYSROOT} --gcc-toolchain=${ANDROID_TOOLCHAIN_ROOT} ${CMAKE_ASM_FLAGS} ${CMAKE_ASM_FLAGS_${UPPER_TYPE}}") # --target=${CMAKE_C_COMPILER_TARGET}
    list(APPEND env_settings "LDFLAGS=--sysroot=${CMAKE_SYSROOT} --gcc-toolchain=${ANDROID_TOOLCHAIN_ROOT} ${CMAKE_SHARED_LINKER_FLAGS}") # --target=${CMAKE_C_COMPILER_TARGET}
    list(APPEND env_settings "LD=${ANDROID_TOOLCHAIN_PREFIX}ld")
    list(APPEND env_settings "STRIP=${ANDROID_TOOLCHAIN_PREFIX}strip")
endif()

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings}
                    ENV ${env_settings}
                    )

find_package(fmt REQUIRED)
find_package(Boost 1.77 REQUIRED COMPONENTS program_options)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC
    fmt::fmt
    Boost::program_options
)
target_compile_features(main PUBLIC cxx_std_11)
