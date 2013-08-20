/*
    Copyright (c) 2013, <copyright holder> <email>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef CARPI_H
#define CARPI_H

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sinks/basic_sink_frontend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/thread.hpp>

namespace Pi {
	
	namespace logging = boost::log::BOOST_LOG_VERSION_NAMESPACE;
	namespace attr = boost::log::BOOST_LOG_VERSION_NAMESPACE::attributes;
	namespace expr = boost::log::BOOST_LOG_VERSION_NAMESPACE::expressions;
	namespace sinks = boost::log::BOOST_LOG_VERSION_NAMESPACE::sinks;
	namespace src = boost::log::BOOST_LOG_VERSION_NAMESPACE::sources;
	namespace keywords = boost::log::BOOST_LOG_VERSION_NAMESPACE::keywords;

	typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;

	class CarPi
	{
	private:
		boost::shared_ptr< sink_t > sink_gps;
		boost::shared_ptr< sink_t > sink_net;
		boost::shared_ptr< sink_t > sink_odb;
		
		/**
		* @brief Initializes the logging for the application
		* 
		* @return void
		*/
		void init_logging();
		
		void stop_logging();
		
		/**
		* @brief Sets up the collector for the Log file
		* 
		* @param sink_backend reference to a sink backend
		* @return void
		*/
		void init_logging_collecting(boost::shared_ptr<sinks::text_file_backend> sink_backend);
		
		void test_callback(const char* data, unsigned int length);	

	public:
		/**
		* @brief Constructor
		* 
		*/
		CarPi();
	
		/**
		* @brief Starts data aquistion threads
		* 
		* @return void
		*/ 
		void run();
	
		/**
		* @brief Stops all running threads
		* 
		* @return void
		*/
		void stop();
		
		/**
		* @brief Class destructor
		* 
		*/
		virtual ~CarPi();
	};
} // End namespace CarPi
#endif // CARPI_H
