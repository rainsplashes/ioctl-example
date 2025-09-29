Demonstrates how data can be transfered between the kernel space and user space (secretively) on Windows.

## Files
### Driver > CopyPhysical.h
The file that manages data protection when sending memory back to the userspace program.
### Driver > IODispatch.h
Handles input from IOCTL and then calls functions such as read/write memory.
### Driver > Interface.h
Details the struct format that will be used by the userspace client as well as command codes.
### Userspace > DriverCommunication.h
Opens driver handle and sends IOCTL commands to the kernel driver as needed.

## Design Notes
An ad hoc "encryption" algorithm is used to obscure data that is sent in order to evade detections by certain anti-viruses/anti-cheats. This algorithm can easily be replaced with one that is more complex.

The IOCTL control code field is also not used, but is instead randomized, in order to further obfuscate app behavior.
