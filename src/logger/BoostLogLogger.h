/* 
 * File:   BoostLogLogger.h
 * Author: Benjamin
 *
 * Created on 2. Juli 2013, 16:41
 */

#ifndef BOOSTLOGLOGGER_H
#define	BOOSTLOGLOGGER_H

#include "Logger.h"
#include <boost/log/common.hpp>

namespace bl = boost::log;

class BoostLogLogger : Logger {
public:
    BoostLogLogger();
    virtual ~BoostLogLogger();
private:
    init();

};

#endif	/* BOOSTLOGLOGGER_H */

