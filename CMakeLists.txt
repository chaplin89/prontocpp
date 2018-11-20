cmake_minimum_required (VERSION 3.8)

project("ProntoCpp")

# Declare main executable
add_executable (${PROJECT_NAME} "")

# Include Conan dependencies
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    include(.dep/Release/conanbuildinfo.cmake)
else()
	include(.dep/Debug/conanbuildinfo.cmake)
endif()
conan_basic_setup(TARGETS)

# Configure config file
set(${PROJECT_NAME}_RuntimeDep "config/${PROJECT_NAME}.cfg")

# Configure sources
set (${PROJECT_NAME}_Headers
	"includes/configuration.hpp"
	"includes/logging.hpp")

set(${PROJECT_NAME}_Sources
	"sources/config.cpp"
	"main.cpp")

# Configure protobuf
set(${PROJECT_NAME}_Protobuf 
	"protobuf/${PROJECT_NAME}Config.proto")

find_package(protobuf)
if (NOT ${protobuf_FOUND})
	message(ERROR "Protobuf was not found")
endif()

protobuf_generate_cpp(${PROJECT_NAME}_PROTO_SRCS ${PROJECT_NAME}_PROTO_HDRS ${${PROJECT_NAME}_Protobuf})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::boost)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::cppzmq)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::grpc)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::rxcpp)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::catch2)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::protobuf)

# Build project structure
source_group("Source" FILES ${${PROJECT_NAME}_Sources})
source_group("Headers" FILES ${${PROJECT_NAME}_Headers})
source_group("Config" FILES ${${PROJECT_NAME}_RuntimeDep})
source_group("Protobuf" FILES ${${PROJECT_NAME}_Protobuf})

# Copy files in binary directory
foreach(RuntimeDep ${${PROJECT_NAME}_RuntimeDep})
	get_filename_component(File ${RuntimeDep} NAME)
	configure_file(${RuntimeDep} ${CMAKE_CURRENT_BINARY_DIR}/${File} COPYONLY)
endforeach()

# Begin Workaround
#	Don't break the compilation if boost::asio and boost::log are in the same translation unit.
#	Reference: https://svn.boost.org/trac10/ticket/12974
target_compile_definitions(${PROJECT_NAME} PRIVATE -D_WIN32_WINNT=0x0600)
# End Workaround

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 2)
set(${PROJECT_NAME}_VERSION_PATCH 1)
configure_file(includes/version.hpp.in ${PROJECT_BINARY_DIR}/version.hpp)

# Executable definition
target_sources(${PROJECT_NAME} PRIVATE
	${${PROJECT_NAME}_Sources}
	${${PROJECT_NAME}_Headers}
	${${PROJECT_NAME}_RuntimeDep}
	${${PROJECT_NAME}_Protobuf}
	${${PROJECT_NAME}_PROTO_SRCS}
	${${PROJECT_NAME}_PROTO_HDRS})

target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Configure CTest/Catch2
enable_testing()
add_executable(catch-test-main test/main.cpp)
conan_target_link_libraries(catch-test-main)
add_test(NAME catch_test COMMAND $<TARGET_FILE:catch-test-main> --success)

enable_testing()
add_executable(bench-test-main benchmark/main.cpp)
conan_target_link_libraries(bench-test-main)
add_test(NAME bench-test-main COMMAND $<TARGET_FILE:bench-test-main> --success)