cmake_minimum_required (VERSION 2.8.11)
project (ioniq_lon_control)

find_package(Boost REQUIRED COMPONENTS thread system)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

add_executable(control_main control_main_timer.cpp)
target_link_libraries(control_main ${USED_LIBS})