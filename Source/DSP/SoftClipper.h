/*
  ==============================================================================

    SoftClipper.h
    Created: 25 Jul 2022 12:04:45pm
    Author:  tatao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>

template <typename SampleType>
class SoftClipper
{
public:
    SoftClipper();

    void prepare(const juce::dsp::ProcessSpec& spec);

    void processBlock(juce::dsp::AudioBlock<float>& audioBlock)
    {
        for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
        {
            auto* channelData = audioBlock.getChannelPointer(channel);
            for (int sample = 0; sample < audioBlock.getNumSamples(); ++sample)
            {
                channelData[sample] = processSample(channelData[sample]);
            }
        }
    }

    SampleType processSample(SampleType currentSample)
    {
        auto wetSignal = currentSample * driveNaturalGain.getNextValue();
        auto returnedSignal = wetSignal;

        if (wetSignal < -1)
            returnedSignal = -(2.0f / 3.0f);
        else if (wetSignal >= -1 && wetSignal <= 1)
            returnedSignal = wetSignal - (std::pow(wetSignal, 3) / 3.0f);
        else if (wetSignal > 1)
            returnedSignal = 2.0f / 3.0f;

        return returnedSignal;
    }

    void reset();
    
    void setDrive(SampleType driveDB);

private:
    float currentSampleRate;
    juce::SmoothedValue<float> driveNaturalGain;
};