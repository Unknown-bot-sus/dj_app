/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 1 Mar 2023 4:27:58pm
    Author:  tls

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer: public juce::AudioSource {
  public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    /** necessary functions to override from juce::AudioSource abstract class*/
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources () override;

    /** load url to read audioFile*/
    void loadURL(juce::URL audioURL);

    /** read audio file*/
    void loadFile (juce::File audioFile);
    /** set gain of transport source or volume*/
    void setGain (double gain);
    /** set speed of resampling source*/
    void setSpeed (double ratio);
    /** set position of the transport source*/
    void setPosition (double posInSecs);
    /** set relative position of the transport source.  A value between 0 to 1*/
    void setPositionRelative (double pos);

    /** get the relative position of the playhead */
    double getPositionRelative ();

    /** start the audio from the start*/
    void start ();
    /** stop playing the audio*/
    void stop ();
    /** resume playing the audio*/
    void resume ();

  private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};