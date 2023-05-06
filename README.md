# Fretboard Quiz
A simple interactive fretboard quiz programmed in JUCE.
Randomly chooses a note to display and does a rudimentary frequency pitch estimation on audio input channel 1 to check if you've played it.
Can be used as either a stand-alone application or as a VST3 plugin in 
most DAWs.

# Issues
- [ ] The plugin works in the AudioPluginHost, but the stand-alone application doesn't seem to properly pick up the input audio...

# TODO
- [x] Implement a solution to avoid false readings based on noise.
      - May 6, 2023: added elementary noise gating. 
- [ ] Implement more sophisticated noisegate
- [ ] More sophisticated pitch detection. Right now I'm just doing a rudimentary estimate of 
      the fundamental frequency using a fourier transform. That being said, it seems to work surprisingly 
      well in practice, due to guitar occupying a pretty mid frequency range.
