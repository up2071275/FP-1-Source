/*
  ==============================================================================

    LabelLAF.cpp
    Created: 3 Apr 2024 7:39:31pm
    Author:  DEIVIDS

  ==============================================================================
*/

#include "LabelLAF.h"

CustomLabel::CustomLabel()
{

}


void CustomLabel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    // Custom drawing code for labels
    if (label.isEnabled())
    {
        g.setFont(label.getFont());
        g.setColour(juce::Colour::fromRGB(173, 29, 29));
        g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::right, true);
    }
};
