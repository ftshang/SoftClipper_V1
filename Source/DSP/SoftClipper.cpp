/*
  ==============================================================================

    SoftClipper.cpp
    Created: 25 Jul 2022 12:04:45pm
    Author:  tatao

  ==============================================================================
*/

#include "SoftClipper.h"

template <typename SampleType>
SoftClipper<SampleType>::SoftClipper() : currentSampleRate(44100.0f), driveNaturalGain(juce::Decibels::decibelsToGain(0.0f))
{

}

template <typename SampleType>
void SoftClipper<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    driveNaturalGain.reset(currentSampleRate, 0.02);
}

template <typename SampleType>
void SoftClipper<SampleType>::reset()
{
    if (currentSampleRate > 0)
    {
        driveNaturalGain.reset(currentSampleRate, 0.02);
    }
}

template <typename SampleType>
void SoftClipper<SampleType>::setDrive(SampleType driveDB)
{
    driveNaturalGain.setTargetValue(juce::Decibels::decibelsToGain(driveDB));
}

template class SoftClipper<float>;
template class SoftClipper<double>;