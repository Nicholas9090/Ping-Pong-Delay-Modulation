/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelApseAudioProcessor::DelApseAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Some feedback
    feedback = 0.5;
    
    // Delay of 0.2 seconds
    delayTime = 0.25;
    
    // Start reading from the start of the circular buffer
    readIndex = 0;
    
    // Initial value of wet mix
    wetMix = 0.5;
    
    //Initial value for modulation
    ring = 0;
    
    // Set the write index ahead of the read index
    writeIndex = delayTime;
    
    // Initial delay buffer size
    delayBufLength = 0;
}

DelApseAudioProcessor::~DelApseAudioProcessor()
{
}

//==============================================================================
const String DelApseAudioProcessor::getName() const

{
    
    return JucePlugin_Name;
}

int DelApseAudioProcessor::getNumParameters()
{
    return kNumParameters;
}

float DelApseAudioProcessor::getParameter(int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kDelayTimeParam: return delayTime;
        case kFeedbackParam: return feedback;
        case kWetMixParam: return wetMix;
        case kRingParam: return ring;
        default: return 0.0f;
    }
}

void DelApseAudioProcessor::setParameter(int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kDelayTimeParam:
            delayTime = newValue;
            // IMPORTANT: calculate the position of the readIndex relative to the write
            // i.e. the delay time in samples
            readIndex = (int)(writeIndex - ((delayTime/3) * delayBufferLength)
                              + delayBufferLength) % delayBufferLength;
            break;
        case kFeedbackParam:
            feedback = newValue;
            break;
        case kWetMixParam:
            wetMix = newValue;
            break;
        case kRingParam:
            ring = newValue;
            break;
        default:
            break;
    }
}

const String DelApseAudioProcessor::getParameterName(int index)
{
    switch (index)
    {
        case kDelayTimeParam: return "delay time";
        case kFeedbackParam: return "feedback";
        case kWetMixParam: return "wet mix";
        case kRingParam: return "ring";
        default: break;
    }
    
    return String::empty;
}

const String DelApseAudioProcessor::getParameterText(int index)
{
    return String(getParameter(index), 2);
}

bool DelApseAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelApseAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelApseAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelApseAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelApseAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelApseAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelApseAudioProcessor::setCurrentProgram (int index)
{
}

const String DelApseAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelApseAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelApseAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Maximum delay of 1 second
    delayBufferLength = (int)(sampleRate * 3);
    
    // Set the buffer to 1 channel of the size of delayBufferLength using setSize
    delayBuffer.setSize(2, delayBufferLength);
    
    // Set all the samples in the buffer to zero
    delayBuffer.clear();
    
    // IMPORTANT: calculate the position of the read index relative to the write index
    // i.e. the delay time in samples
    readIndex = (int)(writeIndex - ((delayTime/3) * delayBufferLength)
                      + delayBufferLength) % delayBufferLength;
}

void DelApseAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelApseAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelApseAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear (i, 0, buffer.getNumSamples());
    
    //calculate p'
    float pDash = (panPosition + 1.0) / 2.0;
    
    // channelData is an array of length numSamples which contain
    // the audio for one channel
    float *channelDataL = buffer.getWritePointer(0);
    float *channelDataR = buffer.getWritePointer(1);

    // delayData is the circular buffer for implementing the delay
    float* delayDataL = delayBuffer.getWritePointer(0);
    float* delayDataR = delayBuffer.getWritePointer(1);

    float wetMix = 0.5;
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        
        // Calculate the next output sample (current input sample + delayed version)
        float outputSampleL = (channelDataL[i] + (wetMix * delayDataL[readIndex]));
        float outputSampleR = (channelDataR[i] + (wetMix * delayDataR[readIndex]));

        // Write the current input into the delay buffer along with the delayed sample
        delayDataL[writeIndex] = channelDataR[i] + (delayDataR[readIndex] * feedback);
        delayDataR[writeIndex] = channelDataL[i] + (delayDataL[readIndex] * feedback);
        
        const float in = channelData[i];
        
        //Multiplication of the waveform with a carrier signal
        channelData[i] = in * sinf(2.0 * M_PI * ph);
        
        //Renewing the phase within a predetermined range
        ph += carrierFrequency_*inverseSampleRate;
        if(ph >= 1.0)
            ph -= 1.0;
        
        // Increment the read index then check to see if it's greater than the buffer length
        // If so wrap back around to zero
        if (++readIndex >= delayBufferLength)
            readIndex = 0;
        // Same with write index
        if (++writeIndex >= delayBufferLength)
            writeIndex = 0;

        // Assign output sample computed above to the output buffer
        channelDataL[i] = outputSampleL;
        channelDataR[i] = outputSampleR;
        
        //channelDataL and channelDataR are pointers to arrays of length numSamples which // contain the audio for one channel. You repeat this for each channel
        float *channelDataL = buffer.getWritePointer(0);
        float *channelDataR = buffer.getWritePointer(1);
    }
        
        
        // ..do something to the data...
}


//==============================================================================
bool DelApseAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelApseAudioProcessor::createEditor()
{
    return new DelApseAudioProcessorEditor (*this);
}

//==============================================================================
void DelApseAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelApseAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelApseAudioProcessor();
}
