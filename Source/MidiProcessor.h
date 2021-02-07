/*
  ==============================================================================

    MidiProcessor.h
    Created: 14 Jun 2020 12:17:44am
    Author:  Mouldy Soul

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MidiProcessor
{
    public :

    void process(MidiBuffer& midiMessages) // pass reference& to an array
    {
        //midiBuffer.clear();
        
        for (auto message: midiMessages) // this is a funky for loop
        {
            auto currentMessage = message.getMessage();
            
            if (currentMessage.isNoteOn())
            {
            }
            
            if (currentMessage.isNoteOn())
            {
                
            }
        }

        midiMessages.swapWith(midiBuffer);
    }

    MidiBuffer midiBuffer;
};
