add_executable(ensei_bench)
target_sources(ensei_bench
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ensei_bench.cpp
    )


if(TARGET SEAL::seal)
    target_link_libraries(ensei_bench PRIVATE SEAL::seal)
elseif(TARGET SEAL::seal_shared)
    target_link_libraries(ensei_bench PRIVATE SEAL::seal_shared)
else()
    message(FATAL_ERROR "Cannot find target SEAL::seal or SEAL::seal_shared")
endif()
