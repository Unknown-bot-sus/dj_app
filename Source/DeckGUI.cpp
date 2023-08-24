/*
  ==============================================================================

    DeckGUI.cpp
    Created: 2 Mar 2023 7:11:36pm
    Author:  tls

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse): player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  addAndMakeVisible(playButton);
  addAndMakeVisible(stopButton);
  addAndMakeVisible(loadButton);
  addAndMakeVisible(markStartButton);
  addAndMakeVisible(markEndButton);
  addAndMakeVisible(loopButton);

  addAndMakeVisible(volSlider);
  addAndMakeVisible(volLabel);
  volLabel.setText("Volumne", juce::dontSendNotification);
  volLabel.attachToComponent(&volSlider, true);
  addAndMakeVisible(speedSlider);
  addAndMakeVisible(speedLabel);
  speedLabel.setText("Speed", juce::dontSendNotification);
  speedLabel.attachToComponent(&speedSlider, true);
  addAndMakeVisible(posSlider);
  addAndMakeVisible(posLabel);
  posLabel.setText("Position", juce::dontSendNotification);
  posLabel.attachToComponent(&posSlider, true);

  addAndMakeVisible(waveformDisplay);
  
  playButton.addListener(this);
  stopButton.addListener(this);
  loadButton.addListener(this);
  markStartButton.addListener(this);
  markEndButton.addListener(this);
  loopButton.addListener(this);

  stopButton.setClickingTogglesState(true);
  loopButton.setClickingTogglesState(true);
  markStartButton.setClickingTogglesState(true);
  markEndButton.setClickingTogglesState(true);

  volSlider.addListener(this);
  speedSlider.addListener(this);
  speedSlider.setTextValueSuffix("x");
  posSlider.addListener(this);

  volSlider.setRange(0.0, 1.0);
  speedSlider.setRange(0.5, 2.0, 0.5);
  posSlider.setRange(0.0, 1.0);

  speedSlider.setValue(1);
  volSlider.setValue(0.5);

  player->setGain(volSlider.getValue());
  player->setSpeed(speedSlider.getValue());
  player->setPositionRelative(posSlider.getValue());
    

  startTimer(500);
}

DeckGUI::~DeckGUI()
{
  stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    loopButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
    stopButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
    markStartButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
    markEndButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  double rowHeight = getHeight() / 8;
  double halfWidth = getWidth() / 2;

  playButton.setBounds(0, 0, halfWidth, rowHeight);
  stopButton.setBounds(halfWidth, 0, halfWidth, rowHeight);
  markStartButton.setBounds(0, rowHeight, halfWidth, rowHeight);
  markEndButton.setBounds(halfWidth, rowHeight, halfWidth, rowHeight);
  loadButton.setBounds(0, rowHeight * 2, halfWidth, rowHeight);
  loopButton.setBounds(halfWidth, rowHeight * 2, halfWidth, rowHeight);

  auto sliderLeft = 120;
  volSlider.setBounds(sliderLeft, rowHeight * 3, getWidth() - sliderLeft - 10, rowHeight);
  speedSlider.setBounds(sliderLeft, rowHeight * 4, getWidth() - sliderLeft - 10, rowHeight);
  posSlider.setBounds(sliderLeft, rowHeight * 5, getWidth() - sliderLeft - 10, rowHeight);
  waveformDisplay.setBounds(0, rowHeight * 6, getWidth(), rowHeight * 2);
}

void DeckGUI::buttonClicked(juce::Button* button) 
{
  if (button == &playButton) 
  {
    stopButton.setToggleState(false, juce::NotificationType::sendNotification);
    if (startMarkerEnable)
    {
      player->setPositionRelative(startPos);
      player->resume();
    } else 
    {
      player->start();
    }
  } else if (button == &stopButton) 
  {
    if (button->getToggleState())
    {
      button->setButtonText("RESUME");
      player->stop();
    } else {
      button->setButtonText("STOP");
      player->resume();
    }
  } else if (button == &loadButton)
  {
    auto fileChooserFlags =
    juce::FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
    {
        auto chosenFile = chooser.getResult();
        player->loadURL(juce::URL{chosenFile});
        waveformDisplay.loadURL(juce::URL{chosenFile});
    });
  } else if (button == &markStartButton)
  {
    if (button->getToggleState())
    {
      button->setButtonText("DISABLE START MARKER");
      startPos = player->getPositionRelative();
      startMarkerEnable = true;
      waveformDisplay.setStartPos(startPos);
    } else 
    {
      button->setButtonText("MARK START");
      startMarkerEnable = false;
    }
    waveformDisplay.setShowStartPos(startMarkerEnable);

  } else if (button == &markEndButton)
  {
    if (button->getToggleState())
    {
      button->setButtonText("DISABLE END MARKER");
      endPos = player->getPositionRelative();
      endMarkerEnable = true;
      waveformDisplay.setEndPos(endPos);
    } else 
    {
      button->setButtonText("MARK END");
      endMarkerEnable = false;
    }
    waveformDisplay.setShowEndPos(endMarkerEnable);
  }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
  if (slider == &volSlider)
  {
      player->setGain(slider->getValue());
  } else if (slider == &speedSlider) 
  {
      player->setSpeed(slider->getValue());
  } else if ( slider == &posSlider) 
  {
      player->setPositionRelative(slider->getValue());
  }
}


bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files) {
  return true;
};

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y) {
  if (files.size() == 1) 
  {
    player->loadFile(juce::File{files[0]});
  }
};

void DeckGUI::timerCallback() {
  double pos = player->getPositionRelative();
  waveformDisplay.setPositionRelative(pos);
  if (loopButton.getToggleState()) 
  {
    if (pos >= 1 || (endMarkerEnable && pos >= endPos))
    {
      if (startMarkerEnable)
      {
        player->setPositionRelative(startPos);
        player->resume();
      } else 
      {
        player->start();
      }
    }
  } else if (endMarkerEnable && pos >= endPos)
  {
    player->stop();
  }
}

void DeckGUI::loadFile (juce::File& audioFile) 
{
  player->loadFile(audioFile);
  waveformDisplay.loadURL(juce::URL{audioFile});
  if (startMarkerEnable)
  {
    markStartButton.triggerClick();
  }

  if (endMarkerEnable)
  {
    markEndButton.triggerClick();
  }

}