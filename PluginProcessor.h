/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Parameters.h"

//==============================================================================
/**
*/
class FilterTripAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    FilterTripAudioProcessor();
    ~FilterTripAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void setFilterModel(int newModel);

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioBuffer<float> getEnvelopeBuffer() {
        return envelopeBuffer;
    };

    juce::AudioProcessorValueTreeState _treeState;

    juce::StringArray filterModels = { "Lowpass", "Highpass", "Bandpass" };


private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    float attack_in_ms = 10.0f;
    float release_in_ms = 50.0f;
    double samplerate;
    float attack_coef;
    float release_coef;
    float envelope = 0.0f;

    int currentFilterModel;

    juce::dsp::LadderFilter<float> _lowpassFilter;
    juce::dsp::LadderFilter<float> _highpassFilter;
    juce::dsp::LadderFilter<float> _bandpassFilter;

    juce::AudioBuffer<float> envelopeBuffer;
    juce::AudioParameterFloat* envelopeParam;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterTripAudioProcessor)
};
