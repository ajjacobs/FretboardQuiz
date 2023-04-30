/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FretboardQuizAudioProcessorEditor::FretboardQuizAudioProcessorEditor (FretboardQuizAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Doesn't need a particularly fast refresh rate
    startTimerHz (15);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

//==============================================================================
void FretboardQuizAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);

    g.setFont (128.0f);
    g.drawFittedText (audioProcessor.getCurrentTarget(), getLocalBounds(), juce::Justification::centred, 1);

    g.setFont (30.0f);
    g.drawFittedText (audioProcessor.getCurrentNote(), getLocalBounds(), juce::Justification::bottomRight, 1);
}
