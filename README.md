# ScrewDriver_MadeOfJelly

Essentially a mash of these resources:

OpenGL App with Juce: https://github.com/TimArt/JUCE-OpenGL-Template

OpenGl plugin with Juce: https://medium.com/@Im_Jimmi/using-opengl-for-2d-graphics-in-a-juce-plug-in-24aa82f634ff 

Faust to Juce: https://faustdoc.grame.fr/workshops/2020-04-10-faust-juce/

In an attempt to make this screwdriver made from Jelly actually useful to someone, I've added the relevant faust .dsp file, and the built AU component. The made with juce thing should come up, as it was made with Juce, but messing around with OpenGL at such a low level I think it's been overridden somehow! Side note you can still click the bottom left corner and opens up Juce website, it just doesn't have that sticker there.


No warranty or anything like that, use at your own peril. I made to fill a need in my own music, which was to have a polyphonic midi controlled resonator inside logic pro. Idea is you put on a midi track in logic, feed an audio signal into the resonator and then you have polyphonic midi control over the voices of a simple karplus strong algorithm. This process was made really simple and straight forward by using Faust as the DSP engine in conjunction with Juce. 

There's some silly Tonnetz grid, that maps midi notes to a value on a a hexgrid made procedurally inside a shader. There's a juce script that turns the Midi into an image texture and pipes into the shader as shaders find images very easy to digest. 

Faust/OpenGL/Juce:

https://youtu.be/7nH26ZVKpmQ

just as a midi visualiser: 

https://youtu.be/QT6ViUUIdZ0

I got bored with Juce and openGL and went on to trying it out with hardware using Faust2Teensy: 

https://youtu.be/yQpNf6XPNZA

Which has now evolved to this using Faust2Daisy, and the FX chain has got slightly more complex: 

https://www.youtube.com/watch?v=7Nbb8VqF5pY&t=1s

License is to thrill. Do what you want, experiment/dissect/remix etc.  
