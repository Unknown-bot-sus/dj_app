/*
  ==============================================================================

    DeckLoader.h
    Created: 13 Mar 2023 12:56:32pm
    Author:  tls

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DeckGUI.h"

class DeckLoader {
  public:
    /** Function to load file to decks*/
    virtual void loadFileToDeck(juce::File& audioFile, unsigned int deckID) = 0;
};