cmake_minimum_required(VERSION 3.10)

project(mddlib VERSION 0.1.0 LANGUAGES CXX)

#specify the C++ standard
set(CMAKE_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED True)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")



set(LOCAL_EXT_PREFIX_DIR ${PROJECT_SOURCE_DIR}/deps/local)       

########## external project ##################

include(ExternalProject)

ExternalProject_Add(gzstreamlib
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  URL ${PROJECT_SOURCE_DIR}/deps/gzstream.tgz
  INSTALL_DIR ${LOCAL_EXT_PREFIX_DIR}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND  make 
  INSTALL_COMMAND  cp gzstream.h <INSTALL_DIR>/include && cp libgzstream.a <INSTALL_DIR>/lib
  )
set_target_properties(gzstreamlib PROPERTIES EXCLUDE_FROM_ALL TRUE)


ExternalProject_Add(spdloglib
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.7.0
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND  mkdir -p build && cd build && cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>  ..  
  INSTALL_COMMAND  cd build && make install 
  )
set_target_properties(spdloglib PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(libzmq
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  GIT_REPOSITORY https://github.com/zeromq/libzmq.git
  GIT_TAG v4.3.2
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND  mkdir -p build && cd build && cmake  -DBUILD_SHARED=OFF -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>  ..  
  INSTALL_COMMAND  cd build && make install 
  )
set_target_properties(libzmq PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(zmqpplib
  DEPENDS libzmq
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  GIT_REPOSITORY https://github.com/zeromq/zmqpp.git
  GIT_TAG 4.2.0
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local
  BUILD_IN_SOURCE 1
  PATCH_COMMAND git apply ${PROJECT_SOURCE_DIR}/patch/zmqpp.patch
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND 
  make CXXFLAGS=-I<INSTALL_DIR>/include   libzmqpp.a   
  INSTALL_COMMAND make install PREFIX=<INSTALL_DIR> BUILD_SHARED=NO 
  )
set_target_properties(zmqpplib PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(snappylib
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  GIT_REPOSITORY https://github.com/google/snappy.git
  GIT_TAG 1.1.8
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND  mkdir -p build && cd build && cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> .. && make 
  INSTALL_COMMAND  cd build && make install 
  )
set_target_properties(snappylib PROPERTIES EXCLUDE_FROM_ALL TRUE)
  
ExternalProject_Add(lz4lib
  PREFIX ${PROJECT_SOURCE_DIR}/deps
  GIT_REPOSITORY https://github.com/lz4/lz4.git
  GIT_TAG v1.9.2
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND  "" 
  BUILD_COMMAND  ""  
  INSTALL_COMMAND  cd lib && make install BUILD_SHARED=no PREFIX=<INSTALL_DIR> 
  )
set_target_properties(lz4lib PROPERTIES EXCLUDE_FROM_ALL TRUE)


ExternalProject_Add(boostlib
  PREFIX ${PROJECT_SOURCE_DIR}/deps/boost
  URL https://dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0.tar.gz
  URL_MD5 4036cd27ef7548b8d29c30ea10956196
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND
  ./bootstrap.sh
  --with-libraries=filesystem
  --with-libraries=program_options
  --with-libraries=system
  --with-libraries=iostreams
  --with-libraries=date_time
  --with-libraries=random
  --with-libraries=test
  --prefix=<INSTALL_DIR>
  BUILD_COMMAND
  C_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/deps/local/include
  CPLUS_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/deps/local/include
  LIBRARY_PATH=${PROJECT_SOURCE_DIR}/deps/local/lib
  ./b2 install link=static variant=release threading=multi runtime-link=static
  INSTALL_COMMAND ""
  INSTALL_DIR ${PROJECT_SOURCE_DIR}/deps/local )
set_target_properties(boostlib PROPERTIES EXCLUDE_FROM_ALL TRUE)



############# test programs ###########################
	find_package(ZLIB REQUIRED)
	find_package(Threads REQUIRED)

	link_directories( "${LOCAL_EXT_PREFIX_DIR}/lib" )
	link_directories( "${LOCAL_EXT_PREFIX_DIR}/lib64" )

	add_executable(test_strutils src/test_strutils.cpp src/mdd/strutils.cpp)
	add_executable(test_sysutils src/test_sysutils.cpp src/mdd/sysutils.cpp src/mdd/strutils.cpp )	
	
	foreach(EXEC test_strutils test_sysutils)
		add_dependencies(${EXEC} boostlib)
		target_include_directories(${EXEC} PUBLIC "${LOCAL_EXT_PREFIX_DIR}/include" )
		target_link_libraries(${EXEC} PUBLIC boost_unit_test_framework)
	endforeach(EXEC)
	




