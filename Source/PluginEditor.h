/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OpenGLComponent.hpp"
//==============================================================================
/**
*/
class KarplunkOgAudioProcessorEditor  :
public juce::AudioProcessorEditor
//private juce::MidiInputCallback,
//private juce::MidiKeyboardStateListener
{
public:
    KarplunkOgAudioProcessorEditor (KarplunkOgAudioProcessor&);
    ~KarplunkOgAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
private:

    KarplunkOgAudioProcessor& audioProcessor;
    OpenGLComponent openGLComponent;
    Draggable3DOrientation draggableOrientation;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KarplunkOgAudioProcessorEditor)
};
