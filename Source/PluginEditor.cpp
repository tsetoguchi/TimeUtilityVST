/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
CurrentlyAudioProcessorEditor::CurrentlyAudioProcessorEditor(
    CurrentlyAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {

    setInterceptsMouseClicks(true, true);
    setMouseClickGrabsKeyboardFocus(false);

    const juce::Displays::Display *screen =
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();

    float baseFontSize = 20.f;
    scaledFontSize = baseFontSize;
    int baseWidth = 400;
    int baseHeight = 300;

    setOpaque(false);

    if (screen != nullptr) {
        auto resolution = screen->totalArea;
        int width = resolution.getWidth();
        int height = resolution.getHeight();

        baseWidth = width * 0.15;
        baseHeight = height * 0.10;

        // Create a scaling factor based on resolution
        // Reference resolution (standard 1080p)
        const int referenceWidth = 1920;
        const int referenceHeight = 1080;

        // Calculate scaling - higher resolution = larger font
        float widthScale = (float)width / referenceWidth;
        float heightScale = (float)height / referenceHeight;

        // Use the smaller scaling factor to ensure it fits on both dimensions
        // or average them if you prefer
        float scaleFactor = ((widthScale + heightScale) / 2.0f) * 3;
        scaledFontSize = juce::jmin(baseFontSize * scaleFactor, 1000.0f);
    }

    addAndMakeVisible(timeLabel);
    timeLabel.setFont(juce::FontOptions(scaledFontSize).withStyle("bold"));
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setText("Time: 0.0s", juce::dontSendNotification);

    // Add the credit label here
    //    addAndMakeVisible(creditLabel);
    //    creditLabel.setFont(juce::FontOptions(scaledFontSize /
    //    5).withStyle("plain"));
    //    creditLabel.setColour(juce::Label::textColourId,
    //    juce::Colour::fromRGB(184, 184, 184));
    //
    //    creditLabel.setJustificationType(juce::Justification::centredRight);
    //    creditLabel.setText("by Konac", juce::dontSendNotification);

    // Ensure labels don't intercept mouse clicks
    timeLabel.setInterceptsMouseClicks(false, false);
    //    creditLabel.setInterceptsMouseClicks(false, false);

    startTimerHz(240);

    setName("CURRENTLY |   by Konac");

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(baseWidth, baseHeight);
}

CurrentlyAudioProcessorEditor::~CurrentlyAudioProcessorEditor() { stopTimer(); }

//==============================================================================
void CurrentlyAudioProcessorEditor::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll(juce::Colour::fromRGB(48, 48, 48));

    //    g.fillAll (juce::Colours::transparentBlack);

    //    g.setColour (juce::Colours::white);
    //    g.setFont (juce::FontOptions (15.0f));
    //    g.drawFittedText ("Hello World!", getLocalBounds(),
    //    juce::Justification::centred, 1);
}

void CurrentlyAudioProcessorEditor::resized() {

    // Calculate the label's height based on the scaled font size
    int labelHeight =
        scaledFontSize *
        1.5; // Adjust multiplier to suit your font's vertical spacing

    // Calculate the vertical offset to center the label correctly
    int yOffset = getHeight() / 2 - labelHeight / 2;

    // Position the time label with dynamic height and centered vertically
    timeLabel.setBounds(getWidth() / 2 - 100, yOffset, 200, labelHeight);

    // Position the credit label at the bottom-right corner
    //    creditLabel.setBounds(getWidth() - 200, yOffset + 75, 190, 20);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

// Mouse down event handler
void CurrentlyAudioProcessorEditor::mouseDown(const juce::MouseEvent &event) {

    isSecondUIActive = !isSecondUIActive;

    animator.cancelAllAnimations(false);

    // First fade out
    timeLabel.setAlpha(1.0f);
    animator.fadeOut(&timeLabel, 200);

    juce::Timer::callAfterDelay(200, [this]() {
        if (isSecondUIActive) {

            timeLabel.setFont(
                juce::FontOptions(scaledFontSize * 0.75).withStyle("bold"));
            timeLabel.setColour(juce::Label::textColourId,
                                juce::Colours::white);
            timeLabel.setJustificationType(juce::Justification::centred);
            timeLabel.setText("by Konac", juce::dontSendNotification);
            isTimerActive = false;

        }

        else {
            isTimerActive = true;
        }

        timeLabel.repaint();
        timeLabel.setAlpha(0.0f);
        animator.fadeIn(&timeLabel, 200);

        repaint();
    });
}

void CurrentlyAudioProcessorEditor::mouseEnter(const juce::MouseEvent &event) {}

void CurrentlyAudioProcessorEditor::timerCallback() {

    if (isTimerActive) {

        // Gets current time in seconds
        double currentTime = audioProcessor.getCurrentTimeInSeconds();

        // Format the time in MM:SS.ms
        int minutes = static_cast<int>(currentTime) / 60;
        int seconds = static_cast<int>(currentTime) % 60;
        int milliseconds =
            static_cast<int>((currentTime - std::floor(currentTime)) * 100);

        // Update the label text
        timeLabel.setText(juce::String::formatted("%02d:%02d.%02d", minutes,
                                                  seconds, milliseconds),
                          juce::dontSendNotification);
    }
}
