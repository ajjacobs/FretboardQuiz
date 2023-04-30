/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FretboardQuizAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FretboardQuizAudioProcessor();
    ~FretboardQuizAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    //juce::AudioProcessorEditor* createEditor() override            { return new juce::GenericAudioProcessorEditor (*this); }
    juce::AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override                                { return true; }
    const juce::String getName() const override                    { return "FretboardQuiz"; }
    bool acceptsMidi() const override                              { return false; }
    bool producesMidi() const override                             { return false; }
    double getTailLengthSeconds() const override                   { return 0.0; }
    int getNumPrograms() override                                  { return 1; }
    int getCurrentProgram() override                               { return 0; }
    void setCurrentProgram (int) override                          {}
    const juce::String getProgramName (int) override               { return {}; }
    void changeProgramName (int, const juce::String&) override     {}

    bool isVST2() const noexcept                                   { return (wrapperType == wrapperType_VST); }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override        {};
    void setStateInformation (const void* data, int sizeInBytes) override  {};

    //==============================================================================
    const juce::String& getCurrentTarget() { return m_currentTarget; }
    const juce::String& getCurrentNote() { return m_currentNote; }

    // TODO: These could probably be audio parameters set in the GUI
    enum
    {
        fftOrder  = 11,            
        fftSize   = 1 << fftOrder, 
        scopeSize = 1024           
    };

private:
    // For now, we will just do a simple frequency estimation based on the 
    // fundamental frequency to get things working.
    juce::dsp::FFT forwardFFT;                      
    juce::dsp::WindowingFunction<float> window;     

    float fifo [fftSize];                           
    float fftData [2 * fftSize];                    
    int fifoIndex = 0;                              
    bool nextFFTBlockReady = false;                 
    juce::String m_currentTarget;
    juce::String m_currentNote; 
    juce::Random rng;


    juce::String generateNote();
    void pushNextSampleIntoFifo (float) noexcept;
    void checkPitch ();
    float estimateFrequency ();


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FretboardQuizAudioProcessor)
};
