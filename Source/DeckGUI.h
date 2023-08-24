/*
  ==============================================================================

    DeckGUI.h
    Created: 2 Mar 2023 7:11:36pm
    Author:  tls

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : 
  public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
  DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
  ~DeckGUI() override;
  
  /** Function to deisgn DeckGUI component*/
  void paint (juce::Graphics&) override;
  /** Function that is called when window is resized*/
  void resized() override;
  
  /** Function that is called when a button is clicked*/
  void buttonClicked(juce::Button* button) override;
  /**Function that is called when a slider value is changed*/
  void sliderValueChanged(juce::Slider* slider) override;

  /** Functions that are responsible for drag and drop feature*/
  bool isInterestedInFileDrag (const juce::StringArray &files) override;
  void filesDropped (const juce::StringArray &files, int x, int y) override;

  /** This function is used to call */
  void timerCallback() override;

  /** This function is used for loading file to audioPlayers and WaveFormDisplayer*/
  void loadFile (juce::File& audioFile);
private:  
  juce::TextButton playButton{"PLAY"};
  juce::TextButton stopButton{"STOP"};
  juce::TextButton loadButton{"LOAD"};
  juce::TextButton markStartButton{"MARK START"};
  juce::TextButton markEndButton{"MARK END"};
  juce::TextButton loopButton{"LOOP"};

  bool isLoop{false};
  bool isStop{false};
  double startPos = 0;
  double endPos = 1;
  bool startMarkerEnable {false};
  bool endMarkerEnable {false};

  juce::Slider volSlider;
  juce::Label volLabel;
  juce::Slider speedSlider;
  juce::Label speedLabel;
  juce::Slider posSlider;
  juce::Label posLabel;

  juce::FileChooser fChooser{"Select a file..."};

  DJAudioPlayer* player;
  WaveformDisplay waveformDisplay;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
