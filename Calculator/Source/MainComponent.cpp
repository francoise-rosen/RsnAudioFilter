#include "MainComponent.h"


std::vector<std::string> MainComponent::operators {"+", "-", "*", "/", "sqrt", "cos", "sin", "tan", "=", "C"};
//==============================================================================
MainComponent::MainComponent()
:textEntered{false}
{
    stream.reset();
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
    double valueOnScreen = textEntryScreen.getText().getDoubleValue();
    if (button == arithmetic[plus])
    {
        if (!textEntered)
        {
            stream.updateState(valueOnScreen, plus, false, false);
            return;
        }
        //double valueToDisplay = (stream.isFull()) ? stream.compute(valueOnScreen) : valueOnScreen;
        double valueToDisplay = stream.compute(valueOnScreen);
        stream.updateState(valueToDisplay, plus, textEntered, true);
        textEntryScreen.setText(juce::String(valueToDisplay), false);
        textEntered = false;
    }
    
    else if (button == arithmetic[minus])
    {
        if (!textEntered)
        {
            stream.updateState(valueOnScreen, minus, false, false);
            return;
        }
        double valueToDisplay = stream.compute(valueOnScreen);
        stream.updateState(valueToDisplay, Operation::minus, textEntered, true);
        textEntryScreen.setText(juce::String(valueToDisplay), false);
        textEntered = false;
    
    }
    
    else if (button == arithmetic[multiply])
    {
        if (!textEntered)
        {
            stream.updateState(valueOnScreen, multiply, false, false);
            return;
        }
        
        double valueToDisplay = stream.compute(valueOnScreen);
        stream.updateState(valueToDisplay, multiply, true, true);
        textEntryScreen.setText(juce::String(valueToDisplay), false);
        textEntered = false;

    }
    
    else if (button == arithmetic[divide])
    {
        if (!textEntered)
        {
            stream.updateState(valueOnScreen, divide, false, false);
            return;
        }
        if (valueOnScreen == 0)
        {
            textEntryScreen.setText(juce::String("NAN"));
            stream.reset();
            return;
        }
        double valueToDisplay = stream.compute(valueOnScreen);
        stream.updateState(valueToDisplay, divide, true, true);
        textEntryScreen.setText(juce::String(valueToDisplay), false);
        textEntered = false;

    }
    
    else if (button == arithmetic[equals])
    {
   
        double valueToDisplay = stream.compute(equals, valueOnScreen);
        
        // in case of equals keep the previous operator in buffer
        bool opOverride = (stream.isFull()) ? false : true;
        std::cout << "stream is full: " << std::boolalpha << stream.isFull() << '\n';
        stream.updateState(valueOnScreen, equals, true, opOverride);
        textEntryScreen.setText(juce::String(valueToDisplay), true);
        textEntered = true;

    }
    
    else if (button == arithmetic[flush])
    {
        stream.reset();
        textEntryScreen.setText(juce::String(0), false);
        textEntered = false;

    }
}

// do I need this at all?
void MainComponent::textEditorTextChanged(juce::TextEditor & text)
{
    if(&text == &textEntryScreen)
    {
        std::cout << "textEntered " << std::boolalpha << textEntered << '\n'; ;
        textEntered = true;
    }
}
