find_package(iterator REQUIRED)

add_library(list src/list.c)
target_include_directories(list PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/list>
        )
target_include_directories(list PRIVATE src)
target_link_libraries(list iterator)

if(${REPLACE_MALLOC})
    target_compile_options(list PRIVATE -include  /usr/local/include/CppUTest/MemoryLeakDetectorMallocMacros.h)
    target_link_libraries(list CppUTest CppUTestExt)
endif()

install(TARGETS list
        EXPORT list-targets
        PUBLIC_HEADER DESTINATION include/list
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES include/list/list.h DESTINATION include/list)
install(EXPORT list-targets
        FILE list-config.cmake
        DESTINATION lib/cmake/list)