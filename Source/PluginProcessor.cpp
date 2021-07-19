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
, apvts(*this, nullptr, "Parameters", createParameters()),
filter(juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(44100, 1000.0f))
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
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
//    mono.setType(juce::dsp::StateVariab leTPTFilterType::lowpass);
    reset();
//    updateFilter();
    filter.prepare(spec);
    
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

void TistortionAudioProcessor::updateFilter(float freqCut)
{
//    auto freq = apvts.getRawParameterValue("LOWCUT");
//    float freqCut = freq->load();
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(44100, freqCut);
}

void TistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto drive = apvts.getRawParameterValue("DRIVE");
    auto range = apvts.getRawParameterValue("RANGE");
    auto blend = apvts.getRawParameterValue("BLEND");
    auto volume = apvts.getRawParameterValue("VOLUME");
    auto cutOff = apvts.getRawParameterValue("LOWCUT");
    
    float driver = drive->load();
       float ranger = range->load();
       float blender = blend->load();
       float volumer = volume->load();
       float cutOffer = cutOff->load();
       
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    updateFilter(cutOffer);

    
    auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    filter.process(context);

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for(int sample = 0; sample < buffer.getNumSamples(); sample ++)
        {
            auto cleanSignal = buffer.getSample(channel, sample);
            
            cleanSignal *= (1.0f - blender);
            channelData[sample] *= (driver * ranger);
            auto drivenSignal = 2.0f / M_PI * atan(channelData[sample]);
            
                
//            auto drivenSignal = 2.0f / M_PI * atan(channelData[sample]);
//            auto drivenSignal = 2.0f / M_PI * atan(M_PI/2.0f * channelData[sample]);
//            auto drivenSignal =  tanh( channelData[sample]);
//            auto drivenSignal =   channelData[sample]/(1+ abs(channelData[sample]));
//            auto drivenSignal =   tanh(sin(channelData[sample]));
            channelData[sample] = (drivenSignal * blender + cleanSignal) * 0.5f * volumer;
            
//            channelData[sample] = ((((((2.f / M_PI) * atan(channelData[sample]))  * blender) + (cleanSignal * (1.f - blender)))/2) * volumer);


        }
//        
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
    DBG("getstate");
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    DBG("setstate");
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

void TistortionAudioProcessor::reset()
{
    
    filter.reset();
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
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.f, 1.f, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RANGE", "Range", 0.f, 3000.f, 3.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BLEND", "Blend", 0.f, 1.f, 0.6f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.f, 3.f, 1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOWCUT", "LowCut", 20.f, 1500.f, 1000.f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("DIST", "Dist", StringArray ("Pi", "Pi2", "Tanh"),0));
    
    return {params.begin(), params.end()};
}


