/*
 * ============================================================================
 *
 * SenderThread.cpp
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

#include "SenderThread.h"
#include "PluginProcessor.h"
#include "ReceiveBuffer.h"

SenderThread::SenderThread(HqstreamerAudioProcessor& p, int sampleGap, int samplesPerPacket) : Thread("SenderThread"), parent(p){
    samplegap = sampleGap;
    samplesperpacket = samplesPerPacket;
}
SenderThread::~SenderThread(){
    
}
void SenderThread::setSampleValues(int sampleGap, int samplesPerPacket){
    samplegap = sampleGap;
    samplesperpacket = samplesPerPacket;
}
    
void SenderThread::run(){
    const int nchannels = 2; //TODO
    int c, s;
    while(!threadShouldExit()){
        //See if there's enough data in the buffer to send
        for(c=0; c<nchannels; ++c){
            parent.buffers[c]->getUseLock();
        }
        ReceiveBuffer* rbuf = parent.buffers[1]; //Check the second buffer because it will be filled slightly after the first
        uint32 wh = rbuf->getWriteHead();
        uint32 rh = rbuf->getReadHead();
        uint32 len = rbuf->getLength();
        uint32 fill = (wh >= rh) ? (wh - rh) : (wh + len - rh);
        if(fill >= samplegap + samplesperpacket){
            int nsamples = samplesperpacket;
            //Copy buffer from critical one to local copy, for multiple use
            OwnedArray<MemoryBlock> localbuffer;
            localbuffer.add(new MemoryBlock(4*nsamples));
            localbuffer.add(new MemoryBlock(4*nsamples));
            float* channelData;
            for(c=0; c<nchannels; ++c){
                channelData = (float*)localbuffer[c]->getData();
                rbuf = parent.buffers[c];
                for(s=0; s<nsamples; ++s){
                    channelData[s] = rbuf->read();
                }
            }
            for(c=0; c<nchannels; ++c){
                parent.buffers[c]->releaseUseLock();
            }
            //Create packet and write header
            int32 packetlen = 16;
            int audioType = parent.getAudioType();
            if(audioType == PACKET_TYPE_AUDIO_FLOAT32){
                packetlen += (4 * nsamples * nchannels);
            }else if(audioType == PACKET_TYPE_AUDIO_INT16){
                packetlen += (2 * nsamples * nchannels);
            }else if(audioType == PACKET_TYPE_AUDIO_DPCM){
                packetlen += (2 * nsamples * nchannels); //but it will shrink
            }else{
                jassertfalse;
                return;
            }
            MemoryBlock packet(packetlen);
            int32* s32p = (int32*)packet.getData();
            s32p[0] = packetlen;
            s32p[1] = audioType;
            s32p[2] = nchannels;
            s32p[3] = nsamples;
            //Check for all zeros
            bool flag = true;
            const float epsilon = 1.0f / 32768.0f;
            for(c=0; c<nchannels; ++c){
                channelData = (float*)localbuffer[c]->getData();
                for(s=0; s<nsamples; ++s){
                    if(channelData[s] >= epsilon || channelData[s] <= (0.0f-epsilon)){
                        flag = false;
                        break;
                    }
                }
                if(!flag) break;
            }
            if(flag){
                //Use PACKET_TYPE_AUDIO_ZEROS
                s32p[0] = 16;
                s32p[1] = PACKET_TYPE_AUDIO_ZEROS;
                packet.setSize(16);
            }else{
                //Write data
                if(audioType == PACKET_TYPE_AUDIO_FLOAT32){
                    float* fp = (float*)packet.getData();
                    uint32 i = 4; //index of fp
                    for(c=0; c<nchannels; ++c){
                        channelData = (float*)localbuffer[c]->getData();
                        for(s=0; s<nsamples; ++s){
                            fp[i++] = channelData[s];
                        }
                    }
                }else if(audioType == PACKET_TYPE_AUDIO_INT16){
                    int16* s16p = (int16*)packet.getData();
                    uint32 i = 8; //index of s16p
                    for(c=0; c<nchannels; ++c){
                        channelData = (float*)localbuffer[c]->getData();
                        for(s=0; s<nsamples; ++s){
                            s16p[i++] = encodeFloat(channelData[s]);
                        }
                    }
                }else if(audioType == PACKET_TYPE_AUDIO_DPCM){
                    uint8* u8p = (uint8*)packet.getData();
                    uint32 i = 16; //index of u8p
                    int16 value, lastsentvalue, delta;
                    for(c=0; c<nchannels; ++c){
                        channelData = (float*)localbuffer[c]->getData();
                        //Write initial value
                        value = encodeFloat(channelData[0]);
                        u8p[i++] = value & 0x00FF;
                        u8p[i++] = (value & 0xFF00) >> 8;
                        lastsentvalue = value;
                        //Write DPCM samples
                        for(s=1; s<nsamples; ++s){
                            value = encodeFloat(channelData[s]);
                            delta = value - lastsentvalue;
                            if(delta >= 0x4000) delta = 0x3FFF;
                            if(delta < -0x4000) delta = -0x4000;
                            if(delta < 0x0040 && delta >= -0x0040){
                                //One-byte
                                u8p[i++] = delta & 0x7F;
                            }else{
                                //Two-byte
                                u8p[i++] = 0x80 | (delta & 0x7F);
                                u8p[i++] = ((delta >> 7) & 0xFF);
                            }
                            lastsentvalue += delta; //in case it wasn't enough
                        }
                    }
                    //Change length
                    s32p[0] = i;
                    packet.setSize(i);
                }else{
                    jassertfalse;
                    return;
                }
            }
            //Send
            parent.sendPacket(packet);
        }else{
            for(c=0; c<nchannels; ++c){
                parent.buffers[c]->releaseUseLock();
            }
        }
        //Done
        if(threadShouldExit()) return;
        wait(5); //Wait for 5ms and try again
    }
}

int16 SenderThread::encodeFloat(float f){
    f *= 32768.0f;
    if(f > 32767.0f) f = 32767.0f;
    if(f < -32768.0f) f = -32768.0f;
    return (int16)f;
}
