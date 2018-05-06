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
String onOffTextFunction (const Parameter& p)
{
    return p.getUserValue() > 0.0f ? "On" : "Off";
}

String chanTextFunction (const Parameter& p, float v)
{
    switch (int (v))
    {
        case -1: return "Ave";
        case 0:  return "Left";
        case 1:  return "Right";
        default: return "";
    }
}

String modeTextFunction (const Parameter& p, float v)
{
    switch (int (v))
    {
        case 0:  return "Off";
        case 1:  return "Up";
        case 2:  return "Down";
        default: return "";
    }
}

String intTextFunction (const Parameter& p, float v)
{
    return String (int (v));
}

String tlTextFunction (const Parameter& p, float v)
{
    return String (v, 2);
}


//==============================================================================
PluginProcessor::PluginProcessor()
{
    addPluginParameter (new Parameter (PARAM_SAMPLES_PER_PIXEL,       "Samp/px",       "", "",     1.0f,   48.0f,  1.0f,    1.0f, 1.0f, intTextFunction));
    addPluginParameter (new Parameter (PARAM_VERTICAL_ZOOM,           "Zoom",          "", "",     0.1f,   100.0f, 0.0f,    1.0f, 0.3f));
    addPluginParameter (new Parameter (PARAM_VERTICAL_OFFSET_L,       "Offset L",      "", "",     -2.0f,  2.0f,   0.0f,    0.0f, 1.0f));
    addPluginParameter (new Parameter (PARAM_VERTICAL_OFFSET_R,       "Offset R",      "", "",     -2.0f,  2.0f,   0.0f,    0.0f, 1.0f));
    addPluginParameter (new Parameter (PARAM_TRIGGER_CHANNEL,         "Trigger Chan",  "", "",     -1.0f,  1.0f,   1.0f,    0.0f, 1.0f, chanTextFunction));
    addPluginParameter (new Parameter (PARAM_TRIGGER_MODE,            "Trigger Mode",  "", "",     0.0f,   2.0f,   1.0f,    1.0f, 1.0f, modeTextFunction));
    addPluginParameter (new Parameter (PARAM_TRIGGER_LEVEL,           "Trigger Level", "", "",     -1.0f,  1.0f,   0.0f,    0.0f, 1.0f, tlTextFunction));
    addPluginParameter (new Parameter (PARAM_TRIGGER_POS,             "Trigger Pos",   "", "",      0.0f,  1.0f,   0.0f,    0.0f, 1.0f, tlTextFunction));
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    ScopedLock sl (lock);
    if (editor)
        editor->scope.addSamples (buffer);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    editor = new PluginEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
