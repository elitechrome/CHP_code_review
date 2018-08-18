add_executable(control_main src/control_main.cpp)
target_link_libraries(control_main ${BOOST_LIBS})

add_executable(timer_example timer_example.cpp)
target_link_libraries(timer_example ${BOOST_LIBS})