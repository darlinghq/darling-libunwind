project(darling-libsystem_unwind)

cmake_minimum_required(VERSION 3.10)
enable_language(C ASM)

set(CMAKE_CXX_LINKER_PREFERENCE_PROPAGATES 0)
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LIBUNWIND_IS_NATIVE_ONLY -nostdinc -nostdinc++ -fno-rtti -fno-exceptions -funwind-tables -ggdb -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse -msse2 -msse3 -w -nostdinc -fno-rtti -funwind-tables -ggdb -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__DARWIN_UNIX03 -fPIC -w")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -nostdlib -Wl,-flat_namespace -Wl,-undefined,suppress")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libunwind/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libunwind/include)

set(LIBUNWIND_CXX_SOURCES
    libunwind/src/libunwind.cpp
    libunwind/src/Unwind-EHABI.cpp
    libunwind/src/Unwind-seh.cpp
	libunwind/src/Unwind_AppleExtras.cpp
)

set(LIBUNWIND_C_SOURCES
    libunwind/src/UnwindLevel1.c
    libunwind/src/UnwindLevel1-gcc-ext.c
    libunwind/src/Unwind-sjlj.c
)

set(LIBUNWIND_ASM_SOURCES
    libunwind/src/UnwindRegistersRestore.S
    libunwind/src/UnwindRegistersSave.S
)

set(LIBUNWIND_SOURCES
    ${LIBUNWIND_CXX_SOURCES}
    ${LIBUNWIND_C_SOURCES}
    ${LIBUNWIND_ASM_SOURCES}
)

set(DYLIB_INSTALL_NAME "/usr/lib/system/libunwind.dylib")
add_circular(unwind FAT
	SOURCES
		${LIBUNWIND_SOURCES}
	SIBLINGS
		system_c
		keymgr
)
#target_link_libraries(unwind PRIVATE system_c keymgr)
#set_target_properties(unwind PROPERTIES LINKER_LANGUAGE C)
#make_fat(unwind)

include(darling_static_lib)
add_darling_static_library(unwind_static SOURCES ${LIBUNWIND_SOURCES})
set_target_properties(unwind_static PROPERTIES COMPILE_FLAGS "-DNDEBUG=1")
make_fat(unwind_static)

install(TARGETS unwind DESTINATION libexec/darling/usr/lib/system)
