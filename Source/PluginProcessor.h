/*
 * ============================================================================
 *
 * PluginProcessor.h
 * Main plugin code, container for all data, audio interface
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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED


/*

Packet Format
All multibyte values are little-endian

0 uint32 length; //Total length of the packet, must equal message.getSize()
4 uint32 type; //see below for types

//If type == PACKET_TYPE_PING
8 int64 starttime;
16

//If type == PACKET_TYPE_PINGRESPONSE
8 int64 starttime_echo;
16

//If type == PACKET_TYPE_INIT
8 uint32 role; //Client's role, 0 sender, 1 receiver
12 uint32 samplerate; //Sample rate in Hz
16

//If type == PACKET_TYPE_AUDIO_*
8 uint32 nchannels;
12 uint32 nsamples;
16-* channel 0 data;
*-* channel 1 data;
etc.

//If type == PACKET_TYPE_AUDIO_ZEROS
format as above, but no data--just fill all samples with 0.0f

//DPCM format:
First two bytes is int16 initial sample (little-endian)
For each byte after that:
    If bit 7 is 0, bits 6:0 represent an int7 delta value.
    If bit 7 is 1, bits 6:0 concatenated with bits 7:0 of the next byte represent
        an int15 delta value (little-endian)


*/

#define PACKET_TYPE_PING 0
#define PACKET_TYPE_PINGRESPONSE 1
#define PACKET_TYPE_INIT 2
#define PACKET_TYPE_AUDIO_ZEROS 10
#define PACKET_TYPE_AUDIO_FLOAT32 11
#define PACKET_TYPE_AUDIO_INT16 12
#define PACKET_TYPE_AUDIO_DPCM 13



#include "JuceHeader.h"

class HQSConnection;
class HQSServer;
class PingThread;
class SenderThread;
class ReceiveBuffer;

//==============================================================================
/**
*/
class HqstreamerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    HqstreamerAudioProcessor();
    ~HqstreamerAudioProcessor();

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    const String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    
    bool sendPacket(const MemoryBlock& packet);
    void packetReceived(const MemoryBlock& packet);
    
    void connectionMade();
    void connectionLost();
    
    void showPingTime(float pingtime);
    void bufferUnderflow();
    void bufferOverflow();
    
    void changeBufferLen();
    
    //==============================================================================
    
    inline bool isServer() {return isserver;}
    inline bool isClient() {return isclient;}
    inline bool isSender() {return issender;}
    inline bool isReceiver() {return isreceiver;}
    inline bool isLinkUp() {return linkup;}
    inline bool isSenderReady() {return senderready;}
    inline int getAudioType() {return audioType;}
    inline float getLatency() {return latency;}
    inline int getPort() {return portcopy;}
    
    void serverSelected();
    void clientSelected();
    void senderSelected();
    void receiverSelected();
    void connectClicked(const String& ip, int port);
    void latencySelected(String lat);
    void qualitySelected(String quality);
    
    //==============================================================================
    
    ScopedPointer<HQSConnection> conn;
    ScopedPointer<HQSServer> server;
    ScopedPointer<PingThread> pingthread;
    ScopedPointer<SenderThread> senderthread;
    OwnedArray<ReceiveBuffer, CriticalSection> buffers;
    
    String statusMsg, audioMsg, infoMsg;
    bool updategui;
    
private:
    bool isserver;
    bool isclient;
    bool issender;
    bool isreceiver;
    bool linkup;
    bool senderready;
    int audioType;
    int samplerate;
    int samplesperblock;
    float latency;
    int portcopy;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HqstreamerAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
