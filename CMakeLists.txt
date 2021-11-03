cmake_minimum_required(VERSION 3.16)

project(ENSEI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

option(ENSEI_FETCH_THIRDPARTY "Download Thirdparty Library" ON)
if(ENSEI_FETCH_THIRDPARTY)
    include(FetchContent)
    mark_as_advanced(FETCHCONTENT_BASE_DIR)
    mark_as_advanced(FETCHCONTENT_FULLY_DISCONNECTED)
    mark_as_advanced(FETCHCONTENT_UPDATES_DISCONNECTED)
    mark_as_advanced(FETCHCONTENT_QUIET)
    mark_as_advanced(SEAL_BUILD_DEPS)
    mark_as_advanced(SEAL_BUILD_TESTS)
    mark_as_advanced(SEAL_BUILD_BENCH)
    set(ENSEI_THIRDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty)
    message(${ENSEI_THIRDPARTY_DIR})
    set(FETCHCONTENT_BASE_DIR ${ENSEI_THIRDPARTY_DIR} CACHE STRING "" FORCE)
    set(THIRDPARTY_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty)
    message(STATUS "SEAL: download ...")
    FetchContent_Declare(
        seal
        GIT_REPOSITORY https://github.com/microsoft/SEAL.git
        GIT_TAG 6bfac481aae4057ea887d42eb0f24371e9b4c4f9 # 3.7.1
        )
    FetchContent_GetProperties(seal)
    if(NOT seal_POPULATED)
        FetchContent_Populate(seal)
        add_subdirectory(
            ${seal_SOURCE_DIR}
            EXCLUDE_FROM_ALL
            )
    endif()
else()
    find_package(SEAL 3.7 REQUIRED)
endif()
add_subdirectory(src)

