cmake_minimum_required(VERSION 2.4)
project(TV CXX C)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Checking vectorization
file(MAKE_DIRECTORY ${TV_BINARY_DIR}/cmake/altivec)
file(MAKE_DIRECTORY ${TV_BINARY_DIR}/cmake/sse)

# Checking for AltiVec
try_compile(
	USE_ALTIVEC
	${TV_BINARY_DIR}/cmake/altivec
	${TV_SOURCE_DIR}/cmake/altivec
	TV_ALTIVEC
)
# Checking for SSE2
try_compile(
	USE_SSE2
	${TV_BINARY_DIR}/cmake/sse2
	${TV_SOURCE_DIR}/cmake/sse2
	TV_SSE2
)

if(USE_ALTIVEC)
message(STATUS "Found Altivec: -maltivec -mabi=altivec")
add_definitions(-maltivec -mabi=altivec)
endif(USE_ALTIVEC)

if(USE_SSE2)
message(STATUS "Found SSE2: -msse2")
add_definitions(-msse2)
endif(USE_SSE2)

if(NOT USE_ALTIVEC AND NOT USE_SSE2)
message(STATUC "No vectorized code is possible")
endif(NOT USE_ALTIVEC AND NOT USE_SSE2)

find_package(Qt4 REQUIRED)
find_package(LATEX)

set(QT_DONT_USE_QTGUI 1)
set(QT_USE_QTTEST 1)
include(${QT_USE_FILE})

install(
	DIRECTORY tv
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

add_subdirectory(doc)

enable_testing()

include_directories(
	${TV_SOURCE_DIR}
	${QT_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

add_subdirectory(test)
