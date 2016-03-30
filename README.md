# HQStreamer
Stream uncompressed audio from one DAW to another across the web

Copyright (C) 2016 Sauraen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License (LICENSE.txt) for more details.

# Compatibility
### Currently available:
 * VST (VST2), x64 version, built on Windows 7, compatible with Windows 8 but not Windows 10
 * VST (VST2), x86 version, built on Windows 7, compatible with Windows 8 but not Windows 10

### May be available upon request and/or sooner or later:
 * VST3, x86 and x64
 * AU, x86 and x64, built on Windows 7
 * AU, x86 and x64, built on Mac OSX El Capitan
 * RTAS
 * AAX

# Installation (VST / VST3 version)
Drop the plugin in your plugins folder of your favorite DAW, and tell your DAW
to rescan the plugins (if it doesn't automatically). If it still does not appear
in your plugin list after rescanning, try installing the latest Visual C++
Runtimes from the Microsoft website: https://www.microsoft.com/en-us/download/details.aspx?id=40784

# Setup
If you want to stream to a computer not on the local network (i.e. behind a
router other than your own), you will have to set up port forwarding on the
router on whichever side you run HQStreamer as a server. (Note that audio can
be transmitted in either direction regardless of which side is the server.)
This is a standard procedure, similar to that for many games and other web-based
applications: you log into the router and add a port forwarding rule to send
all traffic incoming on a particular port to a particular computer (the one
on which the plugin is running). You may choose any unused port; of course, both
users of HQStreamer must enter this port number rather than the default. More
information on this process may be found through your favorite search engine.

# Use
The interface is pretty straightforward: one user is the server, the other is
the client. The server decides whether it is sending or receiving audio. The
user on the server side must click Connect before the client side does--the
server waits for a client to connect. If the stream audio is interrupted, try
clicking the Reset Stream Buffer button (and, if you're the client, asking the
server to click this button as well). If this does not solve the problem, 
disconnect and reconnect (again with the server first).

The receiver has a choice of several latencies, which is simply half the buffer
length. A greater latency will give better stability if the connection is poor,
but obviously delay the audio more time. There's no danger to choosing too small
a latency, other than risking corrupted sound! The sender also has a choice of
buffer lengths, but since the sender's buffer is kept close to empty, this
does not matter very much.

The sender has a choice of several audio types (bit depths):
 * float32 is 32-bit floating point numbers, which is the internal resolution
   of the audio interface with the DAW, and therefore true lossless. However,
   this takes four bytes per sample.
 * int16 is 16-bit integer audio (same as used in standard .WAV files or CD
   audio). This is technically not lossless, because the bit depth is lower,
   however the quality is standard and high enough to be inaudible for most
   purposes.
 * DPCM is 8- or 16-bit encoded delta PCM audio. This means the value sent is
   the difference between the previous sample and this one, and then this value
   is sent as 1 byte if it can fit and 2 bytes otherwise. As long as one sample
   is not more than half the full range away from the last--which will be true
   for almost all audio--this produces results numerically identical to int16
   audio, with roughly a 10%-40% savings on bandwidth (depending on the sound).
   (Pretty much the only time this will be wrong is if your audio is loud square
   waves--so if you're a chiptune artist, you might want to stick to int16!)

# Bugs
Send bug reports to the author's name at Google's email.
