/*
 * ============================================================================
 *
 * BufferBar.cpp
 * GUI component that displays an animated representation of the audio in a
 * ReceiveBuffer
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

#include "BufferBar.h"
#include "ReceiveBuffer.h"

//==============================================================================
BufferBar::BufferBar(HqstreamerAudioProcessor& p, int buffernumber) : parent(p) {
    buffernum = buffernumber;
    startTimerHz(30);
}

BufferBar::~BufferBar(){
    
}

void BufferBar::paint (Graphics& g){
    int width = getWidth(), height = getHeight();
    if(width <= 0 || height <= 0) return;
    
    g.fillAll(Colours::black);
    
    ReceiveBuffer* b = parent.buffers[buffernum];
    b->getUseLock();
    uint32 length = b->getLength();
    if(length <= 0) return;
    uint32 readhead = b->getReadHead();
    uint32 writehead = b->getWriteHead();
    
    uint32 fill = (writehead >= readhead) ? (writehead - readhead) : (writehead + length - readhead);
    float proportionFilled = (float)fill / (float)length;
    float fillend = (float)width * proportionFilled;
    float okayness;
    if(parent.isReceiver()){
        okayness = 1.0f - (2.0f * abs(proportionFilled - 0.5f));
    }else{
        okayness = 1.0f - proportionFilled;
    }
    Colour okaynessColor = Colour((uint8)255 - (uint8)(okayness * 255.0f), (uint8)(okayness * 255.0f), (uint8)0);
    
    //Draw background
    g.setColour(okaynessColor.darker());
    g.fillRect(0, 0, (int)fillend, height);

    //Draw audio
    g.setColour(okaynessColor.brighter(0.7f));
    uint32 s = readhead;
    float subpixel = 0.0f;
    float delta = (float)width / (float)length; //Pixels per sample
    float lastsubpixel = 0.0f;
    float sample, maxsample = 0.0f;
    for(; subpixel < fillend; subpixel += delta){
        sample = abs(b->getSample(s));
        s = (s+1) % length;
        if(sample >= 1.0f) sample = 1.0f;
        if(sample > maxsample) maxsample = sample;
        if(floor(lastsubpixel) != floor(subpixel)){
            //Done with a pixel
            g.fillRect((int)floor(lastsubpixel), (int)(floor(height * (1.0f - maxsample))), 1, (int)(ceil(height * maxsample)));
            maxsample = 0.0f;
        }
        lastsubpixel = subpixel;
    }
    b->releaseUseLock();
    
    //Draw playback head
    g.setColour(okaynessColor);
    g.fillRect((int)floor(fillend), 0, 2, height);
    
}

void BufferBar::resized(){
    
}
void BufferBar::timerCallback(){
    repaint();
}
