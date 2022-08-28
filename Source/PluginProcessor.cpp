/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftClipper_V1AudioProcessor::SoftClipper_V1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener(driveID, this);
    treeState.addParameterListener(outputID, this);
}

SoftClipper_V1AudioProcessor::~SoftClipper_V1AudioProcessor()
{
    treeState.removeParameterListener(driveID, this);
    treeState.removeParameterListener(outputID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SoftClipper_V1AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(driveID, driveName, 0.0f, 30.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0f, 24.0f, 0.0f));

    return { params.begin(), params.end() };
}

void SoftClipper_V1AudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}

void SoftClipper_V1AudioProcessor::updateParameters()
{
    auto drive = treeState.getRawParameterValue(driveID)->load();
    auto output = treeState.getRawParameterValue(outputID)->load();

    //DBG("Drive: " << drive);
    //DBG("Output: " << output);

    driveModule.setGainFromDecibels(drive);
    outputModule.setGainFromDecibels(output);
}

//==============================================================================
const juce::String SoftClipper_V1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoftClipper_V1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoftClipper_V1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoftClipper_V1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoftClipper_V1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoftClipper_V1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoftClipper_V1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoftClipper_V1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoftClipper_V1AudioProcessor::getProgramName (int index)
{
    return {};
}

void SoftClipper_V1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SoftClipper_V1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    driveModule.prepare(spec);
    outputModule.prepare(spec);
    softClipperModule.prepare(spec);

    updateParameters();
}

void SoftClipper_V1AudioProcessor::releaseResources()
{
    driveModule.reset();
    outputModule.reset();
    softClipperModule.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoftClipper_V1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SoftClipper_V1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    driveModule.process(context);
    softClipperModule.processBlock(audioBlock);
    outputModule.process(context);
}

//==============================================================================
bool SoftClipper_V1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SoftClipper_V1AudioProcessor::createEditor()
{
    return new SoftClipper_V1AudioProcessorEditor (*this); 
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SoftClipper_V1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream(stream);
}

void SoftClipper_V1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = treeState.state.readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
    {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoftClipper_V1AudioProcessor();
}
