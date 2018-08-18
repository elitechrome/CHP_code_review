#include <boost/asio/io_service.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include <iostream>

#include "timer.h"

using namespace boost::chrono;
static high_resolution_clock::time_point start;

static microseconds us;

void timerCallback()
{
  us = duration_cast<boost::chrono::microseconds> (boost::chrono::high_resolution_clock::now() - start);
  std::cout << "timer took " << us.count() << "us " << "\n";
  start = high_resolution_clock::now();
}

int main()
{
  boost::asio::io_service service;

  timer t2(20, &service, timerCallback);

  start = high_resolution_clock::now();
  t2.start();

  service.run();
} 