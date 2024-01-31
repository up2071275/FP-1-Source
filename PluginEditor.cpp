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
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    userCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    envelopePercentageSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    userAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    userReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    wetDrySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    outputSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 60, 20);
    userCutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 60, 20);
    envelopePercentageSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 60, 20);
    userAttackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 60, 20);
    userReleaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 60, 20);

    wetDrySlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    outputSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    gainSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userCutoffSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    envelopePercentageSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userAttackSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);
    userReleaseSlider.setRotaryParameters(0, juce::MathConstants<float>::twoPi, true);

    userAttackSlider.setLookAndFeel(&customDialLAF);
    userReleaseSlider.setLookAndFeel(&customDialLAF);
    gainSlider.setLookAndFeel(&customSliderLAF);
    userCutoffSlider.setLookAndFeel(&customSliderLAF);
    envelopePercentageSlider.setLookAndFeel(&customSliderLAF);
    filterModelCombo.setLookAndFeel(&customComboLAF);



    //LOADING IMAGE ASSETS 21.0.1.24
    Background = juce::ImageFileFormat::loadFrom(BinaryData::Background_png, BinaryData::Background_pngSize);
    Dial = juce::ImageFileFormat::loadFrom(BinaryData::Dial_png, BinaryData::Dial_pngSize);
    SliderDB = juce::ImageFileFormat::loadFrom(BinaryData::SliderDB_png, BinaryData::SliderDB_pngSize);
    SliderHZ = juce::ImageFileFormat::loadFrom(BinaryData::SliderHZ_png, BinaryData::SliderHZ_pngSize);
    //LOADING IMAGE ASSETS

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setResizable(false, false);
    //setResizeLimits(420, 270, 1680, 1080);
    //getConstrainer()->setFixedAspectRatio(1.55);

    //setSize(840, 540);
    setSize(630, 948);
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
    /*
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

    */

    filterModelCombo.setTextWhenNothingSelected("Filter Model Selection");
    filterModelCombo.addItemList(audioProcessor.filterModels, 1);


    

}

FilterTripAudioProcessorEditor::~FilterTripAudioProcessorEditor()
{

    userAttackSlider.setLookAndFeel(nullptr);
    userReleaseSlider.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    userCutoffSlider.setLookAndFeel(nullptr);
    envelopePercentageSlider.setLookAndFeel(nullptr);
    filterModelCombo.setLookAndFeel(nullptr);
}

//==============================================================================
void FilterTripAudioProcessorEditor::paint(juce::Graphics& g)
{
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::lightslategrey);

    //DRAWING ASSETS ON SCREEN 21.0.1.24
    g.drawImage(Background, 0, 0, Background.getWidth(), Background.getHeight(), 0, 0, Background.getWidth(), Background.getHeight());
    //g.drawImage(Dial, 61.59, 800.41, Dial.getWidth(), Dial.getHeight(), 0,0, Dial.getWidth(), Dial.getHeight());
    //DRAWING ASSETS ON SCREEN

    auto bounds = getLocalBounds().toFloat();
    
    g.setColour(juce::Colour::fromRGB(173, 29, 29));
    juce::Path path;

    const float centerX = 409.11;
    const float centerY = 680.81;

    const int numSamples = mEnvelopeBuffer.getNumSamples();

    if (numSamples > 0) {
        const int numPoints = std::min(100, numSamples); // Adjust the number of points for smoother circles

        const float minX = 200.0f; // Adjust as needed
        const float minY = 200.0f; // Adjust as needed
        const float maxX = 833.0f; // Adjust as needed
        const float maxY = 833.0f; // Adjust as needed

        for (int i = 0; i < numPoints; i++) {
            const float angle = juce::MathConstants<float>::twoPi * static_cast<float>(i) / numPoints;
            float radius = (getHeight() * 0.35) * mEnvelopeBuffer.getSample(0, i % numSamples);

            // Ensure the radius fits within the bounds
            radius = std::min(radius, std::min(centerX - minX, maxX - centerX));
            radius = std::min(radius, std::min(centerY - minY, maxY - centerY));

            const float x = centerX + radius * std::cos(angle);
            const float y = centerY + radius * std::sin(angle);

            if (i == 0) {
                path.startNewSubPath(x, y);
            }
            else {
                path.lineTo(x, y);
            }
        }
    }
    // Use numSamples directly to scale the stroke thickness
    g.strokePath(path, juce::PathStrokeType(2.0f * static_cast<float>(numSamples) / 50.0f));



}



void FilterTripAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.15;
    auto sliderSize = getWidth() * 0.15;
    auto sliderYpos = topMargin * 4.75;

    userAttackSlider.setBounds(leftMargin *1.25, topMargin * 4.425, 106, 106);
    userReleaseSlider.setBounds(leftMargin * 1.25, topMargin * 5.275, 106, 106);

    gainSlider.setBounds(leftMargin * 10.75, topMargin * 3.01, 25, 166);
    userCutoffSlider.setBounds(leftMargin * 33.8, topMargin * 1.675, 166, 25);
    envelopePercentageSlider.setBounds(leftMargin * 3.85, topMargin * 3, 25, 166);

    filterModelCombo.setBounds(leftMargin * 36.65, topMargin * 1.085, 114, 48);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
