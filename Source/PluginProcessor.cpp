/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "karplunk6.h"
#include "Piano.h"
//==============================================================================
KarplunkOgAudioProcessor::KarplunkOgAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), vTreeState(*this,nullptr,"Parameters",createParameters())
#endif
{
    dampParameter = vTreeState.getRawParameterValue("SLIDER");
}

KarplunkOgAudioProcessor::~KarplunkOgAudioProcessor()
{
}

//==============================================================================
const juce::String KarplunkOgAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KarplunkOgAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KarplunkOgAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KarplunkOgAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}
void KarplunkOgAudioProcessor::keyOn(int pitch, int velocity)
{
    faustObject->keyOn(pitch,velocity);
}

void KarplunkOgAudioProcessor::keyOff(int pitch)
{
    faustObject->keyOff(pitch);
}

void KarplunkOgAudioProcessor::setDamp(float damp)
{
    faustObject->setParamValue("damping",damp);
}

void KarplunkOgAudioProcessor::setMouseDown(bool f)
{
    mouseIsDown.store(f);
}

void KarplunkOgAudioProcessor::setMouseUp(bool f)
{
    mouseIsUp.store(f);
}

void KarplunkOgAudioProcessor::setNoteCords(float x , float y)
{
    noteCordX.store(x);
    noteCordY.store(y);
}

double KarplunkOgAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KarplunkOgAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KarplunkOgAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KarplunkOgAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KarplunkOgAudioProcessor::getProgramName (int index)
{
    return {};
}

void KarplunkOgAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KarplunkOgAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    driver =  new dummyaudio(sampleRate,samplesPerBlock);
    faustObject = new FaustPolyEngine(NULL,driver,NULL);
    inputs = new float*[2];
    outputs = new float*[2];
    for (int channel = 0; channel < 2; ++channel){
        inputs[channel] = new float[samplesPerBlock];
        outputs[channel] = new float[samplesPerBlock];
    }
}

void KarplunkOgAudioProcessor::releaseResources()
{
    //delete faustObject;
    delete driver;
    for (int channel = 0; channel < 2; ++channel){
        delete[] inputs[channel];
        delete[] outputs[channel];
    }
    delete [] inputs;
    delete [] outputs;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KarplunkOgAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void KarplunkOgAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
   // float sliderValue = *vTreeState.getRawParameterValue("SLIDER");
    float sliderValue = *vTreeState.getRawParameterValue("SLIDER");
    setDamp(sliderValue);
  //  DBG(sliderValue);
  //  setDamp(sliderValue);
    setDamp(*dampParameter);
    
   // buffer.clear();
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel){
        for(int i=0; i<buffer.getNumSamples(); i++){
            inputs[channel][i] = *buffer.getWritePointer(channel,i);
        }
    }
    
    for (auto message: midiMessages)
    {
        auto currentMessage = message.getMessage();

        if (currentMessage.isNoteOn())
        {
            keyOn(currentMessage.getNoteNumber(), currentMessage.getVelocity());
        }
        if (currentMessage.isNoteOff())
        {
            keyOff(currentMessage.getNoteNumber());
        }
    }
    
    faustObject->compute(buffer.getNumSamples(),inputs,outputs);
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel){
        for(int i=0; i<buffer.getNumSamples(); i++){
            *buffer.getWritePointer(channel,i) = outputs[channel][i];
        }
    }
    
    midiKeyState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
   // dampParameter
    

//    if (sliderValue != lastSliderValue)
//    {
//
//        lastSliderValue = sliderValue;
//    }
}

//==============================================================================
bool KarplunkOgAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KarplunkOgAudioProcessor::createEditor()
{
    return new KarplunkOgAudioProcessorEditor (*this);
}

//==============================================================================
void KarplunkOgAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = vTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void KarplunkOgAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vTreeState.state.getType()))
            vTreeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KarplunkOgAudioProcessor();
}


AudioProcessorValueTreeState::ParameterLayout
    KarplunkOgAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    //Float
    parameters.push_back(std::make_unique<AudioParameterFloat>("SLIDER","Slider", 0.0f, 1.0f, 1.0f));
 
    return{parameters.begin(),parameters.end()};
}
