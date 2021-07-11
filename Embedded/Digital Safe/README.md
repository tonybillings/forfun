This is a “digital safe,” which is constructed using a clear acrylic cube and contains a BeagleBone Black microcomputer, Adruino Mega microcontroller, a 4x4 LED-lit button pad, and a 2.8” screen. The safe is connected to the network via Ethernet and is accessed over HTTP, where a web-based user interface is provided to manage encrypted files stored on the BeagleBone. The web server was written in Python and used Tornado to handle socket binding, intaking HTTP requests, etc, and encryption was implemented using libtomcrypt (C++). The chosen algorithm is AES-256, which uses a 32-byte key and up to 16 of which is provided by user input on the button pad mounted on the front of the safe; the other 16 bytes is computed based on the MAC address of the BeagleBone.  

![](img/20190107_135739.jpg)  

![](img/20190107_135807.jpg)  

