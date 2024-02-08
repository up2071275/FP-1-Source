/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterTripAudioProcessor::FilterTripAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    , _treeState(*this, nullptr, "PARAMETERS", createParameterLayout()),
    currentFilterModel(0)
#endif
{
    addParameter(envelopeParam = new juce::AudioParameterFloat("envelope", "Envelope", 0.0f, 1.0f, 0.0f));
    setFilterModel(0);

}

FilterTripAudioProcessor::~FilterTripAudioProcessor()
{

}


juce::AudioProcessorValueTreeState::ParameterLayout FilterTripAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    //UI INPUTS -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //juce::StringArray filterModels = { "Lowpass","Highpass","Bandpass" };
    auto pGain = std::make_unique<juce::AudioParameterFloat>(gainID, gainName, 0.0f, 24.0f, 10.0f);
    auto pMix = std::make_unique<juce::AudioParameterFloat>(mixID, mixName, 0.0f, 1.0f, 1.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0f, 24.0f, 1.0f);
    auto pUserCutoff = std::make_unique<juce::AudioParameterFloat>(userCutoffID, userCutoffName, 50, 20000, 12500);
    auto pEnvelopePercentage = std::make_unique<juce::AudioParameterFloat>(envelopePercentageID, envelopePercentageName, 0., 1., 1.);
    auto pUserAttack = std::make_unique<juce::AudioParameterFloat>(userAttackID, userAttackName, 5, 500, 10);
    auto pUserRelease = std::make_unique<juce::AudioParameterFloat>(userReleaseID, userReleaseName, 25, 500, 50);
    //auto pFilterModel = std::make_unique<FilterChoiceParameter>(*this, "filterModel", 0);

    params.push_back(std::move(pGain));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pOutput));
    params.push_back(std::move(pUserCutoff));
    params.push_back(std::move(pEnvelopePercentage));
    //params.push_back(std::move(pFilterModel));
    params.push_back(std::move(pUserAttack));
    params.push_back(std::move(pUserRelease));



    return { params.begin(), params.end() };
    //UI INPUTS -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}
/*
void FilterTripAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}

void FilterTripAudioProcessor::updateParameters()
{
    auto model = static_cast<int>(_treeState.getRawParameterValue(disModelID)->load());
    switch (model)
    {
    case 0: _distortionModule.setDistortionModel(Distortion<float>::DistortionModel::kHard); break;
    case 1: _distortionModule.setDistortionModel(Distortion<float>::DistortionModel::kSoft); break;
    case 2: _distortionModule.setDistortionModel(Distortion<float>::DistortionModel::kSaturation); break;

    }

    _distortionModule.setDrive(_treeState.getRawParameterValue(gainID)->load());
    _distortionModule.setMix(_treeState.getRawParameterValue(mixID)->load());
    _distortionModule.setOutput(_treeState.getRawParameterValue(outputID)->load());
}
*/
//==============================================================================
const juce::String FilterTripAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterTripAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterTripAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterTripAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterTripAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterTripAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterTripAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterTripAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterTripAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterTripAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterTripAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //init spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    //setting up filters
    _lowpassFilter.prepare(spec);
    _highpassFilter.prepare(spec);
    _bandpassFilter.prepare(spec);
    _lowpassFilter.setEnabled(true);
    _lowpassFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
    //setting up filters
    //_highpassFilter.prepare(spec);
    _highpassFilter.setEnabled(true);
    _highpassFilter.setMode(juce::dsp::LadderFilterMode::HPF24);
    //setting up filters
    //_bandpassFilter.prepare(spec);
    _bandpassFilter.setEnabled(true);
    _bandpassFilter.setMode(juce::dsp::LadderFilterMode::BPF24);

    // Set up the audio buffer
    envelopeBuffer.setSize(1, samplesPerBlock);
    //envelopeBuffer.setSize(2, samplesPerBlock);
    // Set up the envelope follower parameters
    samplerate = sampleRate;
    attack_coef = exp(log(0.01) / (attack_in_ms * samplerate * 0.001));
    release_coef = exp(log(0.01) / (release_in_ms * samplerate * 0.001));
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FilterTripAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterTripAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void FilterTripAudioProcessor::setFilterModel(int newModel)
{
    currentFilterModel = newModel;

    // Handle logic based on the chosen filter model
    switch (currentFilterModel)
    {
    case 0: // Lowpass
        _lowpassFilter.setEnabled(true);
        _highpassFilter.setEnabled(false);
        _bandpassFilter.setEnabled(false);
        break;
    case 1: // Bandpass
        _lowpassFilter.setEnabled(false);
        _highpassFilter.setEnabled(false);
        _bandpassFilter.setEnabled(true);
        break;
    case 2: // Highpass
        _lowpassFilter.setEnabled(false);
        _highpassFilter.setEnabled(true);
        _bandpassFilter.setEnabled(false);
        break;
        // Add more cases if you have additional filter models
    default:
        break;
    }

    // You can add more logic as needed for handling the filter model change.
}


void FilterTripAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());




    auto userAttack = _treeState.getRawParameterValue(userAttackID);
    auto userRelease = _treeState.getRawParameterValue(userReleaseID);

    attack_coef = exp(log(0.01) / (*userAttack * samplerate * 0.001));
    release_coef = exp(log(0.01) / (*userRelease * samplerate * 0.001));


    // Process the audio buffer with the envelope follower
    int numSamples = buffer.getNumSamples();
    for (int i = 0; i < numSamples; i++)
    {
        float input = buffer.getSample(0, i);

        auto gainValue = _treeState.getRawParameterValue(gainID);


        float tmp = fabs(input)* (*gainValue);

        if (tmp > envelope)
            envelope = attack_coef * (envelope - tmp) + tmp;
        else
            envelope = release_coef * (envelope - tmp) + tmp;

        envelopeBuffer.setSample(0,i,envelope);





    }

    //FILTER PROCESSING
    juce::dsp::AudioBlock<float> filterBlock{ buffer };


    auto userCutoff = _treeState.getRawParameterValue(userCutoffID);                      //THESE WILL BE USER CONTROLLED
    auto envelopePercentage = _treeState.getParameter(envelopePercentageID)->getValue();               //THESE WILL BE USER CONTROLLED RANGE OF 0 to 1 but is a percentage
    envelopePercentage *= 100;

    double cutOff = *userCutoff + (((envelope * (envelopePercentage)) * (*userCutoff)));

    double highPasscutOff = *userCutoff - (((envelope * (envelopePercentage)) * (*userCutoff)));

    cutOff = juce::jlimit(50.,20000.,cutOff);

    highPasscutOff = juce::jlimit(50., 20000., cutOff);

    _lowpassFilter.setCutoffFrequencyHz(cutOff);
    _highpassFilter.setCutoffFrequencyHz(highPasscutOff);
    _bandpassFilter.setCutoffFrequencyHz(cutOff);


    _lowpassFilter.process(juce::dsp::ProcessContextReplacing<float>(filterBlock));
    _highpassFilter.process(juce::dsp::ProcessContextReplacing<float>(filterBlock));
    _bandpassFilter.process(juce::dsp::ProcessContextReplacing<float>(filterBlock));
    //FILTER PROCESSING
    





    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
   
}

//==============================================================================
bool FilterTripAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterTripAudioProcessor::createEditor()
{
    return new FilterTripAudioProcessorEditor (*this);
}

//==============================================================================
void FilterTripAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
        auto state = _treeState.copyState();
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    

}

void FilterTripAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(_treeState.state.getType()))
            _treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterTripAudioProcessor();
}
