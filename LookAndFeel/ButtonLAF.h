/*
  ==============================================================================

    ButtonLAF.h
    Created: 4 Feb 2024 4:45:05pm
    Author:  DEIVIDS

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomButton : public juce::LookAndFeel_V4
{
public:
    CustomButton();


    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
};