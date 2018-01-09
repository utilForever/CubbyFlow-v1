# Target name
set(target DirectX11_LevelSet)

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")

link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")

ADD_DEFINITIONS(-DUNICODE)

# Sources
file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.fx)

# Build executable
add_executable(${target} WIN32
    ${sources})

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

# Compile options
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(-D_USE_MATH_DEFINES) # for M_PI
endif ()

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS} 

    INTERFACE
)

# Link libraries
target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
    CubbyFlow 
	d3d11
	d3dcompiler
	d3dx11d
	dxerr
	dxguid
	winmm
	comctl32
	)