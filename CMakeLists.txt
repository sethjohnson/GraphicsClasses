cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (VRClasses)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_SOURCE_DIR}/build/install" CACHE PATH "default install path" FORCE )
endif()

set (SOURCEFILES 
  source/main.cpp
  source/Shader.cpp
  source/ShaderProgram.cpp
  source/RawData.cpp
  source/GeometryArray.cpp
  source/AttributeArray.cpp
  source/WindowManager.cpp
  source/WindowManager_GLFW.cpp
)

set (HEADERFILES
  include/Shader.h
  include/ShaderProgram.h
  include/RawData.h
  include/GeometryArray.h
  include/AttributeArray.h
  include/WindowManager.h
  include/WindowManager_GLFW.h
)

source_group("Header Files" FILES ${HEADERFILES})

#------------------------------------------
# Download dependencies
#------------------------------------------

set(glfw_checkout_Dir ${CMAKE_SOURCE_DIR}/lib/glfw)
make_directory(${glfw_checkout_Dir})
include(ExternalProject)

set(glfw_PREFIX "${glfw_checkout_Dir}")
set(glfw_INSTALL_DIR "${glfw_checkout_Dir}/install")

set(glfw_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL_DIR} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DGLFW_BUILD_DOCS=OFF)
set(glfw_DIR "${glfw_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++")
endif()

#   if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#		set(GIT_EXECUTABLE "C:/Program Files (x86)/Git/bin/git.exe")
#   endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   
ExternalProject_add(glfw
		PREFIX ${glfw_PREFIX}
		GIT_REPOSITORY https://github.com/ivlab/glfw.git
		GIT_TAG gpu-affinity
		INSTALL_DIR ${glfw_INSTALL_DIR}
		CMAKE_ARGS ${glfw_CMAKE_ARGS}
		UPDATE_COMMAND ""
)
 
set_property(TARGET glfw PROPERTY FOLDER "Dependencies")

set(GLFW_INCLUDE_DIR ${glfw_INSTALL_DIR}/include CACHE INTERNAL "Directory of GLFW header files")
if(MSVC)
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/glfw3.lib CACHE INTERNAL "GLFW lib file")
else()
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/libglfw3.a CACHE INTERNAL "GLFW lib file")
endif()	
include_directories(${GLFW_INCLUDE_DIR} )
include_directories("/usr/local/include")

# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARIES ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
set(OPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})

# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew)
include_directories(lib/glew/include)


#------------------------------------------
# Download dependencies 2
#------------------------------------------
set(assimp_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/assimp)
make_directory(${assimp_checkout_Dir})

set(soil_dl_Dir ${CMAKE_SOURCE_DIR}/dependencies/SOIL)
make_directory(${soil_dl_Dir})

include(ExternalProject)

set(assimp_PREFIX "${assimp_checkout_Dir}")
set(assimp_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/assimp")

set(soil_PREFIX "${soil_dl_Dir}")
set(soil_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/SOIL")

set(assimp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${assimp_INSTALL_DIR} -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_SAMPLES=OFF -DASSIMP_ENABLE_BOOST_WORKAROUND=ON -DASSIMP_BUILD_TESTS=OFF )
set(assimp_DIR "${assimp_INSTALL_DIR}")
set(soil_DIR "${soil_INSTALL_DIR}")

    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(assimp_CMAKE_ARGS ${assimp_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD=c++11 -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY=libc++)
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GIT_EXECUTABLE "C:/Program Files (x86)/Git/bin/git.exe")
    endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

ExternalProject_add(assimp
PREFIX ${assimp_PREFIX}
GIT_REPOSITORY https://github.com/ivlab/assimp.git
GIT_TAG master
INSTALL_DIR ${assimp_INSTALL_DIR}
CMAKE_ARGS ${assimp_CMAKE_ARGS}
UPDATE_COMMAND ""
)

set_property(TARGET assimp PROPERTY FOLDER "Dependencies")

set(soil_PATCH ${CMAKE_COMMAND} -E copy ${soil_PREFIX}/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt)
set(soil_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${soil_INSTALL_DIR}")

ExternalProject_add(SOIL
PREFIX ${soil_PREFIX}
URL http://www.lonesock.net/files/soil.zip
INSTALL_DIR ${soil_INSTALL_DIR}
CMAKE_ARGS ${soil_CMAKE_ARGS}
PATCH_COMMAND ${soil_PATCH}
UPDATE_COMMAND ""
)

set_property(TARGET SOIL PROPERTY FOLDER "Dependencies")


#find_package(ZLIB)
#INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

set(ASSIMP_INCLUDE_DIR ${assimp_INSTALL_DIR}/include CACHE INTERNAL "Directory of assimp header files")
set(SOIL_INCLUDE_DIR ${soil_INSTALL_DIR}/include CACHE INTERNAL "Directory of SOIL header files")
if(MSVC)
set(ASSIMP_LIBRARY debug ${assimp_INSTALL_DIR}/lib/assimpd.lib optimized ${assimp_INSTALL_DIR}/lib/assimp.lib CACHE INTERNAL "assimp lib file")
set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/SOIL.lib CACHE INTERNAL "SOIL lib file")
else()
set(ASSIMP_LIBRARY ${assimp_INSTALL_DIR}/lib/libassimp.a CACHE INTERNAL "assimp lib file")
set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/libSOIL.a CACHE INTERNAL "SOIL lib file")
endif()
find_package(ZLIB)
if( NOT ZLIB_FOUND )
ExternalProject_Get_Property(assimp binary_dir)
include_directories(${binary_dir}/contrib/zlib)
#set(ZLIB_LIBRARIES optimized ${binary_dir}/contrib/zlib/Release/zlibstatic.lib debug ${binary_dir}/contrib/zlib/Debug/zlibstaticd.lib)
set(ZLIB_LIBRARIES ${binary_dir}/contrib/zlib/Debug/zlibstaticd.lib)
message(sdf ${binary_dir})
endif(NOT ZLIB_FOUND)
set(LIBASSIMP_LIBRARIES ${ASSIMP_LIBRARY} ${ZLIB_LIBRARIES})

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${SOIL_INCLUDE_DIR})

#------------------------------------------
# Include Directories
#------------------------------------------


# Include Directories
include_directories (
  .
  include
)

# Windows Section #
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	set(LIBS_ALL ${LIBS_ALL} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
	message(STATUS "${CORE_VIDEO_FRAMEWORK}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
endif()

make_directory(${CMAKE_BINARY_DIR}/lib)
make_directory(${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach (CONF ${CMAKE_CONFIGURATION_TYPES})
	string (TOUPPER ${CONF} CONF)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/bin)
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
endforeach(CONF CMAKE_CONFIGURATION_TYPES)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")

#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	set(CMAKE_BUILD_POSTFIX "")
endif()

# Build Target
add_executable ( ${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES} )

target_link_libraries(${PROJECT_NAME} ${LIBASSIMP_LIBRARIES} ${SOIL_LIBRARY} ${GLFW_LIBRARY} glew ${OPENGL_LIBRARIES} ${LIBS_ALL})

add_dependencies( ${PROJECT_NAME} glfw glew assimp SOIL)
#install(DIRECTORY ${PROJECT_NAME}/include/ DESTINATION include)

install( TARGETS ${PROJECT_NAME}
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         RUNTIME DESTINATION bin)