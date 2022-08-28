/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftClipper_V1AudioProcessorEditor::SoftClipper_V1AudioProcessorEditor (SoftClipper_V1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);

    addAndMakeVisible(driveDial);
    driveDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    driveDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 72, 36);
    driveDial.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveDial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    driveDial.setLookAndFeel(&customDialLookAndFeel);

    driveDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, driveID, driveDial);

    addAndMakeVisible(outputDial);
    outputDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outputDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 72, 36);
    outputDial.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    outputDial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    outputDial.setLookAndFeel(&customDialLookAndFeel);

    outputDialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputID, outputDial);

    shadowProperties.colour = juce::Colours::black.withAlpha(0.9f);
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int>(-1, 4);
    labelShadow.setShadowProperties(shadowProperties);

    addAndMakeVisible(driveLabel);
    driveLabel.setText("Drive", juce::NotificationType::dontSendNotification);
    driveLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setComponentEffect(&labelShadow);

    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::NotificationType::dontSendNotification);
    outputLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setComponentEffect(&labelShadow);
}

SoftClipper_V1AudioProcessorEditor::~SoftClipper_V1AudioProcessorEditor()
{
    driveDial.setLookAndFeel(nullptr);
    outputDial.setLookAndFeel(nullptr);
}

//==============================================================================
void SoftClipper_V1AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::whitesmoke.darker(0.99f));
    g.fillAll();
}

void SoftClipper_V1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto leftMargin = getWidth() * 0.06f;
    auto topMargin = getHeight() * 0.2f;
    auto dialSize = getWidth() * 0.44f;

    driveDial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    outputDial.setBounds(driveDial.getX() + driveDial.getWidth(), topMargin, dialSize, dialSize);

    driveLabel.setBounds(leftMargin, topMargin - 35, dialSize, 36);
    outputLabel.setBounds(driveLabel.getX() + driveLabel.getWidth(), driveLabel.getY(), dialSize, 36);
}
