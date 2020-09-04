/*
  ==============================================================================

        1. Referencing objects - class done
        2. creating background thread - done
        3. GUI - done
        4. Open file using FileOpener, AudioSourceReader. Buffer - done
        5. Signal processing block - done
        5.1 Volume control
        6. Playing backwards
        7. Change speed / pitch shift
        8. LFO on speed 

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

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // first retain an active buffer
        ReferenceCountedBuffer::Ptr retainedBuffer (currentBuffer);
        
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
        auto outputSamplesRemaining = bufferToFill.numSamples;
        auto outputSamplesOffset = 0;
        
        // while we're still within a block
        while (outputSamplesRemaining > 0)
        {
            auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
            
            // if we run out of samples in the buffer before the block ends
            // we need to know how many samples to proceed with
            auto samplesThisTime = jmin(bufferSamplesRemaining, outputSamplesRemaining);
            
            // copy data from buffer into outputBuffer
            for(auto channel = 0; channel < numOutputChannels; ++channel)
            {
                
                // jassert (destStartSample >= 0 && destStartSample + numSamples <= size); -> fails on second block
                bufferToFill.buffer->copyFrom(channel, // destination channel
                            // destStartSample
                            bufferToFill.startSample + outputSamplesOffset,
                                              *currentAudioSampleBuffer,
                                              channel % numInputChannels,
                                              position,
                                              samplesThisTime);
            }
                
                // update position etc.
                outputSamplesRemaining -= samplesThisTime;
                outputSamplesOffset += samplesThisTime;
                position += samplesThisTime;
            
            if(position == currentAudioSampleBuffer->getNumSamples())
                position = 0;
        }
        
        // update position in current buffer
        retainedBuffer->position = position;
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
        auto buttonWidth = 80;
        openButton.setBounds(buttonBorder, buttonBorder, jmin(buttonWidth, getWidth() - 20), buttonHeight);
        clearButton.setBounds(buttonBorder, 2 * buttonBorder + buttonHeight, jmin(buttonWidth, getWidth() - 20), buttonHeight);
    }


private:
    //==============================================================================
    // Thread methods
    void run() override
    {
        while(!threadShouldExit())
        {
            checkForBufferToFree();
            wait(500);
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
        FileChooser chooser ("Please select a wav file you want to load for playback..",
                         {},
                         "*.wav");
        if (chooser.browseForFileToOpen())
        {
            File file = chooser.getResult();
            std::unique_ptr<AudioFormatReader> reader (formatManager.createReaderFor(file));
    
        // check if pointer is legid
        if(reader.get() != nullptr)
        {
            auto duration = reader->lengthInSamples / reader->sampleRate;
            if(duration < 2.0)
            {
                
                // this must not be deleted manually
                // it'll be removed from buffers
                // and destroyed in checkForBuffersToFree()
                // if the buffer is not needed by any of the processes anymore
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(
                                                                                   file.getFileName(),
                                                            reader->numChannels,
                                                             (int)reader->lengthInSamples
                                                                                   );
                // transfer data from AudioFormatReader object reader
                // to AudioSampleBuffer owned by ReferenceCountedBuffer newBuffer
                reader->read (newBuffer->getAudioSampleBuffer(),
                              0,
                              (int)reader->lengthInSamples,
                              0,
                              true,
                              true);
                
                // make it our currentBuffer
                currentBuffer = newBuffer;
                buffers.add(newBuffer);
                
            }
            else
            {
                // how do I throw and catch and error from without here?
            }
        }
        }
        
    }
    
    void clearButtonClicked()
    {
        currentBuffer = nullptr;
    }
    
    // file path to the background thread
    String chosenPath;
    
    // GUI
    TextButton openButton;
    TextButton clearButton;
    double level;
    Slider volume;
    
    // File reading
    AudioFormatManager formatManager;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
