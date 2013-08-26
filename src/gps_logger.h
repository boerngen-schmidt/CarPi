/*
 * Copyright (c) 2013 Benjamin Börngen-Schmidt 
 *                       boerngen-schmidt@stud.tu-darmstadt.de
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#ifndef GPS_LOGGER_H
#define GPS_LOGGER_H

#include "serial/AsyncSerial.h"
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


namespace Pi {
	namespace  GPS {
		namespace logging = boost::log::BOOST_LOG_VERSION_NAMESPACE;
		namespace attr = boost::log::BOOST_LOG_VERSION_NAMESPACE::attributes;
		namespace src = boost::log::BOOST_LOG_VERSION_NAMESPACE::sources;
		/**
		* @brief sturcture describing position
		* 
		*/
		typedef struct position {
			int degree;
			int minutes;
			int seconds;
			int miliseconds;
			char direction;
		};

		typedef struct datetime {
			int year;
			int month;
			int day;
			int hours;
			int minutes;
			int seconds;
			int miliseconds;
		};

		typedef struct gps_info {
			position latitude;
			position logitude;
			double altitude; /* TODO check if double ok */
			double speed; /* TODO check if double ok */
			datetime date_time;
		};

		class GPSLogger
		{
		public:
			GPSLogger();
			virtual ~GPSLogger();
			
			void setDevice(CallbackAsyncSerial* device);
			void recievedDataCallback(const char* data, size_t length);
			void startPolling(unsigned int);
			void stopPolling();
			
		private:
			CallbackAsyncSerial* dev;
			src::logger lg;
			std::string response;
			
			bool run;
			bool setupDevice();

		};

	} // End namespace GPS
} // End namespace CarPi
#endif // GPS_LOGGER_H
