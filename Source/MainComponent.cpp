#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerAudioSource.addInputSource(&player1, false);
    mixerAudioSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // transportSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();

    mixerAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    double rowHeight = getHeight() / 2;
    deckGUI1.setBounds(0, 0, getWidth() / 2, rowHeight);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, rowHeight);
    playlistComponent.setBounds(0, rowHeight, getWidth(), rowHeight);
}

void MainComponent::loadFileToDeck (juce::File& audioFile, unsigned int deckID)
{
    if (deckID == 1) 
    {
        deckGUI1.loadFile(audioFile);
    } else 
    {
        deckGUI2.loadFile(audioFile);
    }
}