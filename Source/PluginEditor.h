/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FretboardQuizAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           private juce::Timer
{
public:
    FretboardQuizAudioProcessorEditor (FretboardQuizAudioProcessor&);
    ~FretboardQuizAudioProcessorEditor() override { };

    //==============================================================================
    void paint (juce::Graphics&) override;

    void resized () override       { }
    void timerCallback () override { repaint(); }


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FretboardQuizAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FretboardQuizAudioProcessorEditor)
};
