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
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override {
        auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width  * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // fill
        g.setColour (juce::Colours::blue);
        g.fillEllipse (rx, ry, rw, rw);
 
        // outline
        g.setColour (juce::Colours::skyblue);
        g.drawEllipse (rx, ry, rw, rw, 1.0f);
        
        juce::Path p;
        auto pointerLength = radius * 0.40f;
        auto pointerThickness = 3.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        g.setColour (juce::Colours::yellow);
          g.fillPath (p);
    }
   
//    KnobDesign(){
//        setColour(Slider::rotarySliderOutlineColourId, Colours::rebeccapurple);
//        setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
//        setColour(Slider::thumbColourId, Colours::yellowgreen);
//    }
};
