/*
  ==============================================================================

    DialLAF.cpp
    Created: 31 Jan 2024 7:31:29pm
    Author:  Deivids

  ==============================================================================
*/

#include "DialLAF.h"

CustomDial::CustomDial()
{

}

void CustomDial::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{

    float diameter = fmin(width, height) * .7;
    float radius = diameter * 0.5;
    float centerX = x + width * 0.5;
    float centerY = y + height * 0.5;
    float rx = centerX - radius;
    float ry = centerY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
    g.setColour(juce::Colour::fromRGB(115, 99, 86)); //center
    //g.setColour(juce::Colour::fromFloatRGBA(0.45098f, 0.3882f, 0.1764f, 1.0f)); //center
    g.fillEllipse(dialArea);

    g.setColour(juce::Colours::black.withAlpha(0.15f));//outline 
    g.drawEllipse(rx, ry, diameter, diameter, 10.0f);
    juce::Path dialTick;
    g.setColour(juce::Colours::black.darker(0.5));
    dialTick.addRectangle(0, -radius + 1, 3.0f, 21.0f);
    g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));


};