/*
  ==============================================================================

    KnobDesign.h
    Created: 28 Jun 2021 1:21:21am
    Author:  irishill

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

class KnobDesign : public juce::LookAndFeel_V4
{
    public:
    KnobDesign(){
        setColour(Slider::rotarySliderOutlineColourId, Colours::rebeccapurple);
        setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
        setColour(Slider::thumbColourId, Colours::yellowgreen);
    }
};
