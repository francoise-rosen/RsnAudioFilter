#pragma once

#include <JuceHeader.h>
#include "Stream.h"


//==============================================================================



class MainComponent  : public juce::Component,
public juce::Button::Listener,
public juce::TextEditor::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void makeOperators();
    void makeOpVisible();
    
    static std::vector<std::string> operators;
    
    void buttonClicked(juce::Button*) override;
    void textEditorTextChanged(juce::TextEditor&) override;
    
    void updateStream(Stream<double>* thisStream, Operation op, double valueOnScreen);

private:
    //==============================================================================
    juce::TextEditor textEntryScreen;
    juce::OwnedArray<juce::TextButton> arithmetic;

    
    juce::Label lastOperation;
    juce::Label result;
    
    bool textEntered;
    
    Stream<double> stream;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
