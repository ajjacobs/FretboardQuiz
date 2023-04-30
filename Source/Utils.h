/*
  ==============================================================================

    Util.h
    Created: 30 Apr 2023 9:38:49am
    Author:  Jacobsen

  ==============================================================================
*/

#pragma once
#include <cmath>

namespace Utils{

    /// @brief 
    /// @param freq :: float
    /// @return midi note number
    inline int midiNoteFromFreq(const float& freq) {
      return round(log(freq / 440.0) / log(2) * 12 + 69);
    }
    
    /// @brief  
    /// @param index       :: index in the array
    /// @param nSamples    :: number of samples in the array
    /// @param sampleRate  :: Current sample rate
    /// @return associated frequency
    inline float freqFromIndex (const float& index, const float& nSamples, const float& sampleRate) 
    {
      // SampleRate / nSamples = samples/s * 1/samples = 1/s = Hz
      return (sampleRate * index) / nSamples;
    }
};
