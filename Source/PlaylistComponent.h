/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Mar 2023 4:42:31pm
    Author:  tls

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

#include "DJAudioPlayer.h"
#include "FileReader.h"
#include "DeckLoader.h"
//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public juce::FileDragAndDropTarget, public juce::Button::Listener, public juce::TextEditor::Listener
{
public:
  PlaylistComponent(DeckLoader* _root, juce::AudioFormatManager& _formatManager);
  ~PlaylistComponent() override;

  /** necessary functions to override from parent abstract classes*/
  void paint (juce::Graphics&) override;
  void resized() override;
  int getNumRows () override;
  void paintRowBackground (juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
  void 	paintCell (juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
  bool isInterestedInFileDrag (const juce::StringArray &files) override;
  void filesDropped (const juce::StringArray &files, int x, int y) override;
  juce::Component * 	refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate) override;
  void buttonClicked(juce::Button* btn) override;
  void textEditorTextChanged (juce::TextEditor& editor) override;

private:
  std::vector<std::string> trackTitles{"1", "2"};
  std::vector<juce::File> audioFiles;
  juce::TableListBox tableComponent;
  DeckLoader* root;
  
  juce::AudioFormatManager& formatManager;
  std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
  juce::AudioTransportSource transportSource;

  juce::TextButton removeAllButton{"Remove All"};
  juce::TextEditor searchBar;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
