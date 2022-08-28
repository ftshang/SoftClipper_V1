/*
  ==============================================================================

    Gain.h
    Created: 25 Jul 2022 12:04:33pm
    Author:  tatao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class Gain
{
public:
    Gain();

    void processBlock(juce::dsp::AudioBlock<float>& audioBlock)
    {
        for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
        {
            auto* channelData = audioBlock.getChannelPointer(channel);
            for (int sample = 0; sample < audioBlock.getNumSamples(); ++sample)
            {
                channelData[sample] = channelData[sample] * naturalGain.getNextValue();
            }
        }
    }

    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        auto&& inBlock = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert(inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert(inBlock.getNumSamples() == outBlock.getNumSamples());

        auto numChannels = inBlock.getNumChannels();
        auto numSamples = inBlock.getNumSamples();

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputData = inBlock.getChannelPointer(channel);
            auto* outputData = outBlock.getChannelPointer(channel);

            for (size_t sample = 0; sample < numSamples; ++sample)
            {
                outputData[sample] = processSample(inputData[sample]);
            }
        }
    }

    SampleType processSample(SampleType currentSample)
    {
        return currentSample * naturalGain.getNextValue();
    }

    void prepare(const juce::dsp::ProcessSpec& spec);

    void reset();

    void setGainFromDecibels(SampleType gainInDB);

private:
    float currentSampleRate;
    juce::SmoothedValue<float> naturalGain;
};