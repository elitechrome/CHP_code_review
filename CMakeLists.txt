cmake_minimum_required (VERSION 3.1)
project (ioniq_lon_control)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system thread chrono)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_CHRONO_LIBRARY})

include_directories(include)

add_subdirectory(src)