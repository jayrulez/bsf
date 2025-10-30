set(BS_NETWORK_INC_NETWORK
	"bsfNetwork/Network/BsNetwork.h"
)

set(BS_NETWORK_SRC_NETWORK
	"bsfNetwork/Network/BsNetwork.cpp"
)

set(BS_NETWORK_INC_NOFILTER
	"bsfNetwork/BsNetworkPrerequisites.h"
)

source_group("Network" FILES ${BS_NETWORK_INC_NETWORK} ${BS_NETWORK_SRC_NETWORK})
source_group("" FILES ${BS_NETWORK_INC_NOFILTER})

set(BS_NETWORK_SRC
	${BS_NETWORK_INC_NETWORK}
	${BS_NETWORK_SRC_NETWORK}
	${BS_NETWORK_INC_NOFILTER}
)
