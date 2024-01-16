/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FilterTripAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    FilterTripAudioProcessorEditor (FilterTripAudioProcessor&);
    ~FilterTripAudioProcessorEditor() override;

    void setEnvelope(const juce::AudioBuffer<float>& envelopeBuffer) {
        //investigate standard swap (std::swap)
        mEnvelopeBuffer = envelopeBuffer;
    }

    void timerCallback() override {
        mEnvelopeBuffer = audioProcessor.getEnvelopeBuffer();
        //DBG(juce::String (mEnvelopeBuffer.getRMSLevel(0,0,mEnvelopeBuffer.getNumSamples())));
        repaint();

    };
    



    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterTripAudioProcessor& audioProcessor;
    juce::AudioBuffer<float> mEnvelopeBuffer;

    juce::Slider wetDrySlider;
    juce::Slider outputSlider;
    juce::Slider gainSlider;
    juce::Slider userCutoffSlider;
    juce::Slider envelopePercentageSlider;
    juce::Slider userAttackSlider;
    juce::Slider userReleaseSlider;


    juce::Label wetDryLabel;
    juce::Label outputLabel;
    juce::Label gainLabel;
    juce::Label userCutoffLabel;
    juce::Label envelopePercentageLabel;
    juce::Label userAttackLabel;
    juce::Label userReleaseLabel;



    juce::ComboBox filterModelCombo;

  
    juce::AudioProcessorValueTreeState::SliderAttachment wetDryAttachtment, outputAttachtment, gainAttachtment, userCutoffAttachtment, envelopePercentageAttachtment, userReleaseAttachtment, userAttackAttachtment;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment filterModelAttachtment;

    //juce::StringArray filterModels = { "Lowpass","Highpass","Bandpass" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTripAudioProcessorEditor)
};
