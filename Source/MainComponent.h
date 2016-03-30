/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_9002020A4DD09B20__
#define __JUCE_HEADER_9002020A4DD09B20__

//[Headers]     -- You can add your own extra header files here --
/*
 * ============================================================================
 *
 * MainComponent.h
 * Main GUI component for HQStreamer plugin
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

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "BufferBar.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public Component,
                       public Timer,
                       public ButtonListener,
                       public ComboBoxListener
{
public:
    //==============================================================================
    MainComponent (HqstreamerAudioProcessor& p);
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void fillAllControls();
    void gotDisconnect();
    void gotInitAsServer(const String& ip);
    void gotInitAsClient(bool isreceiver);

    virtual void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    HqstreamerAudioProcessor& parent;

    ScopedPointer<BufferBar> barBufferL, barBufferR;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> optSender;
    ScopedPointer<ToggleButton> optReceiver;
    ScopedPointer<Label> label;
    ScopedPointer<TextEditor> txtIP;
    ScopedPointer<Label> label2;
    ScopedPointer<TextEditor> txtPort;
    ScopedPointer<TextButton> btnConnect;
    ScopedPointer<TextButton> btnDisconnect;
    ScopedPointer<Label> lblStatus;
    ScopedPointer<Label> lblInfo;
    ScopedPointer<Label> lblAudio;
    ScopedPointer<ToggleButton> optServer;
    ScopedPointer<ToggleButton> optClient;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> label4;
    ScopedPointer<TextButton> btnReset;
    ScopedPointer<ComboBox> cbxLatency;
    ScopedPointer<ComboBox> cbxQuality;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9002020A4DD09B20__
