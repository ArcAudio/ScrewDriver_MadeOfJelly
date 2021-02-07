/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiProcessor.h"
//==============================================================================
/**
*/
class FaustPolyEngine;
class audio;

class KarplunkOgAudioProcessor  : public
juce::AudioProcessor,
juce::MidiKeyboardState
{
public:
    //==============================================================================
    KarplunkOgAudioProcessor();
    ~KarplunkOgAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState vTreeState;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    
    
    void setDamp (float damp);
    void keyOn(int pitch, int velocity);
    void keyOff(int pitch);
    
    MidiKeyboardState midiKeyState;
    
    std::atomic<int>noteCordX;
    std::atomic<int>noteCordY;
    
    std::atomic<bool>mouseIsDown;
    
    std::atomic<bool>mouseIsUp;

    
    void setMouseDown(bool f);
    void setMouseUp(bool f);
    void setNoteCords(float x,float y);
    
    
   // MidiProcessor midiProcess;
    
private:
    audio *driver;
    FaustPolyEngine *faustObject;
    float **inputs;
    float **outputs;
    
    float lastSliderValue;
    
    std::atomic<float>* dampParameter = nullptr;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KarplunkOgAudioProcessor)
};
