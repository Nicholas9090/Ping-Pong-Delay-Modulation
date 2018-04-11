/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class DelApseAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelApseAudioProcessor();
    ~DelApseAudioProcessor();

    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float panPosition;
    float carrierFrequency_;
    int numSamples;
    int getNumParameters();
    int delayBufLength;
    int waveform_;
    float ph;
    float inverseSampleRate;
    float depth_;
    float frequency_;
    float *channelData;
    float *delayData;
    float getParameter(int index);
    void setParameter(int index, float newValue);
    const String getParameterName(int index);
    const String getParameterText(int index);
    
    
    enum Parameters
    {
        kDelayTimeParam = 0, // 0
        kFeedbackParam, // 1
        kWetMixParam, // 2
        kRingParam, // 3
        kNumParameters // 4
    };
    
    float delayTime;
    float feedback;
    float wetMix;
    float ring;
    
private:
    
    AudioSampleBuffer delayBuffer;
    int delayBufferLength;
    int readIndex;
    int writeIndex;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelApseAudioProcessor)
};
