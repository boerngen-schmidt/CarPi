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

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

CarPi::CarPi() {
	this->init_logging();
}

CarPi::~CarPi() {

}

void CarPi::run() {

}

void CarPi::init_logging() {
	boost::shared_ptr<logging::core> core = logging::core::get();
	typedef boost::shared_ptr<sinks::text_file_backend> sink_backend_t;

	// TODO setup sink for gps
	sink_backend_t backend_gps = boost::make_shared<sinks::text_file_backend>(
			keywords::file_name = "%Y-%m-%d_%H:%M:%s_gps_%N.log"
//	    keywords::time_based_rotation = sinks::file::rotation_at_time_interval();
					);
	boost::shared_ptr< sink_t > sink_gps(new sink_t(backend_gps));

	// TODO setup sink for network
	sink_backend_t backend_net = boost::make_shared<sinks::text_file_backend>(
			keywords::file_name = "%Y-%m-%d_%H:%M:%s_net_%N.log"
			//	    keywords::time_based_rotation = sinks::file::rotation_at_time_interval()
					);
	boost::shared_ptr< sink_t > sink_net(new sink_t(backend_net));

	// TODO setup sink for odb
	sink_backend_t backend_odb = boost::make_shared<sinks::text_file_backend>(
			keywords::file_name = "%Y-%m-%d_%H:%M:%s_odb_%N.log"
			//	    keywords::time_based_rotation = sinks::file::rotation_at_time_interval();
					);
	boost::shared_ptr< sink_t > sink_odb(new sink_t(backend_odb));

	// add sinks to logging core
	core->add_sink(sink_gps);
	core->add_sink(sink_net);
	core->add_sink(sink_odb);

}

