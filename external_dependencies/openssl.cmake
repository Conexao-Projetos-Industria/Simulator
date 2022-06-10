if(UNIX)
    find_package(OpenSSL REQUIRED)
else()
    find_package(OpenSSL QUIET)
endif()

if(NOT OpenSSL_FOUND AND WIN32)

    include(FetchContent)

    # #############################################
    # Get Perl
    include(FindPerl)
    if(NOT PERL_FOUND)
        message("-- Downloading Perl...")

        FetchContent_Declare(
            perl
            URL https://strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit-portable.zip
            SOURCE_DIR perl
        )

        FetchContent_MakeAvailable(perl)
        list(APPEND CMAKE_PROGRAM_PATH ${CMAKE_BINARY_DIR}/perl/perl/bin)
    else()
        get_filename_component(perl_dir ${PERL_EXECUTABLE} DIRECTORY)
        list(APPEND CMAKE_PROGRAM_PATH ${perl_dir})
    endif()

    # #############################################
    # Get NASM assembler
    message("-- Downloading NASM...")

    FetchContent_Declare(
        nasm
        URL https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip
        SOURCE_DIR nasm
    )

    FetchContent_MakeAvailable(nasm)

    list(APPEND CMAKE_PROGRAM_PATH ${CMAKE_BINARY_DIR}/nasm)

    # #############################################
    # Find programs
    find_program(nasm NAMES nasm REQUIRED)
    find_program(perl NAMES perl REQUIRED)
    
    # #############################################
    # Configure directory and file properties as GENERATED so CMake's dependency check works
    # with External_Project
    set(OPENSSL_SOURCE_DIR ${CMAKE_BINARY_DIR}/openssl_external-prefix/src/openssl_external)
    set(OPENSSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/openssl_external-prefix/install)

    set_source_files_properties(${OPENSSL_INSTALL_DIR}/lib/libssl.lib PROPERTIES GENERATED TRUE)
    set_source_files_properties(${OPENSSL_INSTALL_DIR}/lib/libcrypto.lib PROPERTIES GENERATED TRUE)
    file(MAKE_DIRECTORY ${OPENSSL_INSTALL_DIR}/include)

    # #############################################
    # Get OpenSSL
    include(ExternalProject)

    ExternalProject_Add(
        openssl_external
        GIT_REPOSITORY https://github.com/openssl/openssl.git
        GIT_TAG openssl-3.0.3
        GIT_PROGRESS TRUE

        # Copy NASM executable to openssl dir so perl can find it
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy ${nasm} ${OPENSSL_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E chdir ${OPENSSL_SOURCE_DIR} ${perl} Configure VC-WIN64A no-shared --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR}

        BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${OPENSSL_SOURCE_DIR} nmake

        INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${OPENSSL_SOURCE_DIR} nmake install
    )

    add_library(OpenSSL::SSL STATIC IMPORTED)
    set_target_properties(
        OpenSSL::SSL PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libssl.lib
    )
    add_dependencies(OpenSSL::SSL openssl_external)
    
    add_library(OpenSSL::Crypto STATIC IMPORTED)
    set_target_properties(
        OpenSSL::SSL PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INSTALL_DIR}/include
        IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libcrypto.lib
    )
    add_dependencies(OpenSSL::Crypto openssl_external)
endif()
