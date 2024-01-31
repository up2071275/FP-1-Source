/*
  ==============================================================================

    SliderLAF.h
    Created: 31 Jan 2024 7:30:54pm
    Author:  Deivids

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class CustomSlider : public juce::LookAndFeel_V4
{
public:
    CustomSlider();


    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider);
};