#qt-audio-networking

The program is a simple VoIP application that is cross-platform and allows users to “conference call” across a local network. Transmission is near instant (dependant on network performance), and there is no noticeable lag or latency when communicating. It allows participants to select whether they wish to broadcast or listen (or both). Audio is compressed using zlib, and is sent at CD quality (44.1khz, 16 channels), and does not utilise any audio codecs.

#NOTABLE FEATURES
+ Automated peer discovery.
+ Near real-time communication.
+ High-fidelity audio.
+ Ability to select whether to broadcast or listen.

The Network Clients table displays a list of current clients on the network, and displays their status i.e. if they are broadcasting or listening. If the user wishes to broadcast to other users on the network they click the “broadcast” button, which initiates audio recording and transmits this across the network, similarly, to stop they click the “end broadcast” button.
If the user wishes to listen to other users on the network, they click on the “listen” button, which writes the read network packets to the audio device. Users can stop this by clicking on the “stop listen”. The “Input Device” combo box can be used to change the input audio device (if there is more than one on the system). This must be initiated before the user begins broadcasting (and can be changed
once they stop broadcasting). Finally, the volume sliders can be used to control the audio output and input volumes.

A simplified high-level description of how the program works is as follows:
+ A UDP server and client is started on the server.
+ UDP server frequently sends broadcast messages, which also state whether the server is broadcasting
or listening.
+ UDP client receives these messages, adds client to the client list if if doesn't exist, or updates it status if it
does.
+ When broadcasting, UDP server reads audio from input device, and sends this to all clients in the client
list.
+ When listening, UDP client receives audio, and writes this to the output device.
