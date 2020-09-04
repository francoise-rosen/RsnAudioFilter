/*
  ==============================================================================

        1. Referencing objects - class done
        2. creating background thread - done
        3. GUI - done
        4. Open file using FileOpener, AudioSourceReader. Buffer - done
        4.1 Open path on background thread - done
        5. Signal processing block - done
        5.1 Volume control, smooth it!
        6. Playing backwards
        7. Change speed / pitch shift
        8. LFO on speed
 
 Create a class like Groove (check max for details):
 - read a buffer
 - loop on/off
 - Signal -> read through buffer. Signal with value 1 will read it with original pitch
 - loop a region
 - slice a file
 
 -> Synthesiser
 add Waveshaper and Filter
 AllPass Cloud
 Polyphony

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Slider::Listener,
                        private Thread
{
public:
    
    // Reference checking class
    
    // this class contains a pointer to a buffer and a position of playback!
    
    class ReferenceCountedBuffer : public ReferenceCountedObject
    {
    public:
        
        // This class uses an Atomic<int> value to hold the reference count, so that the
        // pointers can be passed between threads safely.
        
        typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
        
        ReferenceCountedBuffer(const String& nameToUse,
                               int numChannels,
                               int numSamples)
                            :name (nameToUse),
                            buffer(numChannels, numSamples)
        {
            DBG (String ("Buffer named ") + name + " is constructed. Num Channels = " + String(numChannels) + ". Num samples = " + String(numSamples));
            
        }
        
        ~ReferenceCountedBuffer()
        {
            DBG (String ("Buffer named ") + name + "destroyed.");
        }
        
        // access a buffer that this object contains
        AudioSampleBuffer* getAudioSampleBuffer()
        {
            return &buffer;
        }
        
        // access or set a position of playback
        int position = 0;
        
    private:
        String name;
        AudioSampleBuffer buffer;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
     
    };
    
    
    //==============================================================================
    MainComponent() : Thread ("Background thread")
    {
        setSize (400, 300);
        
        // initialise GUI
        
        // Label
        addAndMakeVisible(fileLabel);
        fileLabelFont.setHeight(fileLabelFontHeight);
       // fileLabel.setColour(Label::backgroundColourId, Colours::cyan);
        fileLabel.setColour(Label::textColourId, Colours::black);
        fileLabel.setFont(fileLabelFont);
        
        // Open Button
        addAndMakeVisible(openButton);
        openButton.setButtonText("Open...");
        //openButton.addListener(this); // must inherit from Button::Listener
        openButton.onClick = [this]{openButtonClicked();};
        
        // Clear Button
        addAndMakeVisible(clearButton);
        clearButton.setButtonText("Clear");
        clearButton.onClick = [this]{clearButtonClicked();};
        
        // Volume Slider
        mainGain.addListener(this);
        addAndMakeVisible(mainGain);
        mainGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
        mainGain.setTextValueSuffix("dBFS");
        mainGain.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
        mainGain.setRange(0.0, 1.0);
        mainGain.setValue(0.5);
        mainGain.setSkewFactor(0.25);
        
        
        // Mute Button
        // Lable Time running
        
        
        formatManager.registerBasicFormats();
      
        setAudioChannels (0, 2);
        
        startThread();

    }

    ~MainComponent()
    {
        stopThread(4000);
        // This shuts down the audio device and clears the audio source.
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        outputLevel = mainGain.getValue();
        targetOutputLevel = outputLevel;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // first retain an active buffer
        ReferenceCountedBuffer::Ptr retainedBuffer (currentBuffer);
    
        
        auto localTargetLevel = targetOutputLevel;
        
        // check if its a null
        if(retainedBuffer == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
        // now we need to get the buffer that object holds
        // and its playback position
        auto* currentAudioSampleBuffer = retainedBuffer->getAudioSampleBuffer();
        auto position = retainedBuffer->position;
        
        // set I/O
        auto numInputChannels = currentAudioSampleBuffer->getNumChannels();
        auto numOutputChannels = bufferToFill.buffer->getNumChannels();
        
        // we need to keep track of remaining samples in output buffer (block size)
        // and in input buffers
        // so we know if there're less samples in input buffer left
        // is this relevant if loop is on? probably not
        auto outputSamplesRemaining = bufferToFill.numSamples;
        auto outputSamplesOffset = 0;
        
        // while we're still within a block
        while (outputSamplesRemaining > 0)
        {
            auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
            
            // if we run out of samples in the buffer before the block ends
            // we need to know how many samples to proceed with
            auto samplesThisTime = jmin(bufferSamplesRemaining, outputSamplesRemaining);
            
            
            // if level slider changed:
          
                
            for (int channel = 0; channel < numOutputChannels; ++channel)
                {
                    
                    if( localTargetLevel != outputLevel)
                    {
                    // copy input buffer into output buffer
                    bufferToFill.buffer->copyFrom(
                                                channel,
                                                bufferToFill.startSample + outputSamplesOffset,   // [1]
                                                *currentAudioSampleBuffer, // [2]
                                                channel % numInputChannels,
                                                position,
                                                samplesThisTime
                                                  );
                    // 1 start sample in output buffer, outputSampleOffset is needed
                    // for incrementing the start sample every time in buffer has less remaining samples than the output buffer
                    // 2 dereferencing the source buffer
                    
                    // apply gain ramp
                    
                    bufferToFill.buffer->applyGainRamp(
                                                       channel,
                                                       bufferToFill.startSample + outputSamplesOffset,
                                                       outputSamplesRemaining,
                                                       outputLevel,
                                                       localTargetLevel
                                                       );
                    }
            
            else
            {
                // no changles in level
                
                // try outside the if condition
                bufferToFill.buffer->copyFrom(
                                              channel,
                                              bufferToFill.startSample + outputSamplesOffset,
                                              *currentAudioSampleBuffer,
                                              channel % numInputChannels,
                                              position,
                                              samplesThisTime
                                              );
                
                bufferToFill.buffer->applyGain(
                                               channel,
                                               bufferToFill.startSample + outputSamplesOffset,
                                               outputSamplesRemaining,
                                               outputLevel
                                               );
                    }
                    
                }
                
                
                // update position etc.
                outputSamplesRemaining -= samplesThisTime;
                outputSamplesOffset += samplesThisTime;
                position += samplesThisTime;
            
            outputLevel = localTargetLevel;
            
            if(position == currentAudioSampleBuffer->getNumSamples())
                position = 0;
        }
        
        // update position in current buffer
        retainedBuffer->position = position;
    }
    
    // listeners
    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &mainGain)
            targetOutputLevel = slider->getValue();
    }

    void releaseResources() override
    {
        currentBuffer = nullptr;
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
 
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));


    }

    void resized() override
    {
        auto buttonBorder = 10;
        auto buttonHeight = 40;
        auto labelThickness = 40;
        auto buttonWidth = getWidth()-20;
        auto labelArea = getLocalBounds();
        openButton.setBounds(buttonBorder, buttonBorder, jmin(buttonWidth, getWidth() - 20), buttonHeight);
        clearButton.setBounds(buttonBorder, 2 * buttonBorder + buttonHeight, jmin(buttonWidth, getWidth() - 20), buttonHeight);
        
        fileLabel.setBounds(
                            labelArea.removeFromLeft(getWidth() - buttonBorder).removeFromRight(getWidth() - buttonBorder).removeFromBottom(labelThickness).reduced(buttonBorder));
//        fileLabel.setBounds(buttonBorder, getHeight() - labelThickness, getWidth() - 20, labelThickness);
        
        // rough positioning
        auto gainSliderArea = getLocalBounds();
        auto sliderHight = 100;
        auto sliderWidth = 30;
        mainGain.setBounds(gainSliderArea.removeFromBottom(sliderHight).removeFromRight(sliderWidth));
}

private:
    //==============================================================================
    // Thread methods
    void run() override
    {
        while(!threadShouldExit())
        {
            checkForPathToOpen();
            checkForBufferToFree();
            wait(500);
        }
    }
    
    // this function checks the chosenPath member by swapping it into a local var
    void checkForPathToOpen()
        {
            String pathToOpen;
            // swap it with chosenPath
            pathToOpen.swapWith(chosenPath);
            
            // check if the path is not empty
            if (pathToOpen.isNotEmpty())
            {
                File file {pathToOpen};
                // create a reader for this file
                std::unique_ptr<AudioFormatReader> reader (formatManager.createReaderFor(file));
                
                // check if the pointer is not null
                if (reader.get() != nullptr)
                {
                    auto duration = reader->lengthInSamples / reader->sampleRate;
                    
                    if(duration < 2)
                    {
                        // we create a reference
                        ReferenceCountedBuffer::Ptr newBuffer = new     ReferenceCountedBuffer(file.getFileName(),
                                                   reader->numChannels,
                                                                                               (int)reader->lengthInSamples);
                        
                        // read from a reader into a buffer owned by newBuffer
                        reader->read(
                                     // destination
                                     newBuffer->getAudioSampleBuffer(),
                                     // startSample
                                     0,
                                     // samples to read
                                     (int)reader->lengthInSamples,
                                     0,
                                     true, true
                                     );
                        currentBuffer = newBuffer;
                        buffers.add(newBuffer);
                        
                        String fileData;
                        fileData << file.getFileName();
                        fileData << duration;
                        
                        // access to message loop
                        const MessageManagerLock mmLock;
                        fileLabel.setText(fileData, dontSendNotification);
                    }
                    else
                    {
                        const MessageManagerLock mmLock;
                        fileLabel.setText("File is too long", dontSendNotification);
                    }
                }
            }
                                
        }
    
    
    void checkForBufferToFree()
    {
        // if we iterate forward we may have problems array index access
        for( auto i = buffers.size(); --i >= 0;)
        {
            // create a copy of each item in buffers array
            ReferenceCountedBuffer::Ptr localBuffer (buffers.getUnchecked(i));
            
            // if reference count equals 2 then we know
            // that no other process (thread) can be using the buffer
            // 1st reference is in buffers
            // 2nd is stored in localBuffer
            
            if(localBuffer->getReferenceCount() == 2)
                buffers.remove(i);
        }
    }
    
    // Reference Counted members
    
    ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;
    
    // File
    
    void openButtonClicked()
    {
        FileChooser chooser ("Select a wave file for playback..",
                             {},
                             "*.wav");
        
        if (chooser.browseForFileToOpen())
        {
            File file = chooser.getResult();
            auto path = file.getFullPathName();
             chosenPath.swapWith(path);
            //chosenPath = path;
            notify(); // wake up a background thread!
            // since we're going to call a function to open a file on a background thread
        }

        
    }
    
    void clearButtonClicked()
    {
        currentBuffer = nullptr;
        const MessageManagerLock msg;
        fileLabel.setText("Open a file for playback...", dontSendNotification);
    }
   
    // GUI
    TextButton openButton;
    TextButton clearButton;
    ToggleButton loop;
    double outputLevel;
    double currentOutputLevel;
    double targetOutputLevel;
    Slider mainGain;
    Label fileLabel;
    Font fileLabelFont;
    float fileLabelFontHeight = 12.0f;
    
    // File reading
    AudioFormatManager formatManager;
    
    
       // file path to the background thread
    String chosenPath;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
