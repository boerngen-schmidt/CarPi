# src/ CMakeLists.txt file

# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR on)

# Create a variable called carpid_SOURCES containing all .cpp files:
set(carpi_SOURCES odb2_logger.cpp gps_logger.cpp carpi.cpp main.cpp)

# look for the boost library 
# http://stackoverflow.com/questions/6646405/how-do-you-add-boost-libraries-in-cmakelists-txt
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
add_definitions( -DBOOST_ALL_DYN_LINK )
find_package(Boost COMPONENTS log log_setup program_options system thread REQUIRED)
# find_package(Threads)

# Create an executable file called carpid from sources:
# Set boost include dirs and libraries
if(${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	add_subdirectory(serial)
	add_executable(carpid ${carpi_SOURCES})
	target_link_libraries(carpid ${Boost_LIBRARIES} serial pthread)
endif(${Boost_FOUND})