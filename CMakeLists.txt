cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
file(GLOB SOURCES "src/*.cpp")

find_package(Boost 1.71.0 COMPONENTS filesystem REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(eset_app ${SOURCES})
    target_link_libraries(eset_app LINK_PUBLIC ${Boost_LIBRARIES} )
else()
    message(STATUS "Boost library is not available!")
endif()