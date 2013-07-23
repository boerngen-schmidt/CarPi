#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=bcm2708hardfp-gcc
CCC=arm-bcm2708hardfp-linux-gnueabi-g++
CXX=arm-bcm2708hardfp-linux-gnueabi-g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_Raspberry-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_Raspberry
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/CarPi.o \
	${OBJECTDIR}/src/logger/BoostLogLogger.o \
	${OBJECTDIR}/src/obd2/libObd2.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/carpi.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/carpi.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/carpi ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/CarPi.o: src/CarPi.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CarPi.o src/CarPi.cpp

${OBJECTDIR}/src/logger/BoostLogLogger.o: src/logger/BoostLogLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/logger
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/logger/BoostLogLogger.o src/logger/BoostLogLogger.cpp

${OBJECTDIR}/src/obd2/libObd2.o: src/obd2/libObd2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/obd2
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/obd2/libObd2.o src/obd2/libObd2.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/carpi.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
