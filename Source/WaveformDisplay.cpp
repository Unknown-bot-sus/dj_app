/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 2 Mar 2023 11:36:55pm
    Author:  tls

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse)
  : audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
  /* This demo code just fills the component's background and
      draws some placeholder text to get you started.

      You should replace everything in this method with your own
      drawing code..
  */

  g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

  g.setColour (juce::Colours::grey);
  g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
  g.setColour (juce::Colours::orange);


  
  if (fileLoaded) 
  {
    juce::Rectangle<int> thumbnailBounds (0, 0, getWidth() * position, getHeight() );
    juce::Rectangle<int> thumbnailBounds2 (getWidth() * position, 0, getWidth() - (getWidth() * position), getHeight());
    double totalLength = audioThumbnail.getTotalLength();
    audioThumbnail.drawChannel(g, thumbnailBounds, 0, totalLength * position, 0, 1.0f);
    g.setColour(juce::Colours::grey);
    audioThumbnail.drawChannel(g, thumbnailBounds2, totalLength * position, audioThumbnail.getTotalLength(), 0, 1.0f);
    g.setColour(juce::Colours::lightgreen);
    g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());

    if (showStartPos)
    {
      g.setColour(juce::Colours::green);
      juce::Line<float> startLinePos (getWidth() * startPos, 0, getWidth() * startPos, getHeight());
      g.drawLine(startLinePos);
    }

    if (showEndPos)
    {
      g.setColour(juce::Colours::red);
      juce::Line<float> endLinePos (getWidth() * endPos, 0, getWidth() * endPos, getHeight());
      g.drawLine(endLinePos);
    }
  } else 
  {
    g.setFont (14.0f);
    g.drawText ("File not loaded...", getLocalBounds(),
    juce::Justification::centred, true);   // draw some placeholder text
  }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
  audioThumbnail.clear();
  fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
  repaint();
};

void WaveformDisplay::setPositionRelative(double pos) {
  if (position != pos) {
    position = pos;
    repaint();
  }
};

void WaveformDisplay::setStartPos (double pos)
{
  startPos = pos;
};

void WaveformDisplay::setEndPos (double pos)
{
  endPos = pos;
}

void WaveformDisplay::setShowStartPos (bool showPos)
{
  showStartPos = showPos;
}

void WaveformDisplay::setShowEndPos (bool showPos)
{
  showEndPos = showPos;
}