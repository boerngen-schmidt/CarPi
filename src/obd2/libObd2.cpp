/**************************************************************************
*   Copyright (C) 2010 by Michael Carpenter (malcom2073)                  *
*   mcarpenter@interforcesystems.com                                      *
*                                                                         *
*   This file is a part of libobd                                         *
*                                                                         *
*   libobd is free software: you can redistribute it and/or modify        *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation, either version 2 of        *
*   the License, or (at your option) any later version.                   *
*                                                                         *
*   libobd is distributed in the hope that it will be useful,             *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with libobd.  If not, see <http://www.gnu.org/licenses/>.       *
***************************************************************************/

#include "libObd2.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdarg>

void (*debugCallback)(const char *,void*,libObd2::DebugLevel) = NULL;
void (*commsCallback)(const char *,void*) = NULL;
void *debugUserData = NULL;
void *commsUserData = NULL;

// Implementation of Methods
libObd2::libObd2()
{
	m_lastError = NONE;
	debugCallback = 0;
	commsCallback = 0;
}

int libObd2::openPort(const char *portName)
{
	return openPort(portName,-1);
}
void libObd2::setDebugCallback(void (*callbackptr)(const char*,void*,libObd2::DebugLevel),void *usrdata)
{
	//printf("Calling setDebugCallback: %i\n",debugCallback);
	debugCallback = callbackptr;
	debugUserData = usrdata;
	//printf("Calling setDebugCallback: %i\n",debugCallback);
}
void libObd2::setCommsCallback(void (*callbackptr)(const char*,void*),void* usrdata)
{
	commsCallback = callbackptr;
	commsUserData = usrdata;
}
void libObd2::debug(DebugLevel lvl,const char* msg,...)
{
	char fmsg[4096];
	va_list vl;
	va_start(vl,msg);
	vsnprintf(fmsg,sizeof(fmsg),msg,vl);
	if (debugCallback != 0)
	{
		debugCallback(fmsg,debugUserData,lvl);
	}
	va_end(vl);
}
void libObd2::commsDebug(const char *msg)
{
	if (commsCallback != 0)
	{
		commsCallback(msg,commsUserData);
	}
}

int libObd2::openPort(const char *portName,int baudrate)
{
	//NEED TO USE BAUD RATE HERE!!!: baudrate
	//printf("Attempting to open COM port\n");
	debug(libObd2::DEBUG_VERBOSE,"Attempting to open com port %s",portName);
	portHandle = open(portName,O_RDWR | O_NOCTTY | O_NDELAY);
	if (portHandle < 0)
	{
		//printf("Error opening Com: %s\n",portName);
		debug(libObd2::DEBUG_ERROR,"Error opening com port %s",portName);

		return -1;
	}
	//printf("Com Port Opened %i\n",portHandle);
	debug(libObd2::DEBUG_VERBOSE,"Com Port Opened %i",portHandle);
	fcntl(portHandle, F_SETFL, 0); //Set it to blocking. This is required? Wtf?
	//struct termios oldtio;
	struct termios newtio;
	//bzero(&newtio,sizeof(newtio));
	tcgetattr(portHandle,&newtio);
	long BAUD = B9600;
	switch (baudrate)
	{
		case 38400:
			BAUD = B38400;
			break;
		case 115200:
			BAUD  = B115200;
			break;
		case 19200:
			BAUD  = B19200;
			break;
		case 9600:
			BAUD  = B9600;
			break;
		case 4800:
			BAUD  = B4800;
			break;
		default:
			BAUD = B38400;
			break;
	}  //end of switch baud_rate
	if (strspn("/dev/pts",portName) >= 8)
	{
		debug(libObd2::DEBUG_WARN,"PTS Detected... disabling baud rate selection on: %s",portName);
		//printf("PTS detected... disabling baud rate selection: %s\n",portName);
		baudrate = -1;
	}
	else
	{
	}

	newtio.c_cflag |= (CLOCAL | CREAD);
	newtio.c_lflag &= !(ICANON | ECHO | ECHOE | ISIG);
	newtio.c_oflag &= !(OPOST);
	newtio.c_cc[VMIN] = 0;
	newtio.c_cc[VTIME] = 100;
/*
	newtio.c_cflag &= ~CSIZE; //Disable byte size
	newtio.c_cflag &= ~PARENB; //Disable parity
	newtio.c_cflag &= ~CSTOPB; //Disable stop bits
	newtio.c_cflag |= (CLOCAL | CREAD | CS8); //Set local mode, reader, and 8N1.

	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //Disable CANON, echo, and signals

	newtio.c_oflag &= ~(OPOST); //Disable post processing
*/
	if (baudrate != -1)
	{
		if(cfsetispeed(&newtio, BAUD))
		{
			perror("cfsetispeed");
		}

		if(cfsetospeed(&newtio, BAUD))
		{
			perror("cfsetospeed");
		}
		debug(libObd2::DEBUG_VERBOSE,"Setting baud rate to %i on port %s\n",baudrate,portName);
	}
	tcsetattr(portHandle,TCSANOW,&newtio);
	//newtio.c_cc[VMIN] = 0; //Minimum number of bytes to read
	//newtio.c_cc[VTIME] = 100; //Read Timeout (10.0 seconds)


	//tcflush(portHandle,TCIFLUSH);
	return 0;
}

void libObd2::setPortHandle(HANDLE hdnl)
{
	portHandle = hdnl;
}

int libObd2::closePort()
{
	#ifdef WINVER
	CloseHandle(portHandle);
	#else
//	tcsetattr(portHandle,TCSANOW,&oldtio);
	close(portHandle);
	#endif
	return 0;
}
int libObd2::initPort()
{
	sendObdRequest("atz\r",4);
	sendObdRequest("ati\r",4);
	sendObdRequest("ate0\r",5);
	usleep(3000000);
	tcflush(portHandle,TCIFLUSH);
	sendObdRequest("atl0\r",5);
	sendObdRequest("ath0\r",5);
	sendObdRequest("010C\r",5);
	return 1;
}
byte libObd2::byteArrayToByte(byte b1, byte b2)
{
	byte newB1 = 0;
	byte newB2 = 0;
	if ((b1 >= 48) && (b1 <= 57))
	{
		newB1 = ((b1 - 48) * 16);
	}
	else if ((b1 >=65) && (b1 <= 90))
	{
		newB1 = ((b1 - 55) * 16);
	}
	else
	{
		newB1 = ((b1 - 87) * 16);
	}
	if ((b2 >= 48) && (b2 <= 57))
	{
		newB2 = (b2 - 48);
	}
	else if ((b2 >= 65) && (b2 <= 90))
	{
		newB2 = (b2 - 55);
	}
	else
	{
		newB2 = (b2 - 87);
	}
	byte retVal = (newB1 + newB2);
	return retVal;
}

bool libObd2::sendObdRequest(const char *req,int len)
{
	//Blind request
	std::vector<byte> reply;
	return sendObdRequestString(req,len,&reply,-1,-1);
}
bool libObd2::sendObdRequest(const char *req,int len,int sleeptime)
{
	std::vector<byte> reply;
	return sendObdRequestString(req,len,&reply,sleeptime,-1);
}

bool libObd2::sendObdRequestString(const char *req,int length,std::vector<byte> *reply)
{
	return sendObdRequestString(req,length,reply,20,3);
}
bool libObd2::sendObdRequestString(const char *req,int length,std::vector<byte> *reply,int sleeptime)
{
	return sendObdRequestString(req,length,reply,sleeptime,3);
}

void libObd2::flush()
{
	tcflush(portHandle,TCIFLUSH);
}
std::string libObd2::monitorModeReadLine()
{
	std::string retval;
	int len=0;
	char *tmp = new char[1024];
	bool breakit = false;
	while (!breakit)
	{
		len = read(portHandle,tmp,1024);
		if (len < 0)
		{
			printf("No Read\n");
			perror("Error");
			delete[] tmp;
			m_lastError = SERIALREADERROR;
			debug(libObd2::DEBUG_ERROR,"Serial read error");
			return std::string();
		}
		else if (len == 0)
		{
                    usleep(10000);
		}
		else
		{
			for (int i=0;i<len;i++)
			{
				if (tmp[i] == '\n')
				{
					breakit = true;
				}
				else
				{
					retval.append(1,tmp[i]);
				}
			}
		}
	}
	return retval;

}
bool libObd2::sendObdRequestString(const char *req,int length,std::vector<byte> *reply,int sleeptime, int timeout)
{
	reply->clear();
	//std::vector<byte> tmpReply;
	char *tmp = new char[1024];
	char *totalReply = new char[1024];
	int loc = 0;
	int len = 0;
	time_t seconds = time(NULL);
	commsDebug(req);
	len = write(portHandle,req,length);
	if (len < 0)
	{
		printf("No Write\n");
		//delete tmp;
		//delete totalReply;
		//m_lastError = SERIALWRITEERROR;
		//return false;
	}
	if (sleeptime == -1)
	{
		//Not expecting a reply.
		delete[] totalReply;
		delete[] tmp;
		return true;

	}
	usleep(sleeptime * 1000);
	bool continueLoop = true;
	while (continueLoop)
	{
		len = read(portHandle,tmp,1024);
		if (len < 0)
		{
			printf("No Read\n");
			perror("Error");
			delete[] tmp;
			delete[] totalReply;
			m_lastError = SERIALREADERROR;
			debug(libObd2::DEBUG_ERROR,"Serial read error");
			return false;
		}
		else if (len == 0)
		{
			usleep(10000);
		}
		else
		{
			for (int i=0;i<len;i++)
			{
				if (/*(tmp[i] != 0x20) && (tmp[i] != '\r') && (tmp[i] != '\n') && */(tmp[i] != '>'))
				{
					//printf("Byte: %c %i ",tmp[i],tmp[i]);
					totalReply[loc++] = tmp[i];
				}
				if (tmp[i] == '>')
				{
					/*printf("\n> returned. Current: %i, Len: %i\n",i,len);
					printf("\n\nResponse: ");
					for (int j=i;j<len;j++)
					{
						printf("%c",tmp[j]);
					}
					printf(" :End\n\n");
					*/
					continueLoop = false;
				}
			}
		}
		if (timeout > 0)
		{
			if ((time(NULL) - seconds)-(sleeptime / 1000.0) > timeout)
			{
				//printf("Time:%i:%i:%i\n",time(NULL) - seconds,(time(NULL) - seconds) - (sleeptime/1000.0),time(NULL));
				continueLoop = false;

				m_lastError = TIMEOUT;
				printf("Timeout, current reply state:");
				for (int i=0;i<loc;i++)
				{
					printf("%c",totalReply[i]);
				}

				printf(":\n");
				printf("Current reply length: %i\n",loc);
				delete[] tmp;
				delete[] totalReply;
				debug(libObd2::DEBUG_ERROR,"Timeout");
				return false;
			}
		}
		if (timeout == -1)
		{
			//Not waiting for a reply.
			delete[] tmp;
			delete[] totalReply;
			debug(libObd2::DEBUG_ERROR,"Timeout, not waiting for a reply");
			return true;
		}
	}

	//Perform checking on totalReply
	for (int i=0;i<loc;i++)
	{
		reply->push_back(totalReply[i]);
	}
	//printf("Total Reply Size: %i\n",loc);
	totalReply[loc] = '\0';
	commsDebug(totalReply);
	int errorlen = strspn("NODATA",totalReply);
	if (errorlen == 6)
	{
		//Error
		//printf("Error\n");
		//Nodata here
		m_lastError = NODATA;
		delete[] tmp;
		delete[] totalReply;
		debug(libObd2::DEBUG_ERROR,"nodata");
		return false;
	}
	delete[] totalReply;
	delete[] tmp;
	//tcflush(portHandle,TCIFLUSH);
	//debug(obdLib::DEBUG_ERROR,"Hunkydory");
	return true;
}
libObd2::ObdError libObd2::lastError()
{
	return m_lastError;
}
bool libObd2::sendObdRequest(const char *req,int length,std::vector<byte> *reply)
{
	return sendObdRequest(req,length,reply,100,5);
}


bool libObd2::sendObdRequest(const char *req,int length,std::vector<byte> *reply,int sleep, int timeout)
{
	reply->clear();
	std::vector<byte> tmpReply;
	if (!sendObdRequestString(req,length,&tmpReply,sleep,timeout))
	{
		debug(libObd2::DEBUG_ERROR,"sendObdRequestString returned false!!");
		return false;
	}
	if (tmpReply.size() == 0)
	{
		debug(libObd2::DEBUG_ERROR,"sendObdRequestString returned true with a tmpReply size of 0!");
		return false;
	}
	for (unsigned int i=0;i<tmpReply.size()-1;i++)
	{
		if ((tmpReply[i] != 0x20) && (tmpReply[i] != '\r') && (tmpReply[i] != '\n'))
		{
			reply->push_back(byteArrayToByte(tmpReply[i],tmpReply[i+1]));
			i++;
		}
	}
	return true;
}


extern "C" {
	STDCALL void *obdLibNew()
	{
		return new libObd2();
	}
	STDCALL int obdLibOpenPort(void *ptr,const char *portname,int baudrate)
	{
		libObd2 *lib = ((libObd2*)ptr);
		return lib->openPort(portname,baudrate);
	}
	STDCALL int obdLibClosePort(void *ptr)
	{
		libObd2 *lib = ((libObd2*)ptr);
		return lib->closePort();
	}
	STDCALL int obdLibInitPort(void *ptr)
	{
		libObd2 *lib = ((libObd2*)ptr);
		return lib->initPort();
	}
	STDCALL void obdLibDelete(void *ptr)
	{
		libObd2 *lib = ((libObd2*)ptr);
		delete lib;
	}

	//bool sendObdRequestString(const char *req,int length,std::vector<byte> *reply,int sleeptime, int timeout);
	//bool sendObdRequest(const char *req,int length,std::vector<byte> *reply,int sleep, int timeout);
	//bool sendObdRequest(const char *req,int length,std::vector<byte> *reply,int sleep, int timeout);
	STDCALL bool obdLibSendObdRequest(void *ptr,const char *req,int length, char *reply,unsigned int replylength,int sleeptime, int timeout)
	{
		libObd2 *lib = ((libObd2*)ptr);
		std::vector<byte> replyvect;
		if (!lib->sendObdRequest(req,length,&replyvect,sleeptime,timeout))
		{
			return false;
		}
		if (replyvect.size() > replylength)
		{
			//Not enough room in the buffer
			return false;
		}
		for (unsigned int i=0;i<replyvect.size();i++)
		{
			reply[i] = replyvect.at(i);
		}
		reply[replyvect.size()] = '.';
		return true;
	}
	STDCALL void setEcho(void *ptr,bool on)
	{
		libObd2 *lib = ((libObd2*)ptr);
		if (on)
		{
			lib->sendObdRequest("ate1\r",5,250);
		}
		else
		{
			lib->sendObdRequest("ate0\r",5,250);
		}

	}
	STDCALL bool obdLibSendObdRequestString(void *ptr,const char *req,int length, char *reply,unsigned int replylength,int *replylengthptr,int sleeptime, int timeout)
	{
		libObd2 *lib = ((libObd2*)ptr);
		std::vector<byte> replyvect;
		if (!lib->sendObdRequestString(req,length,&replyvect,sleeptime,timeout))
		{
			reply[0] = -1;
			reply[1] = 1;
			reply[2] = lib->lastError();
			return false;
		}
		//char *tmp = new char(1024);
		//sprintf(tmp,"ObdRequest returned! Size: %i",replyvect.size());
		//OutputDebugString(tmp);
		//fprintf(stderr,"ObdRequest returned! Size %i\n",replyvect.size());
		if (replyvect.size() > replylength)
		{
			//Not enough room in the buffer
			reply[0] = -1;
			reply[1] = 2;
			return false;
		}
		for (unsigned int i=0;i<replyvect.size();i++)
		{
			reply[i] = replyvect.at(i);
		}
		//reply[replyvect.size()]='.';
		*replylengthptr = replyvect.size();
		if (replyvect.size() == 0)
		{
			reply[0] = lib->lastError();
		}
		else
		{
			//reply[0] = replyvect.size();
		}
		return true;
	}
}
