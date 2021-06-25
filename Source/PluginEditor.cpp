/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TistortionAudioProcessorEditor::TistortionAudioProcessorEditor (TistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //drive
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(driveSlider);
    
    addAndMakeVisible (driveLabel);
    driveLabel.setText ("Drive", juce::dontSendNotification);
    driveLabel.attachToComponent (&driveSlider, false);


    driveSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVE", driveSlider);
    //===============================================================================================================
    //range
    rangeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rangeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(rangeSlider);
    
    addAndMakeVisible (rangeLabel);
    rangeLabel.setText ("Range", juce::dontSendNotification);
    rangeLabel.attachToComponent (&rangeSlider, false);
    
    rangeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RANGE", rangeSlider);
    //===============================================================================================================
    //blend
    blendSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    blendSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(blendSlider);
    
    addAndMakeVisible (blendLabel);
    blendLabel.setText ("Blend", juce::dontSendNotification);
    blendLabel.attachToComponent (&blendSlider, false);
    
    blendSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BLEND", blendSlider);
    //===============================================================================================================
    //volume
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(volumeSlider);
    
    addAndMakeVisible (volumeLabel);
    volumeLabel.setText ("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent (&volumeSlider, false);
    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeSlider);

     //===============================================================================================================

    
    setSize (800, 300);
}

TistortionAudioProcessorEditor::~TistortionAudioProcessorEditor()
{
}

//==============================================================================
void TistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TistortionAudioProcessorEditor::resized()
{
    const int hop = getWidth()/4;
    
    driveSlider.setBounds(0, getHeight()/5, 200, 200);
    rangeSlider.setBounds(hop, getHeight()/5, 200, 200);
    blendSlider.setBounds(hop*2, getHeight()/5, 200, 200);
    volumeSlider.setBounds(hop*3, getHeight()/5, 200, 200);
}
