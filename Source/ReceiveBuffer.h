/*
 * ============================================================================
 *
 * ReceiveBuffer.h
 * Circular buffer that can be written/read simultaneously by TCP and audio thread
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

#ifndef RECEIVEBUFFER_H_INCLUDED
#define RECEIVEBUFFER_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"

class ReceiveBuffer {
public:
    ReceiveBuffer(HqstreamerAudioProcessor& p);
    ~ReceiveBuffer();
    
    inline uint32 getLength() { return length; }
    void setLength(uint32 samples);
    void resetRun(bool sender);
    
    void getUseLock();
    void releaseUseLock();
    
    float read();
    void write(float s);
    float getSample(uint32 index);
    
    inline uint32 getReadHead() { return readhead; }
    inline uint32 getWriteHead() { return writehead; }
    
private:
    HqstreamerAudioProcessor& parent;
    MemoryBlock data;
    uint32 length, readhead, writehead;
    bool stop, pauseuntilread, pauseuntilwrite;
    ReadWriteLock runlock;
};



#endif  // RECEIVEBUFFER_H_INCLUDED
