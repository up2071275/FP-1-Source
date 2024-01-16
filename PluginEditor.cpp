/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters/Parameters.h"

//==============================================================================
FilterTripAudioProcessorEditor::FilterTripAudioProcessorEditor (FilterTripAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    gainAttachtment(p._treeState, gainID, gainSlider),
    wetDryAttachtment(p._treeState, mixID, wetDrySlider),
    outputAttachtment(p._treeState, outputID, outputSlider),
    userCutoffAttachtment(p._treeState, userCutoffID, userCutoffSlider),
    envelopePercentageAttachtment(p._treeState, envelopePercentageID, envelopePercentageSlider),
    userAttackAttachtment(p._treeState, userAttackID, userAttackSlider),
    userReleaseAttachtment(p._treeState, userReleaseID, userReleaseSlider),

    filterModelAttachtment(p._treeState, filterModelID, filterModelCombo)
{

    wetDrySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    userCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    envelopePercentageSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    userAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    userReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    wetDrySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    outputSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    userCutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    envelopePercentageSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    userAttackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    userReleaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);

    wetDrySlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    outputSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    gainSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userCutoffSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    envelopePercentageSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userAttackSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userReleaseSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setResizable(true,true);
    setResizeLimits(420, 270, 1680, 1080);
    getConstrainer()->setFixedAspectRatio(1.55);

    setSize(840, 540);
    startTimerHz(240);

    //addAndMakeVisible(wetDrySlider);
    //addAndMakeVisible(outputSlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(userCutoffSlider);
    addAndMakeVisible(envelopePercentageSlider);
    addAndMakeVisible(filterModelCombo);
    addAndMakeVisible(userAttackSlider);
    addAndMakeVisible(userReleaseSlider);

    //addAndMakeVisible(wetDryLabel);
    //addAndMakeVisible(outputLabel);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(userCutoffLabel);
    addAndMakeVisible(envelopePercentageLabel);
    addAndMakeVisible(userAttackLabel);
    addAndMakeVisible(userReleaseLabel);


    wetDryLabel.setText("Wet/Dry", juce::dontSendNotification);
    outputLabel.setText("Output", juce::dontSendNotification);
    gainLabel.setText("Filter Gain", juce::dontSendNotification);
    userCutoffLabel.setText("Filter Frequency", juce::dontSendNotification);
    envelopePercentageLabel.setText("Envelope Percentage", juce::dontSendNotification);
    userAttackLabel.setText("Filter Attack", juce::dontSendNotification);
    userReleaseLabel.setText("Filter Release", juce::dontSendNotification);

    wetDryLabel.attachToComponent(&wetDrySlider, false);
    outputLabel.attachToComponent(&outputSlider, false);
    gainLabel.attachToComponent(&gainSlider, false);
    userCutoffLabel.attachToComponent(&userCutoffSlider, false);
    envelopePercentageLabel.attachToComponent(&envelopePercentageSlider, false);
    userAttackLabel.attachToComponent(&userAttackSlider, false);
    userReleaseLabel.attachToComponent(&userReleaseSlider, false);

    wetDryLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setJustificationType(juce::Justification::centred);
    userCutoffLabel.setJustificationType(juce::Justification::centred);
    envelopePercentageLabel.setJustificationType(juce::Justification::centred);
    userAttackLabel.setJustificationType(juce::Justification::centred);
    userReleaseLabel.setJustificationType(juce::Justification::centred);

    filterModelCombo.setTextWhenNothingSelected("Filter Model Selection");
    filterModelCombo.addItemList(audioProcessor.filterModels, 1);

}

FilterTripAudioProcessorEditor::~FilterTripAudioProcessorEditor()
{
}

//==============================================================================
void FilterTripAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::lightslategrey);
    auto bounds = getLocalBounds().toFloat();
  

    g.setColour(juce::Colours::floralwhite);
    g.setFont(36.0f);
    g.drawFittedText("Filter Trip", getLocalBounds(), juce::Justification::centredTop, 36);

    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::rebeccapurple);
    juce::Path path;
    const int numSamples = mEnvelopeBuffer.getNumSamples();
    for (int i = 0; i < numSamples; i++) {
        const float x = getWidth() * static_cast<float>(i) / static_cast<float>(numSamples - 1);
        const float y = (getHeight() * (1.0f - mEnvelopeBuffer.getSample(0, i))) - (getHeight() * 0.35);

        if (i == 0) {
            path.startNewSubPath(x, y);
        }
        else {
            path.lineTo(x, y);
        }
    }
    g.strokePath(path, juce::PathStrokeType(2.0f));
    // (Our component is opaque, so we must completely fill the background with a solid colour)



}

void FilterTripAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.15;
    auto sliderSize = getWidth() * 0.15;
    auto sliderYpos = topMargin * 4.75;
 
    //userAttackSlider.setBounds(leftMargin * 1.5, sliderYpos , sliderSize/2, sliderSize/2);
    //userReleaseSlider.setBounds(leftMargin * 1.5, sliderYpos *1.225 , sliderSize / 2, sliderSize / 2);


    //wetDrySlider.setBounds(leftMargin * 6.5, sliderYpos, sliderSize, sliderSize);
    userAttackSlider.setBounds(leftMargin *13, sliderYpos, sliderSize, sliderSize);
    userReleaseSlider.setBounds(leftMargin * 6.5, sliderYpos, sliderSize, sliderSize);

    gainSlider.setBounds(leftMargin * 26, sliderYpos, sliderSize, sliderSize);
    //userCutoffSlider.setBounds(leftMargin * 19.5, sliderYpos, sliderSize, sliderSize);
    userCutoffSlider.setBounds(leftMargin * 19.5, sliderYpos, sliderSize, sliderSize);
    //outputSlider.setBounds(leftMargin * 26, sliderYpos, sliderSize, sliderSize);
    envelopePercentageSlider.setBounds(leftMargin * 32.5, sliderYpos, sliderSize, sliderSize);
    filterModelCombo.setBounds(leftMargin * 40, sliderYpos, sliderSize - leftMargin, sliderSize * 0.33);

    /* 
    wetDrySlider.setBounds(100, 390, 100, 100);
    gainSlider.setBounds(200, 390, 100, 100);
    outputSlider.setBounds(300, 390, 100, 100);
    userCutoffSlider.setBounds(400, 390, 100, 100);
    envelopePercentageSlider.setBounds(500, 390, 100, 100);

    filterModelCombo.setBounds(600, 390, 150, 50);
    */


    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
