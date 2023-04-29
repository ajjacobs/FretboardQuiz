/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
FretboardQuizAudioProcessor::FretboardQuizAudioProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",     juce::AudioChannelSet::stereo())),
          forwardFFT(fftOrder),
          window (fftSize, juce::dsp::WindowingFunction<float>::blackman)
{
}

bool FretboardQuizAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const 
{
    // the sidechain can take any layout, the main bus needs to be the same on the input and output
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                && ! layouts.getMainInputChannelSet().isDisabled();
}


FretboardQuizAudioProcessor::~FretboardQuizAudioProcessor()
{
}

//==============================================================================
//==============================================================================
void FretboardQuizAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FretboardQuizAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}


void FretboardQuizAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if (buffer.getNumChannels() > 0)
    {
        // for now just assume first channel
        for ( auto j=0; j < buffer.getNumSamples(); ++j)
        {
            pushNextSampleIntoFifo(buffer.getReadPointer(0)[j]);

            if (nextFFTBlockReady)
            {
                estimateFrequency();
                nextFFTBlockReady = false;
            }
        }

    }
}

void FretboardQuizAudioProcessor::estimateFrequency()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable (fftData, fftSize);       

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);  

    float maxAmp = fftData[0]; int maxIdx = 0;
    for (int i = 1; i < scopeSize; ++i)                         
    {
        const float dat = fftData[i];
        if (dat > maxAmp) 
        {
            maxAmp = dat;
            maxIdx = i;
       }
    }
    m_frequency = (getSampleRate() * maxIdx) / fftSize;
}


void FretboardQuizAudioProcessor::pushNextSampleIntoFifo (float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (fifoIndex == fftSize)               // [11]
    {
        if (! nextFFTBlockReady)            // [12]
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;             // [12]
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FretboardQuizAudioProcessor();
}
