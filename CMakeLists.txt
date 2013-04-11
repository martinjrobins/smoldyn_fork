# Main CMakeLists.txt file to build Smoldyn, its libraries, and its utilities using CMake.
# Written by Steve Andrews, April 16, 2012.
# This file, and other files of the Smoldyn project, are licensed with LGPL license.
# The primary Smoldyn web site is http://www.smoldyn.org.
# Please report bugs and problems to support@smoldyn.org.

########## Basic setup ##########

project(smoldyn)
cmake_minimum_required(VERSION 2.6)
set(SMOLDYN_VERSION "2.29")


######### Targets to build ###########

option(OPTION_TARGET_SMOLDYN "Create stand-alone Smoldyn program" ON)
option(OPTION_TARGET_LIBSMOLDYN "Create LibSmoldyn library" OFF)


####### Compiling options ##########

option(OPTION_VCELL "Compile Smoldyn for VCell" OFF)
option(OPTION_STATIC "Compile Smoldyn with static libraries" OFF)
option(OPTION_MINGW "Cross-compile for Windows using MinGW compiler" OFF)
option(OPTION_USE_OPENGL "Build with OpenGL support" ON)
option(OPTION_USE_LIBMZR "Include Libmoleculizer module" OFF)

if (OPTION_VCELL) 
	set(OPTION_USE_LIBTIFF OFF)
	set(OPTION_USE_ZLIB OFF)
	set(HAVE_ZLIB TRUE)
	set(OPTION_USE_ICONV OFF)
	set(HAVE_ICONV FALSE)
elseif(OPTION_USE_LIBMZR)
	option(OPTION_USE_LIBTIFF "Build with LibTiff support" ON)
	option(OPTION_USE_ZLIB "Build with Zlib support" ON)
	option(OPTION_USE_ICONV "Build with Libiconv support" ON)
else()
	option(OPTION_USE_LIBTIFF "Build with LibTiff support" ON)
	option(OPTION_USE_ZLIB "Build with Zlib support" OFF)
	option(OPTION_USE_ICONV "Build with Libiconv support" OFF)
endif()


######### Core code information ###########

set(HEADER_FILES
	source/lib/Geometry.h
	source/lib/math2.h
	source/lib/opengl2.h
	source/lib/parse.h
	source/lib/queue.h
	source/lib/random2.h
	source/lib/Rn.h
	source/lib/RnSort.h
	source/lib/rxnparam.h
	source/lib/SimCommand.h
	source/lib/Sphere.h
	source/lib/string2.h
	source/lib/SurfaceParam.h
	source/lib/Zn.h
	source/lib/SFMT/SFMT.h
	source/Smoldyn/smoldyn.h
	source/Smoldyn/smoldynfuncs.h
	source/vcell/SimpleMesh.h
	source/vcell/SimpleValueProvider.h
)

set(SRC_FILES
	source/lib/Geometry.c
	source/lib/math2.c
	source/lib/opengl2.c
	source/lib/parse.c
	source/lib/queue.c
	source/lib/random2.c
	source/lib/Rn.c
	source/lib/RnSort.c
	source/lib/rxnparam.c
	source/lib/SimCommand.c
	source/lib/Sphere.c
	source/lib/string2.c
	source/lib/SurfaceParam.c
	source/lib/Zn.c
	source/lib/SFMT/SFMT.c
	source/Smoldyn/smolboxes.c
	source/Smoldyn/smolcmd.c
	source/Smoldyn/smolcomparts.c
	source/Smoldyn/smolfilament.c
	source/Smoldyn/smolgraphics.c
	source/Smoldyn/smolmolec.c
	source/Smoldyn/smolmoleculizer.c
	source/Smoldyn/smolport.c
	source/Smoldyn/smolreact.c
	source/Smoldyn/smolsim.c
	source/Smoldyn/smolsurface.c
	source/Smoldyn/smolthread.c
	source/Smoldyn/smolwall.c
)

set(MAIN_FILES source/Smoldyn/smoldyn.c)

if(OPTION_VCELL)
	list(APPEND SRC_FILES
		source/Smoldyn/smoldynhybrid.c
		source/vcell/SimpleMesh.cpp
		source/vcell/SimpleValueProvider.cpp )
endif(OPTION_VCELL)

if(OPTION_TARGET_LIBSMOLDYN)
	list(APPEND HEADER_FILES source/Smoldyn/libsmoldyn.h)
	list(APPEND SRC_FILES source/Smoldyn/libsmoldyn.c)
endif(OPTION_TARGET_LIBSMOLDYN)

set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE CXX )
set_source_files_properties(${MAIN_FILES} PROPERTIES LANGUAGE CXX )

include_directories(source/lib source/Smoldyn source/vcell ${CMAKE_BINARY_DIR})



####### Build for debugging or release ##########

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the buid type: None, Debug, Release, RelWithDebInfo, or MinSizeRel" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")


####### Option: Compile with MinGW ##########

if (OPTION_MINGW)
	set (MINGWDIR /opt/local/i386-mingw32)
	message(STATUS "Cross-compiling for Windows using MinGW")
endif(OPTION_MINGW)


######### Platform dependent information ##########

if (OPTION_VCELL)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows/glut-3.7.6)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/vcell ${CMAKE_CURRENT_SOURCE_DIR}/windows/glut-3.7.6)
	if (ARCH_64bit)
		list(APPEND DEP_LIBS glut64)
	else()
		list(APPEND DEP_LIBS glut32 zlib)
	endif()
elseif (OPTION_MINGW)
	include_directories(${MINGWDIR}/include windows)
	link_directories(${MINGWDIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/windows/dll)
	list(APPEND DEP_LIBS opengl32 glu32 freeglut z)
endif()


####### Option: Build with OpenGL ##########

if (OPTION_USE_OPENGL AND OPTION_VCELL)
	set(HAVE_OPENGL TRUE)
	set(HAVE_GL_GLUT_H TRUE)
	set(HAVE_GL_GLU_H TRUE)
	set(HAVE_GL_GL_H TRUE)
elseif(OPTION_USE_OPENGL)
	if(OPTION_MINGW)
		find_file(HAVE_GL_GL_H GL/gl.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_file(HAVE_OPENGL_GL_H OpenGL/gl.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_file(HAVE_GL_GLU_H GL/glu.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_file(HAVE_OPENGL_GLU_H OpenGL/glu.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_file(HAVE_GL_GLUT_H GL/glut.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_file(HAVE_GLUT_GLUT_H GLUT/glut.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)

		find_path(OPEN_GL_INCLUDE_DIR gl.h PATHS ${MINGWDIR}/include/GL NO_DEFAULT_PATH)
		find_path(GLUT_INCLUDE_DIR glut.h PATHS ${MINGWDIR}/include/GL NO_DEFAULT_PATH)
		find_library(OPEN_GL_LIBRARIES libopengl32.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
		find_library(GLUT_LIBRARIES libfreeglut.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
	else()
		find_file(HAVE_GL_GL_H GL/gl.h)
		find_file(HAVE_OPENGL_GL_H OpenGL/gl.h)
		find_file(HAVE_GL_GLU_H GL/glu.h)
		find_file(HAVE_OPENGL_GLU_H OpenGL/glu.h)
		find_file(HAVE_GL_GLUT_H GL/glut.h)
		find_file(HAVE_GLUT_GLUT_H GLUT/glut.h)

		include(FindOpenGL)
		include(FindGLUT)
	endif()

	if(OPENGL_FOUND OR (OPEN_GL_INCLUDE_DIR AND OPEN_GL_LIBRARIES))
		set(OPENGL_FOUND TRUE)
		message(STATUS "OpenGL found: '${OPEN_GL_INCLUDE_DIR}', '${OPEN_GL_LIBRARIES}'")
	else()
		set(OPENGL_FOUND FALSE)
		message(FATAL_ERROR "OpenGL not found.  Building without OpenGL is possible.")
	endif()

	if(GLUT_FOUND OR (GLUT_INCLUDE_DIR AND GLUT_LIBRARIES))
		set(GLUT_FOUND TRUE)
		message(STATUS "Glut found: '${GLUT_INCLUDE_DIR}', '${GLUT_LIBRARIES}'")
	else()
		set(GLUT_FOUND FALSE)
		message(FATAL_ERROR "Glut not found.  Building without OpenGL is possible.")
	endif()

	set(HAVE_OPENGL TRUE)
	include_directories(${OPEN_GL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
	list(APPEND DEP_LIBS ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif()


####### Option: Build with LibTiff ##########

if(OPTION_USE_LIBTIFF)
	if(NOT HAVE_OPENGL)
		message(WARNING "LibTiff won't be used because build does not include OpenGL.")
	endif()

	if(OPTION_MINGW)
		find_path(TIFF_INCLUDE_DIR tiff.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_library(TIFF_LIBRARY libtiff.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
	elseif(OPTION_STATIC)
		find_path(TIFF_INCLUDE_DIR tiff.h)
		find_library(TIFF_LIBRARY libtiff.a)
	else()
		FIND_PACKAGE(TIFF)
	endif()

	if(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
		set(HAVE_LIBTIFF TRUE)
		message(STATUS "Found Libtiff: '${TIFF_INCLUDE_DIR}', '${TIFF_LIBRARY}'")
	else()
		set(HAVE_LIBTIFF FALSE)
		message(FATAL_ERROR "Libtiff not found")
	endif()

	include_directories(${TIFF_INCLUDE_DIR})
	list(APPEND DEP_LIBS ${TIFF_LIBRARY})
endif(OPTION_USE_LIBTIFF)


####### Option: Build with Zlib ##########

if(OPTION_USE_ZLIB)
	if(OPTION_MINGW)
		find_path(ZLIB_INCLUDE_DIRS zlib.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_library(ZLIB_LIBRARIES libz.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
	elseif(OPTION_STATIC)
		find_path(ZLIB_INCLUDE_DIRS zlib.h)
		find_library(ZLIB_LIBRARIES libz.a)
	else()
		include(FindZlib)
	endif()

	if(ZLIB_INCLUDE_DIRS AND ZLIB_LIBRARIES)
		set(HAVE_ZLIB TRUE)
		message(STATUS "Found Zlib: '${ZLIB_INCLUDE_DIRS}', '${ZLIB_LIBRARIES}'")
	else()
		set(HAVE_ZLIB FALSE)
		message(FATAL_ERROR "Zlib not found")
	endif()

	include_directories(${ZLIB_INCLUDE_DIRS})
	list(APPEND DEP_LIBS ${ZLIB_LIBRARIES})
endif(OPTION_USE_ZLIB)


####### Option: Build with iconv ##########

if(OPTION_USE_ICONV)
	if(OPTION_MINGW)
		find_path(ICONV_INCLUDE_DIRS iconv.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_library(ICONV_LIBRARIES libiconv.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
	elseif(OPTION_STATIC)
		find_path(ICONV_INCLUDE_DIRS iconv.h)
		find_library(ICONV_LIBRARIES libiconv.a)
	else()
		find_path(ICONV_INCLUDE_DIRS iconv.h)
		find_library(ICONV_LIBRARIES NAMES iconv)
	endif()

	if(ICONV_INCLUDE_DIRS AND ICONV_LIBRARIES)
		set(HAVE_ICONV TRUE)
		message(STATUS "Found Libiconv: '${ICONV_INCLUDE_DIRS}', '${ICONV_LIBRARIES}'")
	else()
		set(HAVE_ICONV FALSE)
		message(FATAL_ERROR "Libiconv not found")
	endif()

	include_directories(${ICONV_INCLUDE_DIRS})
	list(APPEND DEP_LIBS ${ICONV_LIBRARIES})
endif(OPTION_USE_ICONV)


####### Option: Build with Libmoleculizer ##########

if(OPTION_USE_LIBMZR)
	# Get Libxml2
	if(OPTION_MINGW)
		find_path(LIBXML2_INCLUDE_DIR parser.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_library(LIBXML2_LIBRARIES libxml2.a PATHS ${MINGWDIR}/lib NO_DEFAULT_PATH)
	elseif(OPTION_STATIC)
		find_path(LIBXML2_INCLUDE_DIR parser.h PATHS /usr/local/include/libxml2/libxml /opt/local/include/libxml2/libxml /usr/include/libxml2/libxml)
		find_library(LIBXML2_LIBRARIES libxml2.a PATHS /usr/local/lib /opt/local/lib /usr/lib/)
	else()
		include(FindLibXml2)
	endif()

	if(LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARIES)
		set(HAVE_LIBXML2 TRUE)
		message(STATUS "Found Libxml2: '${LIBXML2_INCLUDE_DIR}', '${LIBXML2_LIBRARIES}'")
	else()
		set(HAVE_LIBXML2 FALSE)
		message(FATAL_ERROR "Libxml2 not found")
	endif()

	# Get LibXML++ (use version 1.0.5)
	if(OPTION_MINGW)
		find_path(LIBXML++_INCLUDE_DIR libxml++.h PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
		find_library(LIBXML++_LIBRARIES libxml++-1.0.a PATHS ${MINGWDIR}/include NO_DEFAULT_PATH)
	elseif(OPTION_STATIC)
		find_path(LIBXML++_INCLUDE_DIR libxml++.h PATHS /usr/local/include/libxml++-1.0/libxml++)
		find_library(LIBXML++_LIBRARIES libxml++-1.0.a PATHS /usr/local/lib)
	else()
		find_path(LIBXML++_INCLUDE_DIR libxml++.h PATHS /usr/local/include/libxml++-1.0/libxml++)
		find_library(LIBXML++_LIBRARIES NAMES libxml++-1.0.dylib libxml++-1.0.so PATHS /usr/local/lib)
	endif()

	if(LIBXML++_INCLUDE_DIR AND LIBXML++_LIBRARIES)
		set(HAVE_LIBXML++ TRUE)
		message(STATUS "Found Libxml++: '${LIBXML++_INCLUDE_DIR}', '${LIBXML++_LIBRARIES}'")
	else()
		set(HAVE_LIBXML++ FALSE)
		message(FATAL_ERROR "LibXML++ not found.  Building without Libmoleculizer is possible.")
	endif()

	# Get Python
	include(FindPythonInterp)
	include(FindPythonLibs)
	if(NOT PYTHONLIBS_FOUND)
		message(FATAL_ERROR, "PythonLibs not found.")
	endif()
	set(HAVE_PYTHONLIBS TRUE)

	# Get Libmoleculizer (it needs to be built beforehand)
	set(LIBMZR_INCLUDE_DIR source/libmoleculizer-1.1.2/src)
	set(LIBMZR_PYTHON_DIR source/libmoleculizer-1.1.2/python-src/language_parser/moleculizer)
	find_library(LIBMZR_LIBRARIES libmoleculizer.a PATHS source/libmoleculizer-1.1.2 source/libmoleculizer-1.1.2/cmake NO_DEFAULT_PATH)
	message(STATUS "Libmoleculizer: '${LIBMZR_INCLUDE_DIR}', '${LIBMZR_LIBRARIES}'")

	# Add the whole works in
	include_directories(${LIBXML2_INCLUDE_DIR})
	include_directories(${LIBXML++_INCLUDE_DIR})
	include_directories(${PYTHON_INCLUDE_DIRS})
	include_directories(${LIBMZR_INCLUDE_DIR})
	list(APPEND DEP_LIBS ${LIBXML2_LIBRARIES})
	list(APPEND DEP_LIBS ${LIBXML++_LIBRARIES})
	list(APPEND DEP_LIBS ${LIBMZR_LIBRARIES})
	list(APPEND DEP_LIBS ${PYTHON_LIBRARIES})
	set(LIBMOLECULIZER TRUE)
endif(OPTION_USE_LIBMZR)


####### Targets ##########

if(OPTION_TARGET_SMOLDYN)
	add_executable(smoldyn ${SRC_FILES} ${MAIN_FILES} ${HEADER_FILES})
	target_link_libraries(smoldyn ${DEP_LIBS})
endif()

if(OPTION_TARGET_LIBSMOLDYN)
	add_library(smoldyn_shared SHARED ${SRC_FILES} ${HEADER_FILES})
	add_library(smoldyn_static STATIC ${SRC_FILES} ${HEADER_FILES})
	if(OPTION_USE_LIBMZR OR OPTION_MINGW)
		target_link_libraries(smoldyn_shared ${DEP_LIBS})
		target_link_libraries(smoldyn_static ${DEP_LIBS})
	endif(OPTION_USE_LIBMZR OR OPTION_MINGW)
	if(APPLE)
		SET_TARGET_PROPERTIES(smoldyn_shared PROPERTIES LINK_FLAGS "-ltiff -framework Glut -framework OpenGL")
	endif(APPLE)
endif(OPTION_TARGET_LIBSMOLDYN)


########## install ###########

if(NOT OPTION_MINGW)

	if(OPTION_TARGET_SMOLDYN)
		install(TARGETS smoldyn RUNTIME DESTINATION bin)
	endif()

	if(OPTION_TARGET_LIBSMOLDYN)
		install(TARGETS smoldyn_shared LIBRARY DESTINATION lib)
		install(TARGETS smoldyn_static ARCHIVE DESTINATION lib)
		install(FILES source/Smoldyn/libsmoldyn.h source/Smoldyn/smoldyn.h DESTINATION include)
	endif(OPTION_TARGET_LIBSMOLDYN)

	if(OPTION_USE_LIBMZR)
		install(CODE "execute_process(COMMAND python setup.py install -f --prefix=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ../source/libmoleculizer-1.1.2/python-src/language_parser)")
	endif(OPTION_USE_LIBMZR)

endif(NOT OPTION_MINGW)

########## configure file #########

configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/source/smoldynconfigure.h.in
	${CMAKE_CURRENT_BINARY_DIR}/smoldynconfigure.h
)

########## package #########

include(CPack)



