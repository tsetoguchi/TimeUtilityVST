/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <string>

using namespace std;

//==============================================================================
/**
 */
class CurrentlyAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      public juce::Timer // inheriting Timer
{

  public:
    CurrentlyAudioProcessorEditor(CurrentlyAudioProcessor &);
    ~CurrentlyAudioProcessorEditor() override;

    // Mouse down event handler
    void mouseDown(const juce::MouseEvent &event) override;

    void mouseEnter(const juce::MouseEvent &event) override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    // Timer callback
    void timerCallback() override;

  private:
    
    // Titles
    const string windowName = "CURRENTLY |   by Konac";
    const string creditsText = "by Konac";
    
    // Display label for time
    juce::Label timeLabel;

    // Activity trackers
    bool isTimerActive = true;
    bool isSecondUIActive = false;

    // Animator
    juce::ComponentAnimator animator;
    const int fadeDurationMs = 200;
    
    // Colors
    juce::Colour bgColour = juce::Colour::fromRGB(48, 48, 48);

    // Display label for credit
    juce::Label creditLabel;
    
    // Fonts
    double baseFontSize = 20;
    double scaledFontSize = baseFontSize;
    const double maxFontSize = 1000;
    
    const double creditFontSizeScale = 0.75;
    
    // UI resolution
    int baseWidth = 400;
    int baseHeight = 300;
    // Create a scaling factor based on resolution
    // Reference resolution (standard 1080p)
    const int referenceWidth = 1920;
    const int referenceHeight = 1080;
    
    // Screen to UI scaling percentages
    const double screenToUIScaleWidth = 0.15;
    const double screenToUIScaleHeight = 0.10;
    
    // Refresh rate
    const int refreshRate = 300;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CurrentlyAudioProcessor &audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurrentlyAudioProcessorEditor)
};
