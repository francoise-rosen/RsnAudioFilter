#include "MainComponent.h"


std::vector<std::string> MainComponent::operators {"+", "-", "*", "/", "sqrt", "cos", "sin", "tan", "=", "C"};
//==============================================================================
MainComponent::MainComponent()
:textChanged{false}
{
    stream.reset();
    std::cout << "constructor. text changed: " << textChanged << '\n';
    makeOperators();
    makeOpVisible();
    textEntryScreen.setText(juce::String(0));
    addAndMakeVisible(&textEntryScreen);
    textEntryScreen.addListener(this);
    setSize (250, 400);
}


MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
   // g.drawText ("CALCULATOR", getLocalBounds(), juce::Justification::centred, true);

}

void MainComponent::resized()
{
    int edge = 10;
    auto buttonsArea = getLocalBounds().removeFromBottom(getHeight() * 3/4);
    auto binaryOpArea = buttonsArea.removeFromTop(buttonsArea.getHeight()/3).reduced(edge);
    auto unaryOpArea = buttonsArea.removeFromTop(buttonsArea.getHeight()/2).reduced(edge);
    auto bottomArea = buttonsArea.reduced(edge);
    
    for (int i = 0; i < 4; ++i)
    {
        arithmetic[i]->setBounds(binaryOpArea.removeFromLeft((getWidth()-2*edge)/4));
    }
    
    for (int j = 4; j < 8; ++j)
    {
        arithmetic[j]->setBounds(unaryOpArea.removeFromLeft((getWidth()-2*edge)/4));
    }
    
    arithmetic[flush]->setBounds(bottomArea.removeFromRight(bottomArea.getWidth()/3));
    arithmetic[equals]->setBounds(bottomArea);
    
    
    textEntryScreen.setBounds(getLocalBounds().removeFromTop(getHeight()/4).reduced(edge));
}

void MainComponent::makeOperators()
{
    assert (operators.size() == numOfOperators);
    for (int i = 0; i < numOfOperators; ++i)
    {
        arithmetic.add(new juce::TextButton(operators[i], operators[i]));
    }
}

void MainComponent::makeOpVisible()
{
    for (auto p : arithmetic)
    {
        addAndMakeVisible(p);
        p->addListener(this);
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    double value = textEntryScreen.getText().getDoubleValue();
    if (button == arithmetic[plus])
    {
        std::cout << "plus. text changed: " << textChanged << '\n';
        stream.updateState(value, plus, textChanged, true);
        textEntryScreen.setText(juce::String(stream.getValueFromBuffer()));
        textChanged = false;
    }
    
    else if (button == arithmetic[minus])
    {
        std::cout << "minus. text changed: " << textChanged << '\n';
        stream.updateState(value, Operation::minus, textChanged, true);
        textEntryScreen.setText(juce::String(stream.getValueFromBuffer()));
        textChanged = false;
    }
    
    else if (button == arithmetic[multiply])
    {
        stream.updateState(value, Operation::multiply, textChanged, true);
        textEntryScreen.setText(juce::String(stream.getValueFromBuffer()));
        textChanged = false;
    }
    
    else if (button == arithmetic[divide])
    {
        stream.updateState(value, Operation::divide, textChanged, true);
        textEntryScreen.setText(juce::String(stream.getValueFromBuffer()));
        textChanged = false;
    }
    
    else if (button == arithmetic[equals])
    {
        // stream takes care of updating its buffers
        std::cout << "equals. text changed: " << textChanged << '\n';
        // is there the streams buffer full, is there an op?
        // what if user entered 16 sqrt =
        double valueToDisplay = stream.compute(textEntryScreen.getText().getDoubleValue());
        
        // in case of equals keep the previous operator in buffer
        stream.updateState(valueToDisplay, equals, textChanged, false);
        textEntryScreen.setText(juce::String(valueToDisplay), false);
        textChanged = false;
    
        //std::cout << "Text Changed: " << textChanged << '\n';
        
//        if (textChanged)
//        {
//            double value = stream.compute(textEntryScreen.getText().getDoubleValue());
//            textEntryScreen.setText(juce::String(value));
//        }
//
//        else if (!textChanged)
//        {
//            std::cout << "here\n";
//
//            textEntryScreen.setText(juce::String(stream.getValueFromBuffer()));
//        }
        
    }
    
    else if (button == arithmetic[flush])
    {
        stream.reset();
        textEntryScreen.setText(juce::String(0), false);
        textChanged = false;
        std::cout << "flashed\n";
    }
}

// do I need this at all?
void MainComponent::textEditorTextChanged(juce::TextEditor & text)
{
    if(&text == &textEntryScreen)
    {
        
        std::cout << "the state before change: " << textChanged << '\n';
        textChanged = true;
        std::cout << "Changing the state to: " << textChanged << '\n';
    }
}
