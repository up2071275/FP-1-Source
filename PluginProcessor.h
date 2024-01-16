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
class FilterTripAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FilterTripAudioProcessor();
    ~FilterTripAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;



    juce::AudioBuffer<float> getEnvelopeBuffer() {
        return envelopeBuffer;
    };

    juce::AudioProcessorValueTreeState _treeState;

    juce::StringArray filterModels = { "Lowpass","Highpass","Bandpass" };

    class filterChoiceParameter : public juce::AudioParameterChoice
    {
    public :
        filterChoiceParameter(FilterTripAudioProcessor& processor) :
            juce::AudioParameterChoice(filterModelID, filterModelName, juce::StringArray{ "Lowpass","Highpass","Bandpass" }, 1) ,
            processorAttachtedTo(processor)
        {

        }
        void valueChanged(int value) override 
        {
            switch (value)
            {
            case 0:
            {
                processorAttachtedTo._lowpassFilter.setEnabled(true);
                processorAttachtedTo._highpassFilter.setEnabled(false);
                processorAttachtedTo._bandpassFilter.setEnabled(false);

                    break;
            } 
            case 1:
            {
                processorAttachtedTo._lowpassFilter.setEnabled(false);
                processorAttachtedTo._highpassFilter.setEnabled(true);
                processorAttachtedTo._bandpassFilter.setEnabled(false);

                break;
            }
            case 2:
            {
                processorAttachtedTo._lowpassFilter.setEnabled(false);
                processorAttachtedTo._highpassFilter.setEnabled(false);
                processorAttachtedTo._bandpassFilter.setEnabled(true);

                break;
            }
            default:
                break;
            }
        };
    private:
        FilterTripAudioProcessor& processorAttachtedTo;


    };

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    //void parameterChanged(const juce::String& parameterID, float newValue) override;
    //void updateParameters();


    float attack_in_ms = 10.0f;
    float release_in_ms = 50.0f;
    double samplerate;
    float attack_coef;
    float release_coef;
    float envelope = 0.0f;
    //float tmp;


    juce::AudioParameterFloat* userGain;

    juce::dsp::LadderFilter<float> _lowpassFilter;
    juce::dsp::LadderFilter<float> _highpassFilter;
    juce::dsp::LadderFilter<float> _bandpassFilter;

    juce::AudioBuffer<float> envelopeBuffer;
    juce::AudioParameterFloat* envelopeParam;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTripAudioProcessor)
};
