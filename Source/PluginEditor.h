/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TistortionAudioProcessorEditor (TistortionAudioProcessor&);
    ~TistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveSliderAttachment;
    
    juce::Slider rangeSlider;
    juce::Label rangeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeSliderAttachment;

    juce::Slider blendSlider;
    juce::Label blendLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> blendSliderAttachment;

    juce::Slider volumeSlider;
    juce::Label volumeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeSliderAttachment;


    TistortionAudioProcessor& audioProcessor;  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TistortionAudioProcessorEditor)
};
