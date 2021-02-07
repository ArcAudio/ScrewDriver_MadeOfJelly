/*
  ==============================================================================

    DyamicTexture.h
    Created: 14 Jun 2020 4:51:18am
    Author:  Mouldy Soul

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class DynamicTexture
{
    public:
    bool applyTo (OpenGLTexture& texture, float keys[], float timeOn[], float timeOff[]) // add midi array 127 array of floats, map floats to velocity
    {
        
        Image image;

        Colour cb = Colours::black;
        //Colour w = Colours::white;
        //Colour red = Colours::red;
        
        if (! image.isValid())
            image = Image (Image::ARGB, 128, 4, true);
        {
//           Graphics g (image);
//            const MessageManagerLock mml (ThreadPoolJob::getCurrentThreadPoolJob());
//            if (! mml.lockWasGained())
//                return false;
            
        for (int i =0; i < 127; i++)
        {
                float k = keys[i];
                if (k > 0)
                {
                    Colour p = Colour::fromHSL(0.0, 0.0, k, 1.0); // set brightness based on velocity
                    image.setPixelAt(i, 0, p);
                    
                   uint32 n = *(int32*)&timeOn[i];
                 //   uint32 n = timeOn[i];
                    int8 b0 = n & 0xFF;
                    int8 b1 = (n >> 8) & 0xFF;
                    int8 b2 = (n >> 16) & 0xFF;
                    int8 b3 = (n >> 24) & 0xFF;
                    //Colour p2 = Colour(n);
                    Colour p2 = Colour::fromRGBA(b0,b1,b2,b3);
                    image.setPixelAt(i, 1, p2);
                }
            

               if (k == 0)
                {
                    image.setPixelAt(i, 0, cb);
                
                    uint32 n = *(int32*)&timeOff[i];
                   // uint32 n = timeOff[i];
                    int8 b0 = n & 0xFF;
                    int8 b1 = (n >> 8) & 0xFF;
                    int8 b2 = (n >> 16) & 0xFF;
                    int8 b3 = (n >> 24) & 0xFF;
                    Colour p = Colour::fromRGBA(b0,b1,b2,b3);
                   // Colour p = Colour(n);
                    image.setPixelAt(i, 2, p);
                    // since_note off
                }

        }
         
    }
        texture.loadImage(image);
        return true;
}
//    static Image resizeImageToPowerOfTwo (Image image)
//    {
//        if (! (isPowerOfTwo (image.getWidth()) && isPowerOfTwo (image.getHeight())))
//            return image.rescaled (jmin (256, nextPowerOfTwo (image.getWidth())),
//                                   jmin (256, nextPowerOfTwo (image.getHeight())));
//
//        return image;
//    }
};

//
