/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace gin;

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : GinAudioProcessorEditor (p, 60, 100), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }

    addAndMakeVisible (meter);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (reductionMeter);
    reductionMeter.setTopDown (true);

    addAndMakeVisible (scope);
    scope.setNumChannels (3);
    scope.setTriggerMode (TriggeredScope::None);
    scope.setNumSamplesPerPixel (256);
    scope.setVerticalZoomFactor (2.0);
    scope.setVerticalZoomOffset (-0.5, 0);
    scope.setVerticalZoomOffset (-0.5, 1);
    scope.setVerticalZoomOffset (-0.5, 2);
    scope.setColour (TriggeredScope::traceColourId + 0, Colours::transparentBlack);
    scope.setColour (TriggeredScope::envelopeColourId + 0, Colours::orange);
    scope.setColour (TriggeredScope::traceColourId + 1, Colours::transparentBlack);
    scope.setColour (TriggeredScope::envelopeColourId + 1, Colours::white);
    scope.setColour (TriggeredScope::traceColourId + 2, Colours::red);
    scope.setColour (TriggeredScope::envelopeColourId + 2, Colours::transparentBlack);

    setGridSize (7, 2);

	for (auto pp : proc.getPluginParameters())
        pp->addListener (this);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    for (auto p : proc.getPluginParameters())
        p->removeListener (this);
}

//==============================================================================
void CompressorAudioProcessorEditor::parameterChanged (Parameter*)
{
    meter.repaint();
}

void CompressorAudioProcessorEditor::resized()
{
    GinAudioProcessorEditor::resized();

    componentForParam (*proc.input)->setBounds (getGridArea (0, 0));
    componentForParam (*proc.attack)->setBounds (getGridArea (1, 0));
    componentForParam (*proc.release)->setBounds (getGridArea (2, 0));
    componentForParam (*proc.ratio)->setBounds (getGridArea (3, 0));
    componentForParam (*proc.threshold)->setBounds (getGridArea (4, 0));
    componentForParam (*proc.knee)->setBounds (getGridArea (5, 0));
    componentForParam (*proc.output)->setBounds (getGridArea (6, 0));
    
    auto rc = getGridArea (0, 1, 7, 1);
    
    inputMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);
    
    meter.setBounds (rc.removeFromLeft (rc.getHeight()));
    rc.removeFromLeft (4);

    reductionMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);
    
    outputMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);
    
    scope.setBounds (rc);
}
