cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project (ioniq_lon_control)

find_package(Boost REQUIRED COMPONENTS system thread chrono)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_CHRONO_LIBRARY})

add_executable(control_main control_main.cpp)
target_link_libraries(control_main ${USED_LIBS})

add_executable(timer_example timer_example.cpp)
target_link_libraries(timer_example ${USED_LIBS})