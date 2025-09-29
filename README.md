Demonstrates how data can be transfered between the driver space and user space (secretively) on Windows.

## Design Notes
An ad hoc "encryption" algorithm is used to obscure data that is sent in order to evade detections by certain anti-viruses/anti-cheats.
This algorithm can easily be replaced with one that is more complex.
The IOCTL control code field is also not used, but is instead randomized, in order to further obfuscate app behavior.
