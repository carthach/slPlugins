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
static String onOffTextFunction (const Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

//==============================================================================
PluginProcessor::PluginProcessor()
{
    gainl = addExtParam (PARAM_GAIN_L, "Left",  "", "dB", {-100.0f, 100.0f, 0.0f, 5.0f}, 0.0f, 0.1f);
    gains = addExtParam (PARAM_GAIN_S, "Both",  "", "dB", {-100.0f, 100.0f, 0.0f, 5.0f}, 0.0f, 0.1f);
    gainr = addExtParam (PARAM_GAIN_R, "Right", "", "dB", {-100.0f, 100.0f, 0.0f, 5.0f}, 0.0f, 0.1f);
    clipp  = addExtParam (PARAM_CLIP,  "Clip",  "", "",   {   0.0f,   1.0f, 1.0f, 1.0f}, 1.0f, 0.1f, onOffTextFunction);
    
    gainl->conversionFunction  = [] (float in) { return Decibels::decibelsToGain (in); };
    gains->conversionFunction  = [] (float in) { return Decibels::decibelsToGain (in); };
    gainr->conversionFunction  = [] (float in) { return Decibels::decibelsToGain (in); };
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();

    if (isSmoothing())
    {
        int pos = 0;
        
        while (pos < numSamples)
        {
            auto workBuffer = sliceBuffer (buffer, pos, 1);

            workBuffer.applyGain (0, 0, 1, gainl->getProcValue (1));
            workBuffer.applyGain (1, 0, 1, gainr->getProcValue (1));
            workBuffer.applyGain (gains->getProcValue (1));
            
            pos++;
        }
    }
    else
    {
        buffer.applyGain (0, 0, numSamples, gainl->getProcValue (numSamples));
        buffer.applyGain (1, 0, numSamples, gainr->getProcValue (numSamples));
        buffer.applyGain (gains->getProcValue (numSamples));
    }
    
    if (clipp->getUserValue() != 0.0f)
        clip (buffer, -1.0f, 1.0f);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
