/*
  ==============================================================================

    ReferenceCountedBuffer.h
    Created: 24 Dec 2019 1:16:40pm
    Author:  SFD

  ==============================================================================
*/

#pragma once

// this object holds AudioSampleBuffer and the playback position
class ReferenceCountedBuffer: public ReferenceCountedObject
{
public:
    
    // manages the reference count of the object
    typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
    
    // constructor
    ReferenceCountedBuffer (const String& nameToUse,
                            int numChannels,
                            int numSamples)
    :name(nameToUse),
    buffer(numChannels, numSamples)
    
    {
        DBG(String("Buffer named '" ) + name + " '. Its numChannels = " + String(numChannels) + " and numSamples = " + String(numSamples));
    }
    
    ~ReferenceCountedBuffer()
    {
        DBG(String("Buffer named ' ") + name + " ' has been destroyed.");
    }
    
    // we need to keep position public
    int position = 0;
    
    AudioSampleBuffer* getAudioSampleBuffer()
    {
        return &buffer;
    }
private:
    String name;
    AudioSampleBuffer buffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReferenceCountedBuffer)
};
