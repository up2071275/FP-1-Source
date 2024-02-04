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


    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;

    void drawTickBox(juce::Graphics& g, juce::Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        [[maybe_unused]] const bool isEnabled,
        [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
        [[maybe_unused]] const bool shouldDrawButtonAsDown) override;
};