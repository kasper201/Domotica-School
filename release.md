# What's changed
This is the change log for release v2.0.2

## Embedded client
- Added the ability to read existing groups and nodes
- Added the ability to read existing groups and nodes to UI via USART
- Added ability to set nodes into groups via the UI
- Added ability to 'create' groups
- Added ability remove 'created' groups
- Added getElem function
- Updated getAddr function
- Updated uart communication
- Added subscribe to group function
- Added unsubscribe from group function
- Added check for duplicate groups
- Added check for unknown commands
- Updated gen_onoff_send to use correct address


## User interface
- Is currently in a final product state
- Nodes can be added by hand
- Works with addresses and other identification numbers as well
- Fixed unsubscribe address order issue
- Fixed baudrate (from 9600 (Arduino) to 115200 (Node))

## Known bugs
- {placeholder}

## Other
- lucas's part doesn't work yet
- The application no longer functions as a bridge

