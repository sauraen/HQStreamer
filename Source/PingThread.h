/*
 * ============================================================================
 *
 * PingThread.h
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

#ifndef PINGTHREAD_H_INCLUDED
#define PINGTHREAD_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"


class PingThread : public Thread {
public:
    PingThread(HqstreamerAudioProcessor& p);
    ~PingThread();
    
    virtual void run() override;
    
    void gotPingResponse(const MemoryBlock& message);
private:
    HqstreamerAudioProcessor& parent;
    float pingtime;
    Time starttime, endtime;
};



#endif  // PINGTHREAD_H_INCLUDED
