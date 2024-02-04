/*
  ==============================================================================

    ComboLAF.h
    Created: 31 Jan 2024 7:31:05pm
    Author:  Deivids

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomCombo : public juce::LookAndFeel_V4
{
public:
    CustomCombo();

    void drawComboBox(juce::Graphics& g, int width, int height, bool,
        int, int, int, int, juce::ComboBox& box);

    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
        bool isSeparator, bool isActive,
        bool isHighlighted, bool isTicked,
        bool hasSubMenu, const juce::String& text,
        const juce::String& shortcutKeyText,
        const juce::Drawable* icon,
        const juce::Colour* textColourToUse);


};
