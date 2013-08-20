/*
 Copyright (c) 2013, Benjamin Boerngen-Schmidt boerngen-schmidt@stud.tu-darmstadt.de
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

#include "carpi.h"
#include "log.h"
#include "gps_logger.h"
#include "serial/AsyncSerial.h"
#include "odb2_logger.h"

// Init attibutes for logging
BOOST_LOG_ATTRIBUTE_KEYWORD(src_attr, "Source", Pi::log::source)

Pi::CarPi::CarPi() {
	this->init_logging();
}

Pi::CarPi::~CarPi() {
	
}

/**
 * @brief Starts all sub threads for the data aquisiton
 * 
 * @return void
 */
void Pi::CarPi::run() {
	try {
		CallbackAsyncSerial* shield = new CallbackAsyncSerial("/dev/ttyAMA0", 115200);
		// Create the Logger Class
		Pi::GPS::GPSLogger gps;
		
		// set the Callback form the logger class
		shield->setCallback(boost::bind(&Pi::GPS::GPSLogger::recievedDataCallback, gps, _1, _2));
		
		// final class set up and go
		gps.setDevice(boost::ref(shield));
		gps.startPolling(5);
		
		// remove callback since gps will be destroyed
		shield->clearCallback();
	}
	catch(std::exception& e) {
		std::cerr<<"Exception: "<<e.what()<<std::endl;
	}
// 	shield->setCallback(&CarPi::CarPi::test_callback);
	
	// Connection to the car
	// CallbackAsyncSerial car("/dev/ttyODB2", 38400);
// 	car->setCallback(ODB2Logger::recievedDataCallback);
	
	boost::thread_group tg();
	// Start adding threads
	
// 	ODB2Logger odb2_logger(car);
// 	tg.create_thread<ODB2Logger>(&odb2_logger.startPolling, 5000);
}

void Pi::CarPi::init_logging() 
{
	boost::shared_ptr<logging::core> core = logging::core::get();
	typedef boost::shared_ptr<sinks::text_file_backend> sink_backend_t;

	// setup sink for GPS
	sink_backend_t backend_gps = boost::make_shared< sinks::text_file_backend >(
		keywords::file_name = "%Y-%m-%d_gps_%N.log",
	    keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(5)),
		keywords::auto_flush = true
	);
	init_logging_collecting(boost::ref<sink_backend_t>(backend_gps));
	backend_gps->scan_for_files();
	sink_gps = boost::make_shared< sink_t >(backend_gps);
	sink_gps->set_filter(expr::has_attr(src_attr) && src_attr == log::GPS);
	/// TODO Format
	
	
	// setup sink for NETWORK
	sink_backend_t backend_net = boost::make_shared< sinks::text_file_backend >(
		keywords::file_name = "%Y-%m-%d_%H:%M:%s_net_%N.log",
		keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(5))
	);
	init_logging_collecting(boost::ref<sink_backend_t>(backend_net));
	backend_net->scan_for_files();
	sink_net = boost::make_shared< sink_t >(backend_net);
	sink_net->set_filter(expr::has_attr(src_attr) && src_attr == log::NETWORK);
	/// TODO Format

	// setup sink for ODB
	sink_backend_t backend_odb = boost::make_shared< sinks::text_file_backend >(
		keywords::file_name = "%Y-%m-%d_%H:%M:%s_odb_%N.log",
		keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(5))
	);
	init_logging_collecting(boost::ref<sink_backend_t>(backend_odb));
	backend_odb->scan_for_files();
	sink_odb = boost::make_shared< sink_t >(backend_odb);
	sink_odb->set_filter(expr::has_attr(src_attr) && src_attr == log::ODB);
	/// TODO Format

	// add sinks to logging core
	core->add_sink(sink_gps);
	core->add_sink(sink_net);
	core->add_sink(sink_odb);
}

void Pi::CarPi::init_logging_collecting(boost::shared_ptr<sinks::text_file_backend> sink_backend)
{
	sink_backend->set_file_collector(sinks::file::make_collector(
		keywords::target = "logs",
		keywords::min_free_space = 100 * 1024 * 1024
	));
}

void Pi::CarPi::stop_logging()
{
	boost::shared_ptr< logging::core > core = logging::core::get();
	
	// No more sinks, no more logging
	core->remove_all_sinks();
	
	sink_gps->flush();
	sink_net->flush();
	sink_odb->flush();
}

void Pi::CarPi::stop()
{

}

void Pi::CarPi::test_callback(const char* data, unsigned int length)
{
	std::vector<char> v(data,data+length);

	for(unsigned int i=0; i < v.size(); i++) {
		if(v[i]=='\n') {
			std::cout << std::endl;
		} 
		else {
			if(v[i]<32 || v[i]>=0x7f) {
				std::cout.put(' '); //Remove non-ascii char
			} 
			else { 
				std::cout.put(v[i]);
			}
		}
	}
	std::cout.flush(); //Flush screen buffer
}
