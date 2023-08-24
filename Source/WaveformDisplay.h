/*
  ==============================================================================

    WaveformDisplay.h
    Created: 2 Mar 2023 11:36:55pm
    Author:  tls

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
  WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
  ~WaveformDisplay() override;

  /** necessary functions to override from parent abstract class*/
  void paint (juce::Graphics&) override;
  void resized() override;
  void changeListenerCallback(juce::ChangeBroadcaster *source) override;

  /** function to feed audioThumbnail audioFile*/
  void loadURL(juce::URL audioURL);

  /** set the relative position of the play head*/
  void setPositionRelative(double pos);
  /** set the startPos to draw line*/
  void setStartPos (double pos);
  /** set the endPos to draw line*/
  void setEndPos (double pos);
  /** set the showStartPos to hide or show start line*/
  void setShowStartPos (bool showStartPos);
  /** set the showEndPos to hide or show end line*/
  void setShowEndPos (bool showEndPos);
  
private:
  double startPos = 0;
  bool showStartPos{false};
  double endPos = 1;
  bool showEndPos{false};
  juce::AudioThumbnail audioThumbnail;
  bool fileLoaded;
  double position;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
