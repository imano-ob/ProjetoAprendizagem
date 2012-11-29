
# Recquires CMake 2.6 or greater.
cmake_minimum_required (VERSION 2.6)

# General definitions.
project (LandmarkDetector CXX C)
set (PROJECT_NAME LandmarkDetector)
set (PROJECT_BUILDNAME LandmarkDetector)

# Some output.
message ("=== ${PROJECT_NAME} ===")


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC "#define ISMAC")
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()


# CFLAGS: flags que voce queira enviar ao compilador
# LDFLAGS: flags que voce queira enviar ao ligador
set (CFLAGS "" CACHE STRING "flags enviadas ao compilador")
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    set (CFLAGS "${CFLAGS} -Wall -ansi -O3 -U_FORTIFY_SOURCE")
endif (UNIX OR MINGW)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set (LDFLAGS "${LDFLAGS} -mmacosx-version-min=10.5")
endif ()


include(/usr/share/opencv/OpenCVConfig.cmake)
FIND_PACKAGE(OpenCV REQUIRED ) 


# Important build locations.
set (SRC_DIR src)

SET(SRCS
    ./src/main.cc

)

include_directories ("src")


add_executable (${PROJECT_BUILDNAME} ${SRCS})

target_link_libraries (${PROJECT_BUILDNAME} ${OpenCV_LIBS})


set_target_properties (${PROJECT_BUILDNAME} PROPERTIES LINK_FLAGS "${LDFLAGS}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES COMPILE_FLAGS "${CFLAGS}")
set_target_properties (${PROJECT_BUILDNAME} PROPERTIES PROJECT_NAME "${PROJECT_NAME}")


