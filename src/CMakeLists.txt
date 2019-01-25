add_library(list src/list.c)
target_include_directories(list PUBLIC include)
target_include_directories(list PRIVATE src)

if(${REPLACE_MALLOC})
    target_compile_options(list PRIVATE -include  /usr/local/include/CppUTest/MemoryLeakDetectorMallocMacros.h)
    target_link_libraries(list CppUTest CppUTestExt)
endif()

install(TARGETS list ${CMAKE_INSTALL_PREFIX})
install(FILES include/list/list.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/list)
