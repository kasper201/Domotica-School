#ifndef EASYSTRING_H
#define EASYSTRING_H

// Out
#define CONNECTED_OUT "mesh init"
#define MESH_CREATE "mesh cdb create"
#define MESH_PROV_CREATE "mesh prov local 0 0x0001"
#define MESH_RESET "mesh reset-local"
#define MESH_APP_KEY_CREATE "mesh models cfg appkey add 0 0"
#define MESH_APP_KEY_BIND "mesh models cfg model app-bind "     // <NodeAddress> <app key address> <ModelID>
#define MESH_ADDRESS_PROV "0x0001"
#define MESH_APP_KEY " 0x000"
#define MESH_NETWORK_KEY " 0"
#define MESH_SERVER " 0x1000"
#define MESH_CLIENT " 0x1001"
#define MESH_SUBSCRIBE "mesh models cfg model sub-add"          // <NodeAddress> <GroupAddress> <ModelID>
#define MESH_UNSUBSCRIBE "mesh models cfg model sub-del"        // <NodeAddress> <GroupAddress> <ModelID>
#define MESH_BEACON_ON "mesh prov beacon-listen on"
#define MESH_ADD_NODE "mesh prov remote-adv "                  // <uuid> <network key index> <NodeAddress> <duration>
#define MESH_TARGET "mesh target dst"                          // <NodeAddress>

// Easy use
#define HEX_PREFIX " 0x"
#define PROVISIONER_NODE_NAME "Provisioner"
#define NODE_SHOW "Node: "
#define STRING_TO_SHOW_LED "LED of "
#define STRING_TO_SHOW_BUTTON "Button of "

// Easy to change values
#define NODE_ADDRESS_INCREMENTS 1
#define ADD_NODE_DURATION 10

// In
#define CONNECTED_IN "Connected"

#endif // EASYSTRING_H

