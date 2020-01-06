/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    setSize (800, 600);
    createSineWavetable(); // must be initialised before allocating the I/O!
    fillTables();
    Timer::startTimerHz(10); // 10 times per second
    
    // cpu labels
    cpuUsageLabel.setText("CPU usage..", dontSendNotification);
    cpuUsageLabel.setJustificationType(Justification::left);
    addAndMakeVisible(&cpuUsageLabel);
    cpuUsageText.setJustificationType(Justification::right);

    addAndMakeVisible(&cpuUsageText);
    
    // main gain slider
    mainGainLabel.setText("Gain", dontSendNotification);
    mainGainLabel.setJustificationType(Justification::centred);
    mainGainLabel.attachToComponent(&mainGain, false);
    
    mainGain.setValue(0.15);
    mainGain.setRange(0.0, 1.0);
    mainGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mainGain.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 30);
    mainGain.addListener(this);
    addAndMakeVisible(&mainGain);
    
    level = mainGain.getValue();
    

    /// I / O
    setAudioChannels (2, 2);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

// Timer

void MainComponent::timerCallback()
{
    auto cpu = deviceManager.getCpuUsage() * 100.0;
    cpuUsageText.setText(String(cpu) + "%", dontSendNotification);
    
}



// Listeners

void MainComponent::sliderValueChanged(Slider* slider)
{
    if(slider == &mainGain)
    {
        level = slider->getValue() / (double) voices;
    }
}

void MainComponent::normalise(AudioSampleBuffer& table)
{
    // check the size
    jassert(table.getNumSamples() == tableSize);
    float thisTableMagnitude = table.getMagnitude(0, 0, tableSize);
    jassert(thisTableMagnitude > 0.0); // check if samples in the buffer are legid
    
    auto targetMagnitude = (1.0f / thisTableMagnitude) * 0.99998f;
    table.applyGain(targetMagnitude);
    
    jassert(table.getMagnitude(0, 0, tableSize) > 0.0 && table.getMagnitude(0, 0, tableSize) <= 1);
}

// Sinewave Wavetable
// Saw3, Rect3, Tri3
// Saw9, Rect9, Tri9

// fill the tables and add them to array of oscillators

void MainComponent::fillTables()
   {
   createSaw9(3);
   createSaw9(5);
   createSaw9(7);
   createSaw9(9);
   createTri9(3);
   createTri9(5);
   createTri9(7);
   createTri9(9);
  // createRect9(9);
   };

void MainComponent::createSineWavetable()
{
    sineTable.setSize(1, tableSize); // channel, size
    sineTable.clear();
    // it's like elem in vector
    auto* samples = sineTable.getWritePointer(0); // just one channel in our table
    auto angleDelta = MathConstants<double>::twoPi / (double) (tableSize - 1);
    
    auto currentAngle = 0.0;
    
    for(int i = 0; i < tableSize; ++i){
        auto sample = std::sin(currentAngle);
        samples[i] = sample;
        currentAngle += angleDelta;
    }
    
    samples[tableSize] = samples[0];
    waveforms.add(&sineTable); // add sinewave to waveforms
}


void MainComponent::createSaw9(int numOfHarmonics) // numOfHarmonics, bufferToFill&, string waveform
{
    saw9Table.setSize(1, tableSize);
    saw9Table.clear();
    // difference between saw and tri/rect line1
    
    //int numOfHarmonics = 9;
    auto* samples = saw9Table.getWritePointer(0); // difference between saw and tri/rect line2
    
    std::unique_ptr<double[]> harmonics (new double[numOfHarmonics]);
    std::unique_ptr<double[]> levels (new double[numOfHarmonics]);
    
    double formantLevel = 1.0;
//    double currentLevel = formantLevel;
    for(int i = 0; i < numOfHarmonics; ++i)
    {                                              // difference between saw and tri/rect body
        harmonics[i] = i + 1;
        levels[i] = formantLevel / harmonics[i];
        
       // std::cout << levels[i] << '\n';
    }
    
    for(int harmonic = 0; harmonic < numOfHarmonics; ++harmonic)
    {
        auto angleDelta = MathConstants<double>::twoPi / (double) (tableSize - 1) * harmonics[harmonic];
        auto currentAngle = 0.0;
        // iterate over the table and sum up the samples
        
        for(int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] += (float) sample * levels[harmonic];
            currentAngle += angleDelta;
        }
        
    }
    
    samples[tableSize] = samples[0];
    
    normalise(saw9Table);
    
    waveforms.add(&saw9Table);
    
    
//    delete[] harmonics;
//    delete[] levels;

}

void MainComponent::createTri9(int numOfHarmonics=9)
{
    tri9Table.setSize(1, tableSize);              // rect/tri difference, line 1
    tri9Table.clear(); // THIS SOLVES THE NaN problem!!!!! Looks like!

    String triangleLog = "tr9 table size: ";
    Logger::getCurrentLogger()->writeToLog(triangleLog + String(tri9Table.getNumSamples()));
    
    String firstSample = "the first sample value (must be 0.0) ";
    
    
    auto* samples = tri9Table.getWritePointer(0);  // rect/tri difference, line 2


    std::unique_ptr<double[]> harmonics(new double[numOfHarmonics]);
    std::unique_ptr<double[]> harmonicAmplitudes(new double[numOfHarmonics]);
    
    //double harmonics[]{1, 3, 5, 7, 9, 11, 13, 15, 17};

    double currentHarmonic = 1.0;
    double currentAmplitude = 1.0;
    for(int i = 0; i < numOfHarmonics; ++i)
    {
        harmonics[i] = currentHarmonic;
        auto reciprocal = std::pow(currentHarmonic, 2.0);
        jassert(reciprocal != 0.0);
        harmonicAmplitudes[i] = currentAmplitude / reciprocal; // rect/tri difference, line 3
        //std::cout << "harm amp " << harmonicAmplitudes[i] << '\n';
        currentHarmonic += 2.0;

    }
    
    for(int harmonic = 0; harmonic < numOfHarmonics; ++harmonic)
    {
        // each harmonic has it's own currentAngle and angleDelta
         auto angleDelta = MathConstants<double>::twoPi / ((double)(tableSize - 1.0) * harmonics[harmonic]);
     
        //std::cout << "Harmonic " << harmonics[harmonic] << '\n';
       // std::cout << "Angle Delta " << angleDelta << '\n';
        double currentAngle = 0.0;
        for(int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            
            samples[i] += (float)sample * harmonicAmplitudes[harmonic]; // this does crash!
            currentAngle += angleDelta;

        }
        
    }


     samples[tableSize] = samples[0];
    
    // problem occurs before this!
    String thisMag = "Magnitude (upon creation): ";
    Logger::getCurrentLogger()->writeToLog(thisMag + String(tri9Table.getMagnitude(0, tableSize)));

    std::cout << "Triangle done!\n";
 
    normalise(tri9Table);
    
    waveforms.add(&tri9Table);


}
//
//void MainComponent::createTri9(int numOfHarmonics=9)
//{
//    tri9Table.setSize(1, tableSize);              // rect/tri difference, line 1
//
//    auto* samples = tri9Table.getWritePointer(0);  // rect/tri difference, line 2
//
////    std::unique_ptr<double[]> harmonics(new double[numOfHarmonics]);
////    std::unique_ptr<double[]> harmonicAmplitudes(new double[numOfHarmonics]);
//
//    double harmonics[] = {1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0, 17.0, 19.0};
//    double harmonicAmplitudes[] = {1.0, 0.322, 0.02233, 0.0111, 0.002345, 0.002354, 0.0003, 0.0001, 0.00001};
////    double currentHarmonic = 1.0;
////    double currentAmplitude = 1.0;
////    auto reciprocal = 1.0;
////    for(int i = 0; i < numOfHarmonics; ++i)
////    {
////        harmonics[i] = currentHarmonic;
////        reciprocal = pow(currentHarmonic, 2.0);
////        harmonicAmplitudes[i] = currentAmplitude / reciprocal; // rect/tri difference, line 3
////        currentHarmonic += 2.0;
////
////    }
//
//    for(int harmonic = 0; harmonic < numOfHarmonics; ++harmonic)
//    {
//        // each harmonic has it's own currentAngle and angleDelta
//        auto angleDelta = MathConstants<double>::twoPi / (double)(tableSize - 1.0) * harmonics[harmonic];
//        auto currentAngle = 0.0;
//        for(int i = 0; i < tableSize; ++i)
//        {
//            auto sample = std::sin(currentAngle);
//            samples[i] += (float)sample * harmonicAmplitudes[harmonic];
//            currentAngle += angleDelta;
//        }
//    }
//
//    samples[tableSize] = samples[0];
//
//    std::cout << "Triangle done!\n";
//
//
//}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    std::cout << "Prepare to play!\n";
//    String triTable = "Tritable in pretaperToPlay: ";
//    Logger::getCurrentLogger()->writeToLog(triTable + String(tri9Table.getNumSamples()));
//    String thisMag = "Magnitude: ";
//    Logger::getCurrentLogger()->writeToLog(thisMag + String(tri9Table.getMagnitude(0, tableSize)));
    
    auto waves = waveforms.size();
    wave = saw9;
    String numOfWaveforms = "Number of waveforms available: ";
    Logger::getCurrentLogger()->writeToLog(numOfWaveforms + String(waves));
   // allocate our wavetables here, set initial frequencies
    int notes[] = {51, 56, 63}; // default set of frequencies
    
    auto numOfVoices = sizeof(notes)/sizeof(int);
    
    voices = (int)numOfVoices;
    
    assert(numOfVoices==sizeof(notes)/sizeof(int));
    
    
    // set all oscillators to same freq
    for(int voiceIndex = 0; voiceIndex < numOfVoices; ++voiceIndex)
    {
        // create an instance of an oscillator
        // rename it to voice
        auto* voice = new WavetableOscillator_V1(*waveforms[WaveformType::saw9]);
        //auto* oscillator = new WavetableOscillator_V1(tri9Table);
        //auto* oscillator = new WavetableOscillator_V1(sineTable);
        
        // first do with random notes;
       // auto midinote = Random::getSystemRandom().nextDouble()*48.0 + 48.0;
        
        auto midinote = notes[voiceIndex];
        auto frequency = 440.0 * pow(2, (midinote - 69.0) / 12.0);
        
        // if our oscillator waveform changed, we must set frequency
        voice->setFrequency((float)frequency, (float)sampleRate);
        voicesArray.add(voice);
    }
    
    level = mainGain.getValue()/ numOfVoices;
}

// audio thread, don't allocate dynamic memory here!
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftOutputBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightOutputBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    bufferToFill.clearActiveBufferRegion();
    
    // iterate over the voices
    for(int oscIndex = 0; oscIndex < voicesArray.size(); ++oscIndex)
    {
        // check if there're oscillators?
        // check if sampleRate is legid?
        auto* oscillator = voicesArray.getUnchecked(oscIndex);
        for(auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
        // retreive the samples
            auto sampleValue = oscillator->getNextSample() * level;
            leftOutputBuffer[sample] += sampleValue; // accumulate value per sample
            rightOutputBuffer[sample] += sampleValue;
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double labelHeight = 50;
    double sliderHeight = jmax(getHeight()/4, 120);
    double sliderWidth = 75.0;
    auto area = getLocalBounds();
    auto cpuLabelArea = getLocalBounds().removeFromBottom(labelHeight).reduced(5);
    cpuLabelArea = cpuLabelArea.removeFromLeft(getWidth()/4);
    auto mainGainSliderArea = area;
    cpuUsageLabel.setBounds(cpuLabelArea);
    cpuUsageText.setBounds(cpuLabelArea);
    mainGain.setBounds(mainGainSliderArea.removeFromRight(sliderWidth).removeFromBottom(sliderHeight));
    
}
