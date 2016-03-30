/*
 * ============================================================================
 *
 * PingThread.cpp
 * Thread which sends ping packets to the other end and measures the latency
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

#include "PingThread.h"
#include "PluginProcessor.h"

PingThread::PingThread(HqstreamerAudioProcessor& p) : Thread("PingThread"), parent(p){
    pingtime = -1.0f;
}
PingThread::~PingThread(){
    
}
    
void PingThread::run(){
    MemoryBlock data;
    //Don't send first packet quite immediately
    wait(10 + Random::getSystemRandom().nextInt(100));
    while(!threadShouldExit()){
        //Send ping packet
        data.setSize(16);
        uint32* u32ptr = (uint32*)data.getData();
        int64* s64ptr = (int64*)data.getData();
        u32ptr[0] = 16;
        u32ptr[1] = PACKET_TYPE_PING;
        starttime = Time::getCurrentTime();
        s64ptr[1] = starttime.toMilliseconds();
        if(!parent.sendPacket(data)){
            //Send failed
            pingtime = -1.0f;
        }else{
            //Wait for response
            bool gotResponse = wait(2000); //assuming ping won't take more than 2 seconds
            if(threadShouldExit()) return;
            if(gotResponse){
                //Calculate result
                int64 delta = endtime.toMilliseconds() - starttime.toMilliseconds();
                if(pingtime < 0){
                    pingtime = delta;
                }else{
                    pingtime = (0.1f * (float)delta) + (0.9f * pingtime);
                }
            }
        }
        parent.showPingTime(pingtime);
        //Wait for a second
        wait(985 + Random::getSystemRandom().nextInt(30));
    }
}
    
void PingThread::gotPingResponse(const MemoryBlock& message){
    int64* s64ptr = (int64*)message.getData();
    starttime = Time(s64ptr[1]);
    endtime = Time::getCurrentTime();
    notify();
}
