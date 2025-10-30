set(BS_PLATFORM_INC_PLATFORM
	"bsfPlatform/Platform/BsPlatform.h"
	"bsfPlatform/Platform/BsPlatformWindow.h"
	"bsfPlatform/Platform/BsFolderMonitor.h"
	"bsfPlatform/Platform/BsDropTarget.h"
	"bsfPlatform/Platform/BsCursor.h"
)

set(BS_PLATFORM_SRC_PLATFORM
	"bsfPlatform/Platform/BsDropTarget.cpp"
	"bsfPlatform/Platform/BsCursor.cpp"
)

set(BS_PLATFORM_INC_UTILITY
	"bsfPlatform/Utility/BsPlatformEnums.h"
	"bsfPlatform/Utility/BsIconUtility.h"
)

set(BS_PLATFORM_SRC_UTILITY
	"bsfPlatform/Utility/BsIconUtility.cpp"
)

set(BS_PLATFORM_INC_PLATFORM_WIN32
	"bsfPlatform/Private/Win32/BsWin32DropTarget.h"
	"bsfPlatform/Private/Win32/BsWin32Platform.h"
	"bsfPlatform/Private/Win32/BsWin32Window.h"
)

set(BS_PLATFORM_SRC_PLATFORM_WIN32
	"bsfPlatform/Private/Win32/BsWin32FolderMonitor.cpp"
	"bsfPlatform/Private/Win32/BsWin32Platform.cpp"
	"bsfPlatform/Private/Win32/BsWin32DropTarget.cpp"
	"bsfPlatform/Private/Win32/BsWin32Window.cpp"
)

set(BS_PLATFORM_INC_PLATFORM_LINUX
	"bsfPlatform/Private/Linux/BsLinuxPlatform.h"
	"bsfPlatform/Private/Linux/BsLinuxWindow.h"
	"bsfPlatform/Private/Linux/BsLinuxDropTarget.h"
)

set(BS_PLATFORM_SRC_PLATFORM_LINUX
	"bsfPlatform/Private/Linux/BsLinuxPlatform.cpp"
	"bsfPlatform/Private/Linux/BsLinuxWindow.cpp"
	"bsfPlatform/Private/Linux/BsLinuxDropTarget.cpp"
	"bsfPlatform/Private/Linux/BsLinuxFolderMonitor.cpp"
)

set(BS_PLATFORM_INC_PLATFORM_MACOS
	"bsfPlatform/Private/MacOS/BsMacOSPlatform.h"
	"bsfPlatform/Private/MacOS/BsMacOSWindow.h"
	"bsfPlatform/Private/MacOS/BsMacOSDropTarget.h"
)

set(BS_PLATFORM_SRC_PLATFORM_MACOS
	"bsfPlatform/Private/MacOS/BsMacOSFolderMonitor.cpp"
	"bsfPlatform/Private/MacOS/BsMacOSDropTarget.cpp"
	"bsfPlatform/Private/MacOS/BsMacOSWindow.mm"
	"bsfPlatform/Private/MacOS/BsMacOSPlatform.mm"
)

set(BS_PLATFORM_INC_NOFILTER
	"bsfPlatform/BsPlatformPrerequisites.h"
)

if(WIN32)
	list(APPEND BS_PLATFORM_INC_PLATFORM ${BS_PLATFORM_INC_PLATFORM_WIN32})
	list(APPEND BS_PLATFORM_SRC_PLATFORM ${BS_PLATFORM_SRC_PLATFORM_WIN32})
elseif(LINUX)
	list(APPEND BS_PLATFORM_INC_PLATFORM ${BS_PLATFORM_INC_PLATFORM_LINUX})
	list(APPEND BS_PLATFORM_SRC_PLATFORM ${BS_PLATFORM_SRC_PLATFORM_LINUX})
elseif(APPLE)
	list(APPEND BS_PLATFORM_INC_PLATFORM ${BS_PLATFORM_INC_PLATFORM_MACOS})
	list(APPEND BS_PLATFORM_SRC_PLATFORM ${BS_PLATFORM_SRC_PLATFORM_MACOS})
endif()

foreach(source ${BS_PLATFORM_SRC_PLATFORM_LINUX} ${BS_PLATFORM_SRC_PLATFORM_WIN32})
	set_source_files_properties ("${source}" PROPERTIES
			COTIRE_EXCLUDED "True")
endforeach()

if(APPLE)
	source_group("MacOS" FILES ${BS_PLATFORM_INC_PLATFORM_MACOS} ${BS_PLATFORM_SRC_PLATFORM_MACOS})
endif()

source_group("Platform" FILES ${BS_PLATFORM_INC_PLATFORM} ${BS_PLATFORM_SRC_PLATFORM})
source_group("Utility" FILES ${BS_PLATFORM_INC_UTILITY} ${BS_PLATFORM_SRC_UTILITY})
source_group("" FILES ${BS_PLATFORM_INC_NOFILTER})

set(BS_PLATFORM_SRC
	${BS_PLATFORM_INC_PLATFORM}
	${BS_PLATFORM_SRC_PLATFORM}
	${BS_PLATFORM_INC_UTILITY}
	${BS_PLATFORM_SRC_UTILITY}
	${BS_PLATFORM_INC_NOFILTER}
)
