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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

FretboardQuizAudioProcessorEditor::~FretboardQuizAudioProcessorEditor()
{
}

//==============================================================================
void FretboardQuizAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (64.0f);

    g.drawFittedText (audioProcessor.getCurrentTarget()+ "   |   " + audioProcessor.getCurrentNote(), getLocalBounds(), juce::Justification::centred, 1);
    //g.drawFittedText (audioProcessor.getCurrentTarget(), getLocalBounds(), juce::Justification::centredLeft, 1);
    //g.drawFittedText (audioProcessor.getCurrentNote(), getLocalBounds(), juce::Justification::centredRight, 1);
}

void FretboardQuizAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
