/*
  ==============================================================================

    LabelLAF.h
    Created: 3 Apr 2024 7:39:31pm
    Author:  DEIVIDS

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLabel : public juce::LookAndFeel_V4
{
public:
    CustomLabel();

    void drawLabel(juce::Graphics& g, juce::Label& label) override;
 
};