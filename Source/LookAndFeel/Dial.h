/*
  ==============================================================================

    Dial.h
    Created: 26 Aug 2022 7:50:23pm
    Author:  tatao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomDial : public juce::LookAndFeel_V4
{
public:
    CustomDial();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider);

private:
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
};