/*
  ==============================================================================

    Gain.cpp
    Created: 25 Jul 2022 12:04:33pm
    Author:  tatao

  ==============================================================================
*/

#include "Gain.h"

template <typename SampleType>
Gain<SampleType>::Gain() : currentSampleRate(44100.0f), naturalGain(juce::Decibels::decibelsToGain(0.0f))
{

}

template <typename SampleType>
void Gain<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    naturalGain.reset(currentSampleRate, 0.02);
}

template <typename SampleType>
void Gain<SampleType>::reset()
{
    if (currentSampleRate > 0)
    {
        naturalGain.reset(currentSampleRate, 0.02);
    }
}

template <typename SampleType>
void Gain<SampleType>::setGainFromDecibels(SampleType gainInDB)
{
    naturalGain.setTargetValue(juce::Decibels::decibelsToGain(gainInDB));
}

template class Gain<float>;
template class Gain<double>;