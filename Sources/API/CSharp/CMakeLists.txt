# Target name
set(target CubbyFlowSharp)

# Define
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Sources
file(GLOB header_dir
		${root_dir}/Includes)

file(GLOB_RECURSE headers
		${header_dir}/*.h)

file(GLOB_RECURSE sources
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Build library
add_library(${target} SHARED
	${sources})

# Project options
set_target_properties(${target} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6")
set_target_properties(${target} PROPERTIES VS_DOTNET_REFERENCES "System")
set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/clr /EHa")

string(REPLACE "/EHsc" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

string(REGEX REPLACE "/RTC(su|[1su])" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/RTC(su|[1su])" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

# Link libraries
target_link_libraries(${target}
		PRIVATE
		${DEFAULT_LINKER_OPTIONS}
		CubbyFlow)