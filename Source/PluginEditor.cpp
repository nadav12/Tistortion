/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "KnobDesign.h"
#include "TSlider.h"

//==============================================================================
TistortionAudioProcessorEditor::TistortionAudioProcessorEditor (TistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //drive

    driveSlider.setLookAndFeel(&knobDesign);
    addAndMakeVisible(driveSlider);
    
    addAndMakeVisible (driveLabel);
    driveLabel.setText ("Drive", juce::dontSendNotification);
    driveLabel.attachToComponent (&driveSlider, false);
    driveLabel.setJustificationType(juce::Justification::centredBottom);


    driveSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVE", driveSlider);
    //===============================================================================================================
    //range

    rangeSlider.setLookAndFeel(&knobDesign);
    addAndMakeVisible(rangeSlider);
    
    addAndMakeVisible (rangeLabel);
    rangeLabel.setText ("Range", juce::dontSendNotification);
    rangeLabel.attachToComponent (&rangeSlider, false);
    rangeLabel.setJustificationType(juce::Justification::centredBottom);

    
    rangeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RANGE", rangeSlider);
    //===============================================================================================================
    //blend

    blendSlider.setLookAndFeel(&knobDesign);

    addAndMakeVisible(blendSlider);
    
    addAndMakeVisible (blendLabel);
    blendLabel.setText ("Blend", juce::dontSendNotification);
    blendLabel.attachToComponent (&blendSlider, false);
    blendLabel.setJustificationType(juce::Justification::centredBottom);
    
    blendSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BLEND", blendSlider);
    //===============================================================================================================
    //volume

    volumeSlider.setLookAndFeel(&knobDesign);

    addAndMakeVisible(volumeSlider);
    
    addAndMakeVisible (volumeLabel);
    volumeLabel.setText ("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent (&volumeSlider, false);
    volumeLabel.setJustificationType(juce::Justification::centredBottom);

    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeSlider);

     //===============================================================================================================

    setResizable(true, true);
    setResizeLimits(400, 300, 1000, 1000);
    setSize (800, 300);
}

TistortionAudioProcessorEditor::~TistortionAudioProcessorEditor()
{
}

//==============================================================================
void TistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkslateblue);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Tistortion by Twina", getLocalBounds(), juce::Justification::centredTop, 1);
}

void TistortionAudioProcessorEditor::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
     int knobHeight = 200;
     int knobWidth = 200;

    juce::FlexBox flexbox;
    flexbox.flexDirection = juce::FlexBox::Direction::row;
    flexbox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexbox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexbox.alignContent = juce::FlexBox::AlignContent::spaceAround;

    flexbox.items.add(juce::FlexItem(knobHeight, knobWidth, driveSlider));
    flexbox.items.add(juce::FlexItem(knobHeight, knobWidth, rangeSlider));
    flexbox.items.add(juce::FlexItem(knobHeight, knobWidth, blendSlider));
    flexbox.items.add(juce::FlexItem(knobHeight, knobWidth, volumeSlider));
    flexbox.performLayout(bounds);

}
