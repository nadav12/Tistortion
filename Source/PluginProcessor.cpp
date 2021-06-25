/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TistortionAudioProcessor::TistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

TistortionAudioProcessor::~TistortionAudioProcessor()
{
}

//==============================================================================
const juce::String TistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void TistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void TistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto drive = apvts.getRawParameterValue("DRIVE");
    auto range = apvts.getRawParameterValue("RANGE");
    auto blend = apvts.getRawParameterValue("BLEND");
    auto volume = apvts.getRawParameterValue("VOLUME");
    
    float driver = drive->load();
    float ranger = range->load();
    float blender = blend->load();
    float volumer = volume->load();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for(int sample = 0; sample < buffer.getNumSamples(); sample ++)
        {
            auto cleanSignal = buffer.getSample(channel, sample);
            
            channelData[sample] *= (driver * ranger) / 2;
            
            channelData[sample] = ((((((2.f / M_PI) * atan(channelData[sample])) * blender) + (cleanSignal * (1.f - blender)))/2) * volumer);
            
        }
    }
}

//==============================================================================
bool TistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TistortionAudioProcessor::createEditor()
{
    return new TistortionAudioProcessorEditor (*this);
}

//==============================================================================
void TistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TistortionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TistortionAudioProcessor:: createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.f, 1.f, 0.25555));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RANGE", "Range", 0.f, 3000.f, 0.3006));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BLEND", "Blend", 0.f, 1.f, 0.25555));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.f, 3.f, 1.f));
    
    return {params.begin(), params.end()};
}
