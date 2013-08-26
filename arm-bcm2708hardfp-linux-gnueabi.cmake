include(CMakeForceCompiler)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR "armv6zk")

# specify the cross compiler
SET(CMAKE_C_COMPILER /home/benjamin/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /home/benjamin/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-g++)
SET(CMAKE_FIND_ROOT_PATH /home/benjamin/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/)

SET(BOOST_INCLUDEDIR /home/benjamin/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/include)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)