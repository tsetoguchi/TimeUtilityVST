/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CurrentlyAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer // inheriting Timer
{
    
public:
    CurrentlyAudioProcessorEditor (CurrentlyAudioProcessor&);
    ~CurrentlyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Timer callback
    void timerCallback() override;
    
private:
    
    
    // Display label for time
    juce::Label timeLabel;
    
    // Display label for credit
    juce::Label creditLabel;
    
    
    float scaledFontSize = 0;
    
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CurrentlyAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CurrentlyAudioProcessorEditor)
};
