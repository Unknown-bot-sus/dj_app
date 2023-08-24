/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Mar 2023 4:42:31pm
    Author:  tls

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DeckLoader.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckLoader* _root, juce::AudioFormatManager& _formatManager): root(_root), formatManager(_formatManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
  addAndMakeVisible(tableComponent);
  addAndMakeVisible(removeAllButton);
  addAndMakeVisible(searchBar);

  tableComponent.setModel(this);
  removeAllButton.addListener(this);
  searchBar.addListener(this);
  searchBar.setTextToShowWhenEmpty("Input audio file name", juce::Colours::orange);

  tableComponent.getHeader().addColumn("Filename", 1, 400);
  tableComponent.getHeader().addColumn("Duration", 2, 200);
  tableComponent.getHeader().addColumn("", 3, 200);
  tableComponent.getHeader().addColumn("", 4, 200);

  tableComponent.setModel(this);

  for (std::string& filePath: FileReader::read("temp.txt"))
  {
    audioFiles.push_back(juce::File{filePath});
  }
}

PlaylistComponent::~PlaylistComponent()
{
  std::vector<std::string> filePaths;
  for (juce::File audioFile: audioFiles)
  {
    filePaths.push_back(audioFile.getFullPathName().toStdString());
  }

  FileReader::write("temp.txt", filePaths);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  searchBar.setBounds(0, 0, getWidth(), 30);
  double columnWidth = getWidth() / 4;
  tableComponent.setBounds(0, 30, getWidth(), (getHeight() / 5 * 4));
  tableComponent.getHeader().setColumnWidth(1, columnWidth);
  tableComponent.getHeader().setColumnWidth(2, columnWidth);
  tableComponent.getHeader().setColumnWidth(3, columnWidth);
  tableComponent.getHeader().setColumnWidth(4, columnWidth);
  removeAllButton.setBounds(0,  (getHeight() / 5 * 4) + 30, getWidth(), (getHeight() / 5) - 30);
}

int PlaylistComponent::getNumRows() {
  return audioFiles.size();
}

void PlaylistComponent::paintRowBackground (juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) {
// // just highlight selected rows
  if (rowIsSelected)
  {
    g.fillAll(juce::Colours::orange);
  }
  else
  {
    g.fillAll(juce::Colours::darkgrey);
  }
}

void 	PlaylistComponent::paintCell (juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
  if (columnId == 2)
  {
    auto* reader = formatManager.createReaderFor(audioFiles[rowNumber]);
    if (reader != nullptr) // good file!
    {
      std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
      transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
      readerSource.reset (newSource.release());
      unsigned int minutes, seconds;
      seconds = transportSource.getLengthInSeconds();
      minutes = seconds / 60;
      g.drawText(
        std::to_string(minutes) + ":" + std::to_string(seconds % 60), 
        2, 0,
        width - 4, height,
        juce::Justification::centredLeft, 
        true);
    }
  } else if (columnId == 1)
  {
    g.drawText (audioFiles[rowNumber].getFileName(), 
      2, 0,
      width - 4, height,
      juce::Justification::centredLeft,
      true);
  }
}

juce::Component * PlaylistComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate) {
  if (existingComponentToUpdate == nullptr && (columnId == 3 || columnId == 4))
  {
    juce::TextButton* btn = new juce::TextButton();
    if (columnId == 3) 
    {
      btn->setButtonText("LOAD TO LEFT DECK");
    } else if (columnId == 4) 
    {
      btn->setButtonText("LOAD TO RIGHT DECK"); 
    }
    juce::String id {std::to_string(rowNumber) + "," + std::to_string(columnId)};
    btn->setComponentID(id);
    btn->onClick = [this, rowNumber, columnId] { root->loadFileToDeck(audioFiles[rowNumber], columnId - 2);};
    existingComponentToUpdate = btn;
  }
  return existingComponentToUpdate;
}

bool PlaylistComponent::isInterestedInFileDrag (const juce::StringArray &files) {
  return true;
};

void PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y) {
  std::cout <<"files Dropped" << std::endl;
  for (juce::String file : files) 
  {
    audioFiles.push_back(juce::File{file});
  }

  tableComponent.updateContent();

  std::cout << audioFiles.size() << std::endl;
};

void PlaylistComponent::buttonClicked(juce::Button* btn)
{
  std::cout << "clicked" << std::endl;
  if (btn == &removeAllButton)
  {
    audioFiles.clear();
    std::cout << audioFiles.size() << std::endl;
    tableComponent.updateContent();
  }
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor &editor)
{ 
  juce::String searchText = searchBar.getText(); 

  if (searchText == "")
  {
    return;
  }


  for (int i = 0; i < audioFiles.size(); ++i)
  { 
    if (audioFiles[i].getFileName().containsWholeWord(searchText))
    {                                             
      tableComponent.selectRow(i, false, true); 
    }
  }
}