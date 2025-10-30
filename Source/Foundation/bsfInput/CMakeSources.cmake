set(BS_INPUT_INC_INPUT
	"bsfInput/Input/BsInputFwd.h"
	"bsfInput/Input/BsKeyboard.h"
	"bsfInput/Input/BsMouse.h"
	"bsfInput/Input/BsGamepad.h"
	"bsfInput/Input/BsInput.h"
	"bsfInput/Input/BsInputConfiguration.h"
	"bsfInput/Input/BsVirtualInput.h"
)

set(BS_INPUT_SRC_INPUT
	"bsfInput/Input/BsInput.cpp"
	"bsfInput/Input/BsInputConfiguration.cpp"
	"bsfInput/Input/BsVirtualInput.cpp"
)

set(BS_INPUT_INC_WIN32
	"bsfInput/Private/Win32/BsWin32Input.h"
)

set(BS_INPUT_SRC_WIN32
	"bsfInput/Private/Win32/BsWin32Input.cpp"
	"bsfInput/Private/Win32/BsWin32Gamepad.cpp"
	"bsfInput/Private/Win32/BsWin32Keyboard.cpp"
	"bsfInput/Private/Win32/BsWin32Mouse.cpp"
)

set(BS_INPUT_INC_LINUX
	"bsfInput/Private/Linux/BsLinuxInput.h"
)

set(BS_INPUT_SRC_LINUX
	"bsfInput/Private/Linux/BsLinuxInput.cpp"
	"bsfInput/Private/Linux/BsLinuxGamepad.cpp"
	"bsfInput/Private/Linux/BsLinuxKeyboard.cpp"
	"bsfInput/Private/Linux/BsLinuxMouse.cpp"
)

set(BS_INPUT_INC_MACOS
	"bsfInput/Private/MacOS/BsMacOSInput.h"
)

set(BS_INPUT_SRC_MACOS
	"bsfInput/Private/MacOS/BsMacOSInput.cpp"
	"bsfInput/Private/MacOS/BsMacOSGamepad.cpp"
	"bsfInput/Private/MacOS/BsMacOSKeyboard.cpp"
	"bsfInput/Private/MacOS/BsMacOSMouse.cpp"
)

set(BS_INPUT_INC_NOFILTER
	"bsfInput/BsInputPrerequisites.h"
)

source_group("Input" FILES ${BS_INPUT_INC_INPUT} ${BS_INPUT_SRC_INPUT})
source_group("Win32" FILES ${BS_INPUT_INC_WIN32} ${BS_INPUT_SRC_WIN32})
source_group("Linux" FILES ${BS_INPUT_INC_LINUX} ${BS_INPUT_SRC_LINUX})
source_group("MacOS" FILES ${BS_INPUT_INC_MACOS} ${BS_INPUT_SRC_MACOS})
source_group("" FILES ${BS_INPUT_INC_NOFILTER})

set(BS_INPUT_SRC
	${BS_INPUT_INC_INPUT}
	${BS_INPUT_SRC_INPUT}
	${BS_INPUT_INC_NOFILTER}
)

if(WIN32)
	list(APPEND BS_INPUT_SRC ${BS_INPUT_INC_WIN32})
	list(APPEND BS_INPUT_SRC ${BS_INPUT_SRC_WIN32})
elseif(LINUX)
	list(APPEND BS_INPUT_SRC ${BS_INPUT_INC_LINUX})
	list(APPEND BS_INPUT_SRC ${BS_INPUT_SRC_LINUX})
elseif(APPLE)
	list(APPEND BS_INPUT_SRC ${BS_INPUT_INC_MACOS})
	list(APPEND BS_INPUT_SRC ${BS_INPUT_SRC_MACOS})
endif()
