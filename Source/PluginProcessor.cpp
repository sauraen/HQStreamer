/*
 * ============================================================================
 *
 * PluginProcessor.cpp
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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HQSServer.h"
#include "HQSConnection.h"
#include "PingThread.h"
#include "ReceiveBuffer.h"
#include "SenderThread.h"


//==============================================================================
HqstreamerAudioProcessor::HqstreamerAudioProcessor(){
    jassert(sizeof(float) == 4);
    server = new HQSServer(*this);
    linkup = senderready = false;
    isserver = isclient = false;
    issender = isreceiver = false;
    updategui = true;
    audioType = PACKET_TYPE_AUDIO_DPCM;
    latency = 1.0f;
    samplerate = 48000;
    samplesperblock = 256;
    portcopy = 51478;
    statusMsg = "<Status>";
    infoMsg = "<Connection info>";
    audioMsg = "<Audio info>";
    buffers.add(new ReceiveBuffer(*this));
    buffers.add(new ReceiveBuffer(*this));
    changeBufferLen();
}

HqstreamerAudioProcessor::~HqstreamerAudioProcessor(){
    
}

//==============================================================================
const String HqstreamerAudioProcessor::getName() const{
    return JucePlugin_Name;
}

int HqstreamerAudioProcessor::getNumParameters(){
    return 0;
}

float HqstreamerAudioProcessor::getParameter (int index){
    index = 0; //suppress warning
    return 0.0f;
}

void HqstreamerAudioProcessor::setParameter (int index, float newValue){
    index = 0; //suppress warning
    newValue = 0.0f; //suppress warning
}

const String HqstreamerAudioProcessor::getParameterName (int index){
    return "Parameter " + String(index) + " Name";
}

const String HqstreamerAudioProcessor::getParameterText (int index){
    return "Parameter " + String(index) + " Text";
}

const String HqstreamerAudioProcessor::getInputChannelName (int channelIndex) const{
    return String (channelIndex + 1);
}

const String HqstreamerAudioProcessor::getOutputChannelName (int channelIndex) const{
    return String (channelIndex + 1);
}

bool HqstreamerAudioProcessor::isInputChannelStereoPair (int index) const{
    return (index == 0 || index == 1);
}

bool HqstreamerAudioProcessor::isOutputChannelStereoPair (int index) const{
    return (index == 0 || index == 1);
}

bool HqstreamerAudioProcessor::acceptsMidi() const{
    return false;
}

bool HqstreamerAudioProcessor::producesMidi() const{
    return false;
}

bool HqstreamerAudioProcessor::silenceInProducesSilenceOut() const{
    return false;
}

double HqstreamerAudioProcessor::getTailLengthSeconds() const{
    return 0.0;
}

int HqstreamerAudioProcessor::getNumPrograms(){
    return 1;
}

int HqstreamerAudioProcessor::getCurrentProgram(){
    return 0;
}

void HqstreamerAudioProcessor::setCurrentProgram (int index){
    index = 0; //suppress warning
}

const String HqstreamerAudioProcessor::getProgramName (int index){
    return "Nonexistent Program " + String(index);
}

void HqstreamerAudioProcessor::changeProgramName (int index, const String& newName){
    index = newName.length(); //suppress warning
}

//==============================================================================
void HqstreamerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock){
    if(samplerate != sampleRate){
        samplerate = sampleRate;
        changeBufferLen();
    }
    if(samplesperblock != samplesPerBlock){
        samplesperblock = samplesPerBlock;
        if(senderthread != nullptr){
            senderthread->setSampleValues(samplesperblock << 1, samplesperblock + (samplesperblock >> 1));
        }
    }
}

void HqstreamerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

    
void HqstreamerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // Zero output buffers not corresponding to any input buffers (should not be necessary here)
    //for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    int32 nsamples = buffer.getNumSamples();
    int32 nchannels = buffer.getNumChannels();
    int c, s;
    if(linkup){
        if(issender){
            //Sender
            for(c=0; c<nchannels; ++c){
                buffers[c]->getUseLock();
            }
            for(s=0; s<nsamples; ++s){
                for(c=0; c<nchannels; ++c){
                     buffers[c]->write(buffer.getWritePointer(c)[s]);
                }
            }
            for(c=0; c<nchannels; ++c){
                buffers[c]->releaseUseLock();
            }
        }else if(isreceiver){
            for(c=0; c<nchannels; ++c){
                buffers[c]->getUseLock();
            }
            for(s=0; s<nsamples; ++s){
                for(c=0; c<nchannels; ++c){
                    buffer.getWritePointer(c)[s] = buffers[c]->read();
                }
            }
            for(c=0; c<nchannels; ++c){
                buffers[c]->releaseUseLock();
            }
        }//else neither sender nor receiver, do nothing
    }
}

//==============================================================================
bool HqstreamerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HqstreamerAudioProcessor::createEditor()
{
    return new HqstreamerAudioProcessorEditor (*this);
}

//==============================================================================
void HqstreamerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HqstreamerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HqstreamerAudioProcessor();
}
//==============================================================================

void HqstreamerAudioProcessor::packetReceived(const MemoryBlock& packet){
    statusMsg = "Packet received, size " + String((int)packet.getSize());
    if(packet.getSize() < 8){
        statusMsg = "Packet too small! " + String((int)packet.getSize());
        return;
    }
    int c, s;
    uint32* u32ptr = (uint32*)packet.getData();
    uint32 size = u32ptr[0];
    if(size != (uint32)packet.getSize()){
        statusMsg = "Bad size field " + String((int)size);
        return;
    }
    uint32 type = u32ptr[1];
    if(type == PACKET_TYPE_PING){
        //Send a copy back
        MemoryBlock ret;
        ret.setSize(16);
        uint32* u32ptr2 = (uint32*)ret.getData();
        u32ptr2[0] = 16;
        u32ptr2[1] = PACKET_TYPE_PINGRESPONSE;
        u32ptr2[2] = u32ptr[2];
        u32ptr2[3] = u32ptr[3];
        if(!sendPacket(ret)){
            //statusMsg = "Could not send PINGRESPONSE";
        }else{
            statusMsg = "Sent PINGRESPONSE";
        }
    }else if(type == PACKET_TYPE_PINGRESPONSE){
        //Send it to the ping thread
        //statusMsg = "Got PINGRESPONSE";
        pingthread->gotPingResponse(packet);
    }else if(type == PACKET_TYPE_INIT){
        statusMsg = "Got INIT";
        uint32 role = u32ptr[2];
        uint32 sr = u32ptr[3];
        if(isserver){
            if((isreceiver ^ (role == 1)) || sr != samplerate){
                connectionLost();
                NativeMessageBox::showMessageBoxAsync(AlertWindow::WarningIcon, "HQStreamer", "Client rejected connection (role " + String(role) + ")!\n"
                    + "Client wants sample rate of " + String(sr) + " but this DAW is at " + String(samplerate) + "--one of you will have to change your project sample rate.");
                return;
            }
            updategui = true;
        }else if(isclient){
            isreceiver = (role == 1);
            issender = !isreceiver;
            //Send a response
            MemoryBlock ret;
            ret.setSize(16);
            uint32* u32ptr2 = (uint32*)ret.getData();
            u32ptr2[0] = 16;
            u32ptr2[1] = PACKET_TYPE_INIT;
            u32ptr2[2] = isreceiver ? 0 : 1;
            u32ptr2[3] = samplerate;
            if(!sendPacket(ret)){
                statusMsg = "Could not send INIT";
            }else{
                statusMsg = "Sent INIT";
            }
            if(sr != samplerate){
                connectionLost();
                NativeMessageBox::showMessageBoxAsync(AlertWindow::WarningIcon, "HQStreamer", "Rejecting connection (role " + String(role) + ")!\n"
                    + "Server wants sample rate of " + String(sr) + " but this DAW is at " + String(samplerate) + "--one of you will have to change your project sample rate.");
                return;
            }
            updategui = true;
        }else{
            return;
        }
        for(c=0; c<getNumInputChannels(); c++){
            buffers[c]->resetRun(issender);
        }
        if(issender){
            senderthread = new SenderThread(*this, samplesperblock << 1, samplesperblock + (samplesperblock >> 1));
            senderthread->startThread();
        }
    }else if(type == PACKET_TYPE_AUDIO_ZEROS){
        if(!isreceiver) return;
        uint32 nchannels = u32ptr[2], nsamples = u32ptr[3];
        audioMsg = "zeros audio, " + String(size) + "b, " + String(nchannels) + "ch, " + String(nsamples) + "spl";
        if(nchannels != getNumInputChannels()) return;
        for(c=0; c<nchannels; ++c){
            buffers[c]->getUseLock();
        }
        for(c=0; c<nchannels; ++c){
            for(s=0; s<nsamples; ++s){
                buffers[c]->write(0.0f);
            }
        }
        for(c=0; c<nchannels; ++c){
            buffers[c]->releaseUseLock();
        }
    }else if(type == PACKET_TYPE_AUDIO_FLOAT32){
        if(!isreceiver) return;
        uint32 nchannels = u32ptr[2], nsamples = u32ptr[3];
        audioMsg = "float32 audio, " + String(size) + "b, " + String(nchannels) + "ch, " + String(nsamples) + "spl";
        if(nchannels != getNumInputChannels()) return;
        float* fptr = (float*)packet.getData();
        uint32 i = 4; //index of fptr
        for(c=0; c<nchannels; ++c){
            buffers[c]->getUseLock();
        }
        for(c=0; c<nchannels; ++c){
            for(s=0; s<nsamples; ++s){
                buffers[c]->write(fptr[i++]);
            }
        }
        for(c=0; c<nchannels; ++c){
            buffers[c]->releaseUseLock();
        }
    }else if(type == PACKET_TYPE_AUDIO_INT16){
        if(!isreceiver) return;
        uint32 nchannels = u32ptr[2], nsamples = u32ptr[3];
        audioMsg = "int16 audio, " + String(size) + "b, " + String(nchannels) + "ch, " + String(nsamples) + "spl";
        if(nchannels != getNumInputChannels()) return;
        int16* iptr = (int16*)packet.getData();
        uint32 i = 8; //index of iptr
        float temp;
        for(c=0; c<nchannels; ++c){
            buffers[c]->getUseLock();
        }
        for(c=0; c<nchannels; ++c){
            for(s=0; s<nsamples; ++s){
                temp = (float)iptr[i++];
                temp /= 32768.0f;
                buffers[c]->write(temp);
            }
        }
        for(c=0; c<nchannels; ++c){
            buffers[c]->releaseUseLock();
        }
    }else if(type == PACKET_TYPE_AUDIO_DPCM){
        if(!isreceiver) return;
        uint32 nchannels = u32ptr[2], nsamples = u32ptr[3];
        audioMsg = "DPCM audio, " + String(size) + "b, " + String(nchannels) + "ch, " + String(nsamples) + "spl";
        if(nchannels != getNumInputChannels()) return;
        uint8* bptr = (uint8*)packet.getData();
        uint32 i = 16; //index of bptr
        uint8 b; int16 value, delta; uint16 tempu16; float tempf;
        for(c=0; c<nchannels; ++c){
            buffers[c]->getUseLock();
        }
        for(c=0; c<nchannels; ++c){
            //Get initial value
            value = (int16)((uint16)bptr[i] | ((uint16)bptr[i+1] << 8));
            i += 2;
            tempf = (float)value;
            tempf /= 32768.0f;
            buffers[c]->write(tempf);
            //Get DPCM samples
            for(s=1; s<nsamples; ++s){
                b = bptr[i++];
                if(b & 0x80){
                    //Two-byte
                    tempu16 = (uint16)(b & 0x7F) | ((uint16)(bptr[i++]) << 7);
                    tempu16 |= (tempu16 & 0x4000) << 1; //Sign bit
                    delta = (int16)tempu16;
                }else{
                    //One-byte
                    b |= (b & 0x40) << 1; //Sign bit
                    delta = (int16)((int8)b); //Cast to s8 then s16
                }
                value += delta;
                tempf = (float)value;
                tempf /= 32768.0f;
                buffers[c]->write(tempf);
            }
        }
        for(c=0; c<nchannels; ++c){
            buffers[c]->releaseUseLock();
        }
    }else{
        statusMsg = "Bad type field " + String((int)type);
        return;
    }
}
bool HqstreamerAudioProcessor::sendPacket(const MemoryBlock& packet){
    if(!linkup) return false;
    if(conn == nullptr) return false;
    return conn->sendMessage(packet);
}

void HqstreamerAudioProcessor::connectionMade(){
    statusMsg = "Connection made!";
    linkup = true;
    pingthread = new PingThread(*this);
    pingthread->startThread();
    if(isserver){
        //Send init packet
        MemoryBlock ret;
        ret.setSize(16);
        uint32* u32ptr2 = (uint32*)ret.getData();
        u32ptr2[0] = 16;
        u32ptr2[1] = PACKET_TYPE_INIT;
        u32ptr2[2] = isreceiver ? 0 : 1;
        u32ptr2[3] = samplerate;
        if(!sendPacket(ret)){
            statusMsg = "Could not send INIT";
        }else{
            statusMsg = "Sent INIT";
        }
    }
}
void HqstreamerAudioProcessor::connectionLost(){
    statusMsg = "Connection lost!";
    linkup = false;
    senderready = false;
    if(isclient){
        issender = isreceiver = false;
    }
    if(pingthread != nullptr){
        pingthread->stopThread(10);
        pingthread = nullptr;
    }
    if(senderthread != nullptr){
        senderthread->stopThread(10);
        senderthread = nullptr;
    }
    server->stop();
    if(conn != nullptr){
        conn->disconnect();
        conn = nullptr;
    }
    updategui = true;
}

void HqstreamerAudioProcessor::showPingTime(float pingtime){
    infoMsg = "Avg RT ping " + String(pingtime) + " ms";
}
void HqstreamerAudioProcessor::latencySelected(String lat){
    if(lat == "250ms"){
        latency = 0.25f;
    }else if(lat == "500ms"){
        latency = 0.5f;
    }else if(lat == "1s"){
        latency = 1.0f;
    }else if(lat == "2s"){
        latency = 2.0f;
    }else if(lat == "4s"){
        latency = 4.0f;
    }
    changeBufferLen();
}
void HqstreamerAudioProcessor::qualitySelected(String quality){
    if(quality == "float32"){
        audioType = PACKET_TYPE_AUDIO_FLOAT32;
    }else if(quality == "int16"){
        audioType = PACKET_TYPE_AUDIO_INT16;
    }else if(quality == "DPCM"){
        audioType = PACKET_TYPE_AUDIO_DPCM;
    }
}
void HqstreamerAudioProcessor::changeBufferLen(){
    for(int c=0; c<2; c++){
        buffers[c]->setLength((int)(latency * (float)samplerate * 2.0f));
        if(linkup){
            buffers[c]->resetRun(issender);
        }
    }
}

void HqstreamerAudioProcessor::bufferUnderflow(){
    audioMsg = "Buffer underflow!";
}
void HqstreamerAudioProcessor::bufferOverflow(){
    audioMsg = "Buffer overflow!";
}
void HqstreamerAudioProcessor::serverSelected(){
    if(linkup || senderready) return;
    if(!isserver){
        isclient = false;
        isserver = true;
        issender = false;
        isreceiver = true;
        updategui = true;
    }
}
void HqstreamerAudioProcessor::clientSelected(){
    if(linkup || senderready) return;
    if(!isclient){
        isserver = false;
        isclient = true;
        issender = false;
        isreceiver = false;
        updategui = true;
    }
}
void HqstreamerAudioProcessor::senderSelected(){
    if(linkup || senderready || !isserver) return;
    isreceiver = false;
    issender = true;
    updategui = true;
}
void HqstreamerAudioProcessor::receiverSelected(){
    if(linkup || senderready || !isserver) return;
    issender = false;
    isreceiver = true;
    updategui = true;
}
void HqstreamerAudioProcessor::connectClicked(const String& ip, int port){
    if(!isserver && !isclient) return;
    if(linkup || senderready) return;
    if(isserver){
        portcopy = port;
        statusMsg = "Hacking your firewall";
        server->beginWaitingForSocket(port);
        statusMsg = "Ready, waiting for client...";
        senderready = true;
        updategui = true;
    }else if(isclient){
        portcopy = port;
        conn = new HQSConnection(*this);
        if(conn->connectToSocket(ip, port, 1000)){
            statusMsg = "Connection established";
        }else{
            conn = nullptr;
            statusMsg = "Connection failed";
        }
        updategui = true;
    }
}

