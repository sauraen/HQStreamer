/*
 * ============================================================================
 *
 * SenderThread.h
 * Thread which takes samples out of the ReceiveBuffer and sends them over TCP
 * 
 * From HQStreamer - Stream lossless audio from one DAW to another across the web
 * Copyright (C) 2016 Sauraen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ============================================================================
*/

#ifndef SENDERTHREAD_H_INCLUDED
#define SENDERTHREAD_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"


class SenderThread : public Thread {
public:
    SenderThread(HqstreamerAudioProcessor& p, int sampleGap, int samplesPerPacket);
    ~SenderThread();
    
    virtual void run() override;
    
    void setSampleValues(int sampleGap, int samplesPerPacket);
    
    static int16 encodeFloat(float f);
    
private:
    HqstreamerAudioProcessor& parent;
    int samplegap;
    int samplesperpacket;
};


#endif  // SENDERTHREAD_H_INCLUDED
