cmake_minimum_required(VERSION 3.0)

project(asplib_contrib_KissFFT)
message(STATUS "Processing dependency KissFFT...")

if (NOT ASPLIB_DEPENDS_SRC_PREFIX)
  set(ASPLIB_DEPENDS_SRC_PREFIX ${CMAKE_BINARY_DIR}/src)
  set(ASPLIB_DEPENDS_DOWNLOAD TRUE)
else()
  set(ASPLIB_DEPENDS_DOWNLOAD FALSE)
endif()

include(add_KissFFT.cmake)
