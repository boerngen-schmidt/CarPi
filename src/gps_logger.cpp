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
#include <boost/log/sources/record_ostream.hpp>
#include <boost/algorithm/string.hpp>
#include "gps_logger.h"
#include "log.h"

/** 
 * Example output and definition of 
 * 
 * +CGPSINFO:3543.459713,N,14003.475552,E,170813,110120.0,74.5,0,0
 * Latitude(degree[1-2(0-90)] minutes[2].seconds[6]):N|S:Logitude(degree[1-3(0-180)] minutes[2].seconds[6]):E|W,Date(DDMMYY),TimeUTC(hhmmss.s),Altitude MSL,Speed knots
 * http://www.geomidpoint.com/latlon.html
 * 
 * Regex
 * ((\d{1,3})(\d{2})\.(\d{6})),[N|S],(\d{1,3})(\d{2})\.(\d{6}),[W|E],(\d\d)(\d\d)(\d\d),(\d\d)(\d\d)(\d\d\.\d)
*/

Pi::GPS::GPSLogger::GPSLogger()
{
	lg.add_attribute("Source", attr::constant<Pi::log::source>(Pi::log::GPS));
}

Pi::GPS::GPSLogger::~GPSLogger()
{

}

void Pi::GPS::GPSLogger::setDevice(CallbackAsyncSerial* device){
	dev = device;
}

void Pi::GPS::GPSLogger::startPolling(unsigned int seconds)
{
	std::string end = "\r\n";
	

	
	for(int i = 0; i < 5; i++) {
		if (dev->errorStatus() || dev->isOpen() == false) {
			std::cerr << "Serial port unexpectedly closed" << std::endl;
		}
		dev->writeString("AT+CGPSINFO" + end);
		sleep(seconds);
	}
	
}

void Pi::GPS::GPSLogger::recievedDataCallback(const char* data, size_t length)
{
	std::string str(data, data+length);
	response += str;
	boost::replace_all(response, "\n", "");
	boost::replace_all(response, "\r", "\n");
	// read Until OK or ERROR
	size_t pos;
	if ((pos = response.find("OK")) >= 0){
		// Also remove \r\n
		std::string completeAnswer = response.substr(0, pos + 2);
		// remove complete answer
		boost::erase_head(response, pos + 2);
		
		std::cout << "completeAnswer: " << completeAnswer << std::endl;
		std::cout << "answer: " << response << std::endl;
		
		boost::log::record rec = lg.open_record();
		if (rec) {
			boost::log::record_ostream strm(rec);
			strm << completeAnswer;
			strm.flush();
			lg.push_record(boost::move(rec));
		}
	}
	else if((pos = response.find("ERROR")) >= 0) {
		
	}
}

bool Pi::GPS::GPSLogger::setupDevice()
{
	dev->writeString("ATL0" + end);
	usleep(200000);
	dev->writeString("AT+CGPS=1,1" + end);
	usleep(200000);
	dev->writeString("ATE0" + end);
	usleep(200000);
}



