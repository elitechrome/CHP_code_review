#ifndef __TIMER__H__
#define __TIMER__H__
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>



struct timer
{
    timer(unsigned int delay, boost::asio::io_service* s_service, boost::function< void () > callback) :
    m_timer(*s_service),
    m_callback(callback),
    m_delay(delay)
    {
    }

    void start()
    {
        m_timeBase = boost::posix_time::microsec_clock::universal_time();
        do_start();
    }

    void do_start()
    {
        m_elapsed = boost::posix_time::microsec_clock::universal_time() - m_timeBase;
        m_timer.expires_from_now(m_estimated - m_elapsed +
        boost::posix_time::millisec(m_delay));
        m_timer.async_wait(boost::bind(&timer::handle_wait, this, _1));
    }

    void handle_wait(const boost::system::error_code& e)
    {
        m_estimated += boost::posix_time::millisec(m_delay);
        do_start();
        m_callback();
    }

    boost::asio::deadline_timer m_timer;
    boost::function< void () > m_callback;
    unsigned int m_delay;
    boost::posix_time::ptime m_timeBase;
    boost::posix_time::time_duration m_estimated, m_elapsed;
};
#endif