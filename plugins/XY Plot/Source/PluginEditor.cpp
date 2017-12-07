/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : slAudioProcessorEditor (p), processor (p)
{
    addAndMakeVisible (&scope);
    
    for (slParameter* pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp->getUid() == PARAM_MODE)
            pc = new Select (pp);
        else if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (7, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
    
    for (auto pp : processor.getPluginParameters())
        pp->addListener (this);
    
    updateScope();
}

PluginEditor::~PluginEditor()
{
    for (auto pp : processor.getPluginParameters())
        pp->removeListener (this);
    
    ScopedLock sl (processor.lock);
    processor.editor = nullptr;
}

//==============================================================================

Rectangle<int> PluginEditor::getGridArea (int x, int y, int w, int h)
{
    return Rectangle<int> (getWidth() - inset - cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    slAudioProcessorEditor::resized();

    auto rc = Rectangle<int> (inset, headerHeight + inset, getWidth() - cx - 2 * inset, getHeight() - headerHeight - 2 * inset);
    
    scope.setBounds (rc);
    
    componentForId (PARAM_MODE)->setBounds (getGridArea (0, 0));
    componentForId (PARAM_LOG)->setBounds (getGridArea (0, 1));
}

void PluginEditor::updateScope()
{
}