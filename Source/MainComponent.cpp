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

//[Headers] You can add your own extra header files here...
/*
 * ============================================================================
 *
 * MainComponent.cpp
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

#include "HQSConnection.h"
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (HqstreamerAudioProcessor& p)
    : parent(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (optSender = new ToggleButton ("new toggle button"));
    optSender->setButtonText (TRANS("Sender"));
    optSender->addListener (this);

    addAndMakeVisible (optReceiver = new ToggleButton ("new toggle button"));
    optReceiver->setButtonText (TRANS("Receiver"));
    optReceiver->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Remote IP:")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (txtIP = new TextEditor ("new text editor"));
    txtIP->setMultiLine (false);
    txtIP->setReturnKeyStartsNewLine (false);
    txtIP->setReadOnly (false);
    txtIP->setScrollbarsShown (true);
    txtIP->setCaretVisible (true);
    txtIP->setPopupMenuEnabled (true);
    txtIP->setText (TRANS("127.0.0.1"));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Port:")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (txtPort = new TextEditor ("new text editor"));
    txtPort->setMultiLine (false);
    txtPort->setReturnKeyStartsNewLine (false);
    txtPort->setReadOnly (false);
    txtPort->setScrollbarsShown (true);
    txtPort->setCaretVisible (true);
    txtPort->setPopupMenuEnabled (true);
    txtPort->setText (TRANS("51478"));

    addAndMakeVisible (btnConnect = new TextButton ("new button"));
    btnConnect->setButtonText (TRANS("Connect"));
    btnConnect->setConnectedEdges (Button::ConnectedOnRight);
    btnConnect->addListener (this);

    addAndMakeVisible (btnDisconnect = new TextButton ("new button"));
    btnDisconnect->setButtonText (TRANS("Disconnect"));
    btnDisconnect->setConnectedEdges (Button::ConnectedOnLeft);
    btnDisconnect->addListener (this);

    addAndMakeVisible (lblStatus = new Label ("new label",
                                              TRANS("<Status>")));
    lblStatus->setFont (Font (15.00f, Font::plain));
    lblStatus->setJustificationType (Justification::centredLeft);
    lblStatus->setEditable (false, false, false);
    lblStatus->setColour (TextEditor::textColourId, Colours::black);
    lblStatus->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lblInfo = new Label ("new label",
                                            TRANS("<Connection info>")));
    lblInfo->setFont (Font (15.00f, Font::plain));
    lblInfo->setJustificationType (Justification::centredLeft);
    lblInfo->setEditable (false, false, false);
    lblInfo->setColour (TextEditor::textColourId, Colours::black);
    lblInfo->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lblAudio = new Label ("new label",
                                             TRANS("<Audio info>")));
    lblAudio->setFont (Font (15.00f, Font::plain));
    lblAudio->setJustificationType (Justification::centredLeft);
    lblAudio->setEditable (false, false, false);
    lblAudio->setColour (TextEditor::textColourId, Colours::black);
    lblAudio->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (optServer = new ToggleButton ("Server"));
    optServer->addListener (this);

    addAndMakeVisible (optClient = new ToggleButton ("Client"));
    optClient->addListener (this);

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Latency:")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Quality:")));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (btnReset = new TextButton ("new button"));
    btnReset->setTooltip (TRANS("Click here when something goes wrong"));
    btnReset->setButtonText (TRANS("Reset Stream Buffer"));
    btnReset->addListener (this);

    addAndMakeVisible (cbxLatency = new ComboBox ("new combo box"));
    cbxLatency->setEditableText (false);
    cbxLatency->setJustificationType (Justification::centredLeft);
    cbxLatency->setTextWhenNothingSelected (String::empty);
    cbxLatency->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbxLatency->addItem (TRANS("250ms"), 1);
    cbxLatency->addItem (TRANS("500ms"), 2);
    cbxLatency->addItem (TRANS("1s"), 3);
    cbxLatency->addItem (TRANS("2s"), 4);
    cbxLatency->addItem (TRANS("4s"), 5);
    cbxLatency->addListener (this);

    addAndMakeVisible (cbxQuality = new ComboBox ("new combo box"));
    cbxQuality->setEditableText (false);
    cbxQuality->setJustificationType (Justification::centredLeft);
    cbxQuality->setTextWhenNothingSelected (String::empty);
    cbxQuality->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbxQuality->addItem (TRANS("float32"), 1);
    cbxQuality->addItem (TRANS("int16"), 2);
    cbxQuality->addItem (TRANS("DPCM"), 3);
    cbxQuality->addListener (this);


    //[UserPreSize]
    addAndMakeVisible(barBufferL = new BufferBar(parent, 0));
    addAndMakeVisible(barBufferR = new BufferBar(parent, 1));

    //[/UserPreSize]

    setSize (232, 368);


    //[Constructor] You can add your own custom stuff here..
    startTimerHz(15);
    parent.updategui = true;
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    optSender = nullptr;
    optReceiver = nullptr;
    label = nullptr;
    txtIP = nullptr;
    label2 = nullptr;
    txtPort = nullptr;
    btnConnect = nullptr;
    btnDisconnect = nullptr;
    lblStatus = nullptr;
    lblInfo = nullptr;
    lblAudio = nullptr;
    optServer = nullptr;
    optClient = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    btnReset = nullptr;
    cbxLatency = nullptr;
    cbxQuality = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    optSender->setBounds (8, 32, 80, 24);
    optReceiver->setBounds (96, 32, 88, 24);
    label->setBounds (8, 64, 80, 24);
    txtIP->setBounds (96, 64, 128, 24);
    label2->setBounds (8, 88, 80, 24);
    txtPort->setBounds (96, 88, 128, 24);
    btnConnect->setBounds (16, 120, 104, 24);
    btnDisconnect->setBounds (120, 120, 104, 24);
    lblStatus->setBounds (8, 152, 216, 24);
    lblInfo->setBounds (8, 176, 216, 24);
    lblAudio->setBounds (8, 200, 216, 24);
    optServer->setBounds (8, 8, 80, 24);
    optClient->setBounds (96, 8, 88, 24);
    label3->setBounds (8, 224, 80, 24);
    label4->setBounds (8, 248, 80, 24);
    btnReset->setBounds (16, 280, 208, 24);
    cbxLatency->setBounds (96, 224, 128, 24);
    cbxQuality->setBounds (96, 248, 128, 24);
    //[UserResized] Add your own custom resize handling here..
    barBufferL->setBounds (8, 312, 216, 24);
    barBufferR->setBounds (8, 336, 216, 24);
    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == optSender)
    {
        //[UserButtonCode_optSender] -- add your button handler code here..
        parent.senderSelected();
        //[/UserButtonCode_optSender]
    }
    else if (buttonThatWasClicked == optReceiver)
    {
        //[UserButtonCode_optReceiver] -- add your button handler code here..
        parent.receiverSelected();
        //[/UserButtonCode_optReceiver]
    }
    else if (buttonThatWasClicked == btnConnect)
    {
        //[UserButtonCode_btnConnect] -- add your button handler code here..
        int port = txtPort->getText().getIntValue();
        parent.connectClicked(txtIP->getText(), port);
        //[/UserButtonCode_btnConnect]
    }
    else if (buttonThatWasClicked == btnDisconnect)
    {
        //[UserButtonCode_btnDisconnect] -- add your button handler code here..
        parent.connectionLost();
        //[/UserButtonCode_btnDisconnect]
    }
    else if (buttonThatWasClicked == optServer)
    {
        //[UserButtonCode_optServer] -- add your button handler code here..
        parent.serverSelected();
        //[/UserButtonCode_optServer]
    }
    else if (buttonThatWasClicked == optClient)
    {
        //[UserButtonCode_optClient] -- add your button handler code here..
        parent.clientSelected();
        //[/UserButtonCode_optClient]
    }
    else if (buttonThatWasClicked == btnReset)
    {
        //[UserButtonCode_btnReset] -- add your button handler code here..
        parent.changeBufferLen();
        //[/UserButtonCode_btnReset]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbxLatency)
    {
        //[UserComboBoxCode_cbxLatency] -- add your combo box handling code here..
        parent.latencySelected(cbxLatency->getText());
        //[/UserComboBoxCode_cbxLatency]
    }
    else if (comboBoxThatHasChanged == cbxQuality)
    {
        //[UserComboBoxCode_cbxQuality] -- add your combo box handling code here..
        parent.qualitySelected(cbxQuality->getText());
        //[/UserComboBoxCode_cbxQuality]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainComponent::fillAllControls(){
    optServer->setToggleState(parent.isServer(), dontSendNotification);
    optClient->setToggleState(parent.isClient(), dontSendNotification);
    optSender->setToggleState(parent.isSender(), dontSendNotification);
    optReceiver->setToggleState(parent.isReceiver(), dontSendNotification);
    if(parent.conn != nullptr && parent.conn->getConnectedHostName() != ""){
        txtIP->setText(parent.conn->getConnectedHostName());
    }
    txtPort->setText(String(parent.getPort()), dontSendNotification);
    optServer->setEnabled(!parent.isLinkUp());
    optClient->setEnabled(!parent.isLinkUp());
    optSender->setEnabled(!parent.isLinkUp() && !parent.isSenderReady() && parent.isServer());
    optReceiver->setEnabled(!parent.isLinkUp() && !parent.isSenderReady() && parent.isServer());
    txtIP->setEnabled(!parent.isLinkUp() && !parent.isSenderReady() && parent.isClient());
    txtPort->setEnabled(!parent.isLinkUp() && !parent.isSenderReady());
    btnConnect->setEnabled(!parent.isLinkUp() && !parent.isSenderReady() && (parent.isServer() || parent.isClient()));
    btnDisconnect->setEnabled(parent.isLinkUp() || parent.isSenderReady());
    cbxQuality->setEnabled(parent.isSender());
    if(parent.getAudioType() == PACKET_TYPE_AUDIO_FLOAT32){
        cbxQuality->setSelectedId(1, dontSendNotification);
    }else if(parent.getAudioType() == PACKET_TYPE_AUDIO_INT16){
        cbxQuality->setSelectedId(2, dontSendNotification);
    }else if(parent.getAudioType() == PACKET_TYPE_AUDIO_DPCM){
        cbxQuality->setSelectedId(3, dontSendNotification);
    }
    if(parent.getLatency() == 0.25f){
        cbxLatency->setSelectedId(1, dontSendNotification);
    }else if(parent.getLatency() == 0.5f){
        cbxLatency->setSelectedId(2, dontSendNotification);
    }else if(parent.getLatency() == 1.0f){
        cbxLatency->setSelectedId(3, dontSendNotification);
    }else if(parent.getLatency() == 2.0f){
        cbxLatency->setSelectedId(4, dontSendNotification);
    }else if(parent.getLatency() == 4.0f){
        cbxLatency->setSelectedId(5, dontSendNotification);
    }
}
void MainComponent::timerCallback(){
    if(parent.updategui){
        fillAllControls();
        parent.updategui = false;
    }
    lblStatus->setText(parent.statusMsg, dontSendNotification);
    lblInfo->setText(parent.infoMsg, dontSendNotification);
    lblAudio->setText(parent.audioMsg, dontSendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component, public Timer" constructorParams="HqstreamerAudioProcessor&amp; p"
                 variableInitialisers="parent(p)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="232"
                 initialHeight="368">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TOGGLEBUTTON name="new toggle button" id="2f11532f5dbc2bcd" memberName="optSender"
                virtualName="" explicitFocusOrder="0" pos="8 32 80 24" buttonText="Sender"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="c221e7ff956f005a" memberName="optReceiver"
                virtualName="" explicitFocusOrder="0" pos="96 32 88 24" buttonText="Receiver"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="d7659a616379461" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 64 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Remote IP:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="92cd3aa02725f20a" memberName="txtIP"
              virtualName="" explicitFocusOrder="0" pos="96 64 128 24" initialText="127.0.0.1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="d3cb6949aff6ee7f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 88 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="ec1a329fb8dde423" memberName="txtPort"
              virtualName="" explicitFocusOrder="0" pos="96 88 128 24" initialText="51478"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="bedf667b518431aa" memberName="btnConnect"
              virtualName="" explicitFocusOrder="0" pos="16 120 104 24" buttonText="Connect"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5012d2e485a400c4" memberName="btnDisconnect"
              virtualName="" explicitFocusOrder="0" pos="120 120 104 24" buttonText="Disconnect"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="267974bd88701bd9" memberName="lblStatus"
         virtualName="" explicitFocusOrder="0" pos="8 152 216 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&lt;Status&gt;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6411dc54d14d9495" memberName="lblInfo" virtualName=""
         explicitFocusOrder="0" pos="8 176 216 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&lt;Connection info&gt;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="11a41bb3bf592002" memberName="lblAudio"
         virtualName="" explicitFocusOrder="0" pos="8 200 216 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&lt;Audio info&gt;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="Server" id="1227e2e57a5488b" memberName="optServer" virtualName=""
                explicitFocusOrder="0" pos="8 8 80 24" buttonText="Server" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="Client" id="468ba3d3b9559d1d" memberName="optClient" virtualName=""
                explicitFocusOrder="0" pos="96 8 88 24" buttonText="Client" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="fd4df0bad6d27691" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 224 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Latency:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f3149efc5f6a8fa2" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 248 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Quality:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="b9d9e64ad5326f2" memberName="btnReset"
              virtualName="" explicitFocusOrder="0" pos="16 280 208 24" tooltip="Click here when something goes wrong"
              buttonText="Reset Stream Buffer" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="130fdd8bb5c707a7" memberName="cbxLatency"
            virtualName="" explicitFocusOrder="0" pos="96 224 128 24" editable="0"
            layout="33" items="250ms&#10;500ms&#10;1s&#10;2s&#10;4s" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="36ad04e4f84e9726" memberName="cbxQuality"
            virtualName="" explicitFocusOrder="0" pos="96 248 128 24" editable="0"
            layout="33" items="float32&#10;int16&#10;DPCM" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
