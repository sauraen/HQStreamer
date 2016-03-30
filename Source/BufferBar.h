/*
 * ============================================================================
 *
 * BufferBar.h
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

#ifndef BUFFERBAR_H_INCLUDED
#define BUFFERBAR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class BufferBar : public Component, public Timer{
public:
    BufferBar(HqstreamerAudioProcessor& p, int buffernumber);
    ~BufferBar();

    void paint (Graphics&);
    void resized();
    
    virtual void timerCallback() override;

private:
    HqstreamerAudioProcessor& parent;
    int buffernum;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferBar)
};


#endif  // BUFFERBAR_H_INCLUDED
