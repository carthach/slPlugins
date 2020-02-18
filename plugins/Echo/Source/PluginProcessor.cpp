/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

using namespace gin;

//==============================================================================
EchoAudioProcessor::EchoAudioProcessor()
{
    sync  = addExtParam ("sync",  "Sync",      "", "",   {   0.0f,   1.0f, 1.0f, 1.0f},    0.0f, 0.0f);
    time  = addExtParam ("time",  "Delay",     "", "",   {   0.0f,   5.0f, 0.0f, 1.0f},    1.0f, 0.0f);
    beat  = addExtParam ("beat",  "Delay",     "", "",   {   0.0f,  10.0f, 0.0f, 1.0f},    0.0f, 0.0f);
    fb    = addExtParam ("fb",    "Feedback",  "", "dB", {-100.0f,   0.0f, 0.0f, 5.0f},  -10.0f, 0.1f);
    cf    = addExtParam ("cf",    "Crossfeed", "", "dB", {-100.0f,   0.0f, 0.0f, 5.0f}, -100.0f, 0.1f);
    mix   = addExtParam ("mix",   "Mix",       "", "%",  {   0.0f, 100.0f, 0.0f, 1.0f},    0.0f, 0.1f);
    
    delay = addIntParam ("delay", "Delay",     "", "",   {   0.0f,   5.0f, 0.0f, 1.0f},    1.0f, 0.1f);
    
    fb->conversionFunction  = [] (float in) { return Decibels::decibelsToGain (in); };
    cf->conversionFunction  = [] (float in) { return Decibels::decibelsToGain (in); };
    mix->conversionFunction = [] (float in) { return in / 100.0f; };
}

EchoAudioProcessor::~EchoAudioProcessor()
{
}

//==============================================================================
void EchoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    GinProcessor::prepareToPlay (sampleRate, samplesPerBlock);
    
    stereoDelay.setSampleRate (sampleRate);
}

void EchoAudioProcessor::reset()
{
    GinProcessor::reset();
    
    stereoDelay.clear();
}

void EchoAudioProcessor::releaseResources()
{
}

void EchoAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    if (isSmoothing())
    {
        int todo = buffer.getNumSamples();
        int pos = 0;
        
        while (todo > 0)
        {
            int sz = std::min (8, todo);
            auto workBuffer = sliceBuffer (buffer, pos, sz);
            
            stereoDelay.setParams (time->getProcValueSmoothed (sz), mix->getProcValueSmoothed (sz),
                                   fb->getProcValueSmoothed (sz), cf->getProcValueSmoothed (sz));
            
            stereoDelay.process (workBuffer);
        }
    }
    else
    {
        stereoDelay.setParams (time->getProcValue(), mix->getProcValue(),
                               fb->getProcValue(), cf->getProcValue());
        
        stereoDelay.process (buffer);
    }
}

//==============================================================================
bool EchoAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* EchoAudioProcessor::createEditor()
{
    return new EchoAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EchoAudioProcessor();
}
