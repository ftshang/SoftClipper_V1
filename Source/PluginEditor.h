/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./LookAndFeel/Dial.h"

//==============================================================================
/**
*/
class SoftClipper_V1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SoftClipper_V1AudioProcessorEditor (SoftClipper_V1AudioProcessor&);
    ~SoftClipper_V1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoftClipper_V1AudioProcessor& audioProcessor;

    juce::Slider driveDial;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveDialAttachment;

    juce::Slider outputDial;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputDialAttachment;

    juce::DropShadow shadowProperties;
    juce::DropShadowEffect labelShadow;

    juce::Label driveLabel;
    juce::Label outputLabel;

    CustomDial customDialLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftClipper_V1AudioProcessorEditor)
};
