/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// implement this in wavesher as a static array of strings?
std::vector<String> RaveshaperAudioProcessorEditor::functions{"sfdtanh", "sfdcos", "soft clipper", "clipper cascade", "sfdsine"};

//==============================================================================
RaveshaperAudioProcessorEditor::RaveshaperAudioProcessorEditor (RaveshaperAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(&gain);
    addAndMakeVisible(&saturate);
    addAndMakeVisible(&crossfade);
    addAndMakeVisible(&symmetry);
    addAndMakeVisible(&function_A);
    addAndMakeVisible(&function_B);
    fillCombos();
    
    setSize (400, 300);
    
    // tree state
    gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramGain, gain));
    
    saturationAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramSaturate, saturate));
    
    symmentryAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramSymmetry, symmetry));
    
    crossfadeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramCrossfade, crossfade));
    
    functionA_attachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramFunctionsA, function_A));
    
    functionB_attachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.getProcessorValueTreeState(), RaveshaperAudioProcessor::paramFunctionsB, function_B));
    
    
}

RaveshaperAudioProcessorEditor::~RaveshaperAudioProcessorEditor()
{
}

//==============================================================================
void RaveshaperAudioProcessorEditor::fillCombos()
{
    for(int i = 1; i < functions.size() + 1; ++i)
    {
        function_A.addItem(functions[i], i);
        function_B.addItem(functions[i], i);
    }
}

//==============================================================================
void RaveshaperAudioProcessorEditor::paint (Graphics& g)
{
    
    g.fillAll (Colours::darkcyan);
    
    g.setColour (Colours::white);
    g.setFont (21.0f);
    
    auto area = getLocalBounds().reduced(20);
    area = area.withTop(area.getBottom() - 40);
    
    // draw fitted text here
}

void RaveshaperAudioProcessorEditor::resized()
{
    
}
