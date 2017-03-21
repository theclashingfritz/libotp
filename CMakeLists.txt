cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(libotp)

find_package(PythonLibs)
if (NOT PYTHONLIBS_FOUND)
    message(FATAL_ERROR "Python libs not found.")
endif()

find_package(PythonInterp)
if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found.")
endif()

if(PYTHON_VERSION_MAJOR EQUAL 2)
    set(PYTHON2_EXECUTABLE "${PYTHON_EXECUTABLE}")
else()
    set(PYTHON2_EXECUTABLE python2)
endif()

### Panda3D dir ###
SET(PANDA_DIR "C:\\Panda3D-1.10.0" CACHE STRING "Panda3D directory.")
execute_process(COMMAND ${PYTHON2_EXECUTABLE} "${CMAKE_SOURCE_DIR}/src/run_interrogate.py" "${PANDA_DIR}")

### Define core components ###
set(BASE_FILES
    src/base/config_libotp.h
    src/base/config_libotp.cxx
    src/base/config_libotp.N
    src/base/util.h
)

set(COMPONENTS_FILES
    src/components/CMoverGroup.cxx
    src/components/CMoverGroup.h
    src/components/CMover.cxx
    src/components/CMover.h
    src/components/CImpulse.cxx
    src/components/CImpulse.h
)

set(NAMETAG_FILES
    src/nametag/ChatBalloon.cxx
    src/nametag/ChatBalloon.h
    src/nametag/NametagGlobals.cxx
    src/nametag/NametagGlobals.h
    src/nametag/NametagGroup.cxx
    src/nametag/NametagGroup.h
    src/nametag/Nametag.cxx
    src/nametag/Nametag.h
    src/nametag/Nametag2d.cxx
    src/nametag/Nametag2d.h
    src/nametag/Nametag3d.cxx
    src/nametag/Nametag3d.h
    src/nametag/NametagFloat2d.cxx
    src/nametag/NametagFloat2d.h
    src/nametag/NametagFloat3d.cxx
    src/nametag/NametagFloat3d.h
)

set(MARGINS_FILES
    src/margins/ClickablePopup.cxx
    src/margins/ClickablePopup.h
    src/margins/MarginCell.cxx
    src/margins/MarginCell.h
    src/margins/MarginManager.cxx
    src/margins/MarginManager.h
    src/margins/MarginPopup.cxx
    src/margins/MarginPopup.h
    src/margins/WhisperPopup.cxx
    src/margins/WhisperPopup.h
)

set(SETTINGS_FILES
    src/settings/Settings.cxx
    src/settings/Settings.h
)

set(IGATE_FILES
    src/base_igate.cxx
    src/components_igate.cxx
    src/nametag_igate.cxx
    src/margins_igate.cxx
    src/settings_igate.cxx
    src/libotp_module.cxx
)

include_directories(src/base)
include_directories(src/nametag)
include_directories(src/margins)
include_directories(src/settings)
include_directories(src/components)
include_directories(${PANDA_DIR}/include)
include_directories(${PYTHON_INCLUDE_DIR})
link_directories(${PANDA_DIR}/lib)

set(LIBPFX "")
if(WIN32)
    set(LIBPFX "lib")
    add_definitions(/wd4275)
    add_definitions(/EHa)
    add_definitions(/GF)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    execute_process(COMMAND "${CMAKE_C_COMPILER}" -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    message(STATUS "GCC Version: ${GCC_VERSION}")
    if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        add_definitions(-std=c++11) # Enable the new C++ standard
    else()
        add_definitions(-std=c++0x) # Enable the new C++ standard
    endif()
endif()

add_definitions(-DBUILDING_LIBOTP)

set(PANDA_LIBS
    ${LIBPFX}p3framework
    ${LIBPFX}panda
    ${LIBPFX}pandafx
    ${LIBPFX}pandaexpress
    ${LIBPFX}p3dtool
    ${LIBPFX}p3dtoolconfig
    ${LIBPFX}p3direct
    ${LIBPFX}p3pandatoolbase
)

# Add p3interrogatedb, if it exists
find_library(HAVE_P3INTERROGATEDB
             ${LIBPFX}p3interrogatedb
             HINTS ${PANDA_DIR}/lib)
             
if (HAVE_P3INTERROGATEDB)
    message(STATUS "libp3interrogatedb found")
    LIST(APPEND PANDA_LIBS ${LIBPFX}p3interrogatedb)
else()
    message(STATUS "libp3interrogatedb not found")
endif()

add_library(libotp SHARED
    ${BASE_FILES}
    ${COMPONENTS_FILES}
    ${NAMETAG_FILES}
    ${MARGINS_FILES}
    ${SETTINGS_FILES}
    ${IGATE_FILES}
)

if(WIN32)
    SET_TARGET_PROPERTIES(libotp PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(libotp ${PANDA_LIBS} ${PYTHON_LIBRARY})

add_custom_command(TARGET libotp
                   PRE_BUILD
                   COMMAND ${PYTHON2_EXECUTABLE} "${CMAKE_SOURCE_DIR}/src/run_interrogate.py" "${PANDA_DIR}"
                   COMMENT "Interrogate")

### Setup source groups and defines for configurables ###
source_group("base" FILES ${BASE_FILES})
source_group("components" FILES ${COMPONENTS_FILES})
source_group("nametag" FILES ${NAMETAG_FILES})
source_group("margins" FILES ${MARGINS_FILES})
source_group("settings" FILES ${SETTINGS_FILES})
source_group("interrogate_files" FILES ${IGATE_FILES})
