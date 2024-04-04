/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel/DialLAF.h"
#include "LookAndFeel/SliderLAF.h"
#include "LookAndFeel/ButtonLAF.h"
#include "LookAndFeel/LabelLAF.h"


//==============================================================================
/**
*/
class FilterTripAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    FilterTripAudioProcessorEditor (FilterTripAudioProcessor&);
    ~FilterTripAudioProcessorEditor() override;

    enum RadioButtonIds
    {
        FilterButtons = 1001
    };

    void setEnvelope(const juce::AudioBuffer<float>& envelopeBuffer) {
        //investigate standard swap (std::swap)
        mEnvelopeBuffer = envelopeBuffer;
    }

    void timerCallback() override {
        mEnvelopeBuffer = audioProcessor.getEnvelopeBuffer();
        //DBG(juce::String (mEnvelopeBuffer.getRMSLevel(0,0,mEnvelopeBuffer.getNumSamples())));
        repaint();
        
    };
    
    void updateToggleState(juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";

        juce::Logger::outputDebugString(name + " Button changed to " + stateString);

        if (button == &lowpassButton && state)
            audioProcessor.setFilterModel(0);
        else if (button == &bandpassButton && state)
            audioProcessor.setFilterModel(1);
        else if (button == &highpassButton && state)
            audioProcessor.setFilterModel(2);
    }



    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterTripAudioProcessor& audioProcessor;
    juce::AudioBuffer<float> mEnvelopeBuffer;

   


    juce::Image Background;
    juce::Image Dial;
    juce::Image SliderDB;
    juce::Image SliderHZ;
   

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

    juce::Label cutoffValueLabel;


    juce::ComboBox filterModelCombo;

    juce::ToggleButton lowpassButton{ "Lowpass" },
                       bandpassButton{ "Bandpass" },
                       highpassButton{ "Highpass" };

  
    juce::AudioProcessorValueTreeState::SliderAttachment wetDryAttachtment, outputAttachtment, gainAttachtment, userCutoffAttachtment, envelopePercentageAttachtment, userReleaseAttachtment, userAttackAttachtment;
    //juce::AudioProcessorValueTreeState::ComboBoxAttachment filterModelAttachtment;
    //juce::AudioProcessorValueTreeState::ButtonAttachment lowpassFilterModelAttachtment, bandpassFilterModelAttachtment, highpassFilterModelAttachtment;
    

    CustomDial customDialLAF;
    CustomSlider customSliderLAF;
    CustomButton customButtonLAF;

    CustomLabel customLabelLAF;




    //juce::StringArray filterModels = { "Lowpass","Highpass","Bandpass" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTripAudioProcessorEditor)
};
