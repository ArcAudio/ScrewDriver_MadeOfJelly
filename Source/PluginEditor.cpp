/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KarplunkOgAudioProcessorEditor::KarplunkOgAudioProcessorEditor (KarplunkOgAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), openGLComponent (p)
{
    addAndMakeVisible(openGLComponent);
    openGLComponent.setBounds(0, 0, 1690, 800);
    setSize (1690, 800);
}

KarplunkOgAudioProcessorEditor::~KarplunkOgAudioProcessorEditor()
{
}

//==============================================================================
void KarplunkOgAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void KarplunkOgAudioProcessorEditor::mouseDown(const MouseEvent& e)
{
}


void KarplunkOgAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
