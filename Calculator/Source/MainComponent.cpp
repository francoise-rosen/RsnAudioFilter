#include "MainComponent.h"


std::vector<std::string> MainComponent::operators {"+", "-", "*", "/", "sqrt", "cos", "sin", "tan", "=", "C"};
//==============================================================================
MainComponent::MainComponent()
:textEntered{false}
{
    stream.reset();
    makeOperators();
    makeOpVisible();
    textEntryScreen.setFont (juce::Font {"Monaco", 36.0f, juce::Font::bold});
    textEntryScreen.setText(juce::String(0));
    addAndMakeVisible(&textEntryScreen);
    textEntryScreen.addListener(this);
    textEntryScreen.setCaretVisible(false);
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

void MainComponent::buttonClicked (juce::Button* button)
{
    double valueOnScreen = textEntryScreen.getText().getDoubleValue();
    
    if (button == arithmetic[plus])
    {
        updateStream (&stream, plus, valueOnScreen);
    }
    
    else if (button == arithmetic[minus])
    {
        updateStream (&stream, minus, valueOnScreen);
    }
    
    else if (button == arithmetic[multiply])
    {
        updateStream (&stream, multiply, valueOnScreen);
    }
    
    else if (button == arithmetic[divide])
    {
        updateStream (&stream, divide, valueOnScreen);
    }
    
    else if (button == arithmetic[cosine])
    {
        updateStreamUn (&stream, cosine, valueOnScreen);
    }
    
    else if (button == arithmetic[sine])
    {
        updateStreamUn(&stream, sine, valueOnScreen);
    }
    
    else if (button == arithmetic[tangent])
    {
        updateStreamUn(&stream, tangent, valueOnScreen);
    }
    
    else if (button == arithmetic[squareRoot])
    {
        updateStreamUn(&stream, squareRoot, valueOnScreen);
    }
    
    else if (button == arithmetic[equals])
    {
   
        double valueToDisplay = stream.result(valueOnScreen);
    
        bool opOverride = (stream.isFull()) ? false : true;
        stream.updateBuffer(valueOnScreen, equals, true, opOverride);
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
       
        textEntered = true;
    }
}

void MainComponent::updateStream(Stream<double> *thisStream
                                 , Operation op, double valueOnScreen)
{
    if(!textEntered) return;
    
    if(!stream.isFull()) // initialise the stream
    {
        stream.storeValue(valueOnScreen);
        stream.storeOperator(op);
        stream.setFull(true);
        textEntered = false;
        return;
    }
    
    double valueToDisplay = stream.computeBinary(valueOnScreen);
    stream.updateBuffer(valueToDisplay, op, textEntered, true);
    textEntryScreen.setText(juce::String(valueToDisplay), false);
    textEntered = false;
}

void MainComponent::updateStreamUn(Stream<double> *thisStream
                                   , Operation op
                                   , double valueOnScreen)
{
    juce::String res = stream.computeUnary(valueOnScreen, op);
    textEntryScreen.setText(res, true);
    textEntered = true;
}
