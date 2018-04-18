#
# Find TBB
#

set(TBB_VERSION_REQUIRED "2.0")

if(NOT TBB_ROOT)
	set(TBB_ROOT $ENV{TBB_ROOT})
endif()
if(NOT TBB_ROOT)
	set(TBB_ROOT $ENV{TBBROOT})
endif()