/*
 * ============================================================================
 *
 * ReceiveBuffer.cpp
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

#include "ReceiveBuffer.h"

ReceiveBuffer::ReceiveBuffer(HqstreamerAudioProcessor& p) : parent(p){
    length = readhead = writehead = 0;
    stop = true;
    pauseuntilread = false;
    pauseuntilwrite = false;
}
ReceiveBuffer::~ReceiveBuffer(){
    
}

void ReceiveBuffer::setLength(uint32 samples){
    const ScopedWriteLock lock(runlock); //Make sure neither read or write are active
    length = samples;
    data.setSize(4*length);
    data.fillWith(0);
    readhead = 0;
    writehead = length / 2;
    stop = true;
    pauseuntilread = false;
    pauseuntilwrite = false;
}

void ReceiveBuffer::resetRun(bool sender){
    const ScopedWriteLock lock(runlock); //Make sure neither read or write are active
    data.fillWith(0);
    readhead = 0;
    if(sender){
        writehead = 0;
    }else{
        writehead = length / 2;
    }
    pauseuntilwrite = true;
    pauseuntilread = false;
    stop = false;
}

void ReceiveBuffer::getUseLock(){
    runlock.enterRead();
}
void ReceiveBuffer::releaseUseLock(){
    runlock.exitRead();
}

float ReceiveBuffer::read(){
    if(stop || pauseuntilwrite) return 0.0f;
    pauseuntilread = false;
    float ret = ((float*)data.getData())[readhead];
    readhead = (readhead+1) % length;
    if(readhead == writehead){
        pauseuntilwrite = true;
        parent.bufferUnderflow();
        return 0.0f;
    }
    return ret;
}
void ReceiveBuffer::write(float s){
    if(stop || pauseuntilread) return;
    pauseuntilwrite = false;
    ((float*)data.getData())[writehead] = s;
    writehead = (writehead+1) % length;
    if(writehead == readhead){
        pauseuntilread = true;
        parent.bufferOverflow();
    }
}

float ReceiveBuffer::getSample(uint32 index){
    if(index >= length) return 0.0f;
    return ((float*)data.getData())[index];
}
