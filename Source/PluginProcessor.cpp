/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"

//==============================================================================
FretboardQuizAudioProcessor::FretboardQuizAudioProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",     juce::AudioChannelSet::mono())),
          forwardFFT(fftOrder),
          window (fftSize, juce::dsp::WindowingFunction<float>::blackman),
          rng()
{
    addParameter (threshold = new juce::AudioParameterFloat ("threshold", "Threshold", 0.0f, 30.0f, 15.0f));
}

bool FretboardQuizAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const 
{
    // the sidechain can take any layout, the main bus needs to be the same on the input and output
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                && ! layouts.getMainInputChannelSet().isDisabled();
}

//==============================================================================
void FretboardQuizAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_currentTarget = generateNote();
}

void FretboardQuizAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (buffer.getNumChannels() > 0)
    {
        // for now just assume first channel
        for ( auto j=0; j < buffer.getNumSamples(); ++j)
        {
            //juce::String s = juce::String::formatted(
                    //"channel 0: {%f}",
                    //buffer.getReadPointer(0)[j]);
            //DBG(s);

            pushNextSampleIntoFifo(buffer.getReadPointer(0)[j]);

            if (nextFFTBlockReady)
            {
                checkPitch ();
                nextFFTBlockReady = false;
            }
        }

    }
}

void FretboardQuizAudioProcessor::pushNextSampleIntoFifo (const float& sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that we have enough samples to run the FFT
    if (fifoIndex == fftSize)               
    {
        if (! nextFFTBlockReady)            
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;             
}

void FretboardQuizAudioProcessor::checkPitch ()
{
    const float freq = estimateFrequency ();
    const int note = Utils::midiNoteFromFreq (freq);
    const juce::String notename = juce::MidiMessage::getMidiNoteName (note, true, false, 3);
    m_currentNote = notename;

    if (m_currentNote.equalsIgnoreCase (m_currentTarget)) 
    {
        m_currentTarget = generateNote ();
    }
}

inline juce::String FretboardQuizAudioProcessor::generateNote() 
{ 
    return juce::MidiMessage::getMidiNoteName (rng.nextInt(128), true, false, 3); 
}

float FretboardQuizAudioProcessor::estimateFrequency ()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable (fftData, fftSize);       

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);  


    float maxAmp = -1.0; int maxIdx = -1;
    for (int i = 0; i < fftSize; ++i)                         
    {
        const auto level = abs(fftData[i]);
        if (level > maxAmp) 
        {
            maxAmp = level;
            maxIdx = i;
        }
    }

    // hand-tuned threshold
    if (maxAmp < threshold->get())
        return 0.0f;

    return Utils::freqFromIndex(maxIdx, fftSize, getSampleRate());
}

juce::AudioProcessorEditor* FretboardQuizAudioProcessor::createEditor()
{
    return new FretboardQuizAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FretboardQuizAudioProcessor();
}
