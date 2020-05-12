cmake_minimum_required(VERSION 3.10)
project(words_count_2_0)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS
        locale filesystem system)

set(LibZip_LIBRARIES "")
include(CheckLibraryExists)
SET(LibZip_LIBRARY_PATH "")
check_library_exists(zip zip_open_from_source
        "${LibZip_LIBRARY_PATH}" LibZip_FOUND)
if(LibZip_FOUND)
    message("Found LibZip")
    list(APPEND LibZip_LIBRARIES "zip" )
    link_directories(${LibZip_LIBRARY_PATH})
endif()

include_directories(${LibZip_INCLUDE_DIRS})

add_executable(words_count_2_0 main.cpp queue.h functions.h config_read.h
        functions.cpp file_to_vecs.h file_to_vecs.cpp manager.h manager.cpp config_read.cpp)
target_link_libraries(words_count_2_0 LINK_PUBLIC ${Boost_LIBRARIES} Threads::Threads ${LibZip_LIBRARIES})
