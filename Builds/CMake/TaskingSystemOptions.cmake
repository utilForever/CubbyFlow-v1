#
# Setup tasking system build configuration
#

# Determine the best default option for tasking system backend
if(NOT DEFINED CUBBYFLOW_TASKING_SYSTEM)
	find_package(TBB QUIET)
	if(TBB_FOUND)
		set(TASKING_DEFAULT TBB)
	else()
		find_package(OpenMP QUIET)
		if(OpenMP_FOUND)
			set(TASKING_DEFAULT OpenMP)
		else()
			set(TASKING_DEFAULT CPP11Thread)
		endif()
	endif()
else()
	set(TASKING_DEFAULT ${CUBBYFLOW_TASKING_SYSTEM})
endif()

set(CUBBYFLOW_TASKING_SYSTEM ${TASKING_DEFAULT} CACHE STRING
	"Per-node thread tasking system [CPP11Thread, TBB, OpenMP, Serial]")

set_property(CACHE CUBBYFLOW_TASKING_SYSTEM PROPERTY
			 STRINGS CPP11Thread TBB OpenMP Serial)

# Note - Make the CUBBYFLOW_TASKING_SYSTEM build option case-insensitive
string(TOUPPER ${CUBBYFLOW_TASKING_SYSTEM} CUBBYFLOW_TASKING_SYSTEM_ID)

set(CUBBYFLOW_TASKING_TBB			FALSE)
set(CUBBYFLOW_TASKING_OPENMP		FALSE)
set(CUBBYFLOW_TASKING_CPP11THREAD	FALSE)
set(CUBBYFLOW_TASKING_SERIAL		FALSE)