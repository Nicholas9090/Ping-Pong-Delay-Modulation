/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DelApseAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Timer,
                                     public Slider::Listener
{
public:
    //==============================================================================
    DelApseAudioProcessorEditor (DelApseAudioProcessor& p);
    ~DelApseAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* spiral_jpg;
    static const int spiral_jpgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DelApseAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> sliderDelayTime;
    ScopedPointer<Slider> sliderFeedback;
    ScopedPointer<Slider> sliderMix;
    ScopedPointer<Label> labelDelayTime;
    ScopedPointer<Label> labelFeedback;
    ScopedPointer<Label> labelMix;
    ScopedPointer<Slider> sliderRing;
    ScopedPointer<Label> labelRing;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelApseAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
