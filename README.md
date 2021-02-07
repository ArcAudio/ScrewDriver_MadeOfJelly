# ScrewDriver_MadeOfJelly

Essentially a mash of these resources:

OpenGL App with Juce: https://github.com/TimArt/JUCE-OpenGL-Template

OpenGl plugin with Juce: https://medium.com/@Im_Jimmi/using-opengl-for-2d-graphics-in-a-juce-plug-in-24aa82f634ff 

Faust to Juce: https://faustdoc.grame.fr/workshops/2020-04-10-faust-juce/

In an attempt to make this screwdriver made from Jelly actually useful to someone, I've added the relevant faust .dsp file, and the built AU component. The made with juce thing should come up, as it was made with Juce, but messing around with OpenGL at such a low level I think it's been overridden somehow! Side note you can still click the bottom left corner and opens up Juce website, it just doesn't have that sticker there.


No warranty or anything like that, use at your own peril. I made to fill a need in my own music, which was to have a polyphonic midi controlled resonator inside logic pro. Idea is you put on a midi track in logic, feed an audio signal into the resonator and then you have polyphonic midi control over the voices of a simple karplus strong algorithm. This process was made really simple and straight forward by using Faust as the DSP engine in conjunction with Juce. 

There's some silly Tonnetz grid, that maps midi notes to a value on a a hexgrid made procedurally inside a shader. There's a juce script that turns the Midi into an image texture and pipes into the shader as shaders find images very easy to digest. 

Faust/OpenGL/Juce:

[![IMAGE ALT TEXT](http://img.youtube.com/vi/7nH26ZVKpmQ/0.jpg)](http://www.youtube.com/watch?v=7nH26ZVKpmQ "Faust OpenGL Juce")

just as a midi visualiser: 

[![IMAGE ALT TEXT](http://img.youtube.com/vi/QT6ViUUIdZ0/0.jpg)](http://www.youtube.com/watch?v=QT6ViUUIdZ0 "Midi Visualiser")

I got bored with Juce and openGL and went on to trying it out with hardware using Faust2Teensy: 

[![IMAGE ALT TEXT](http://img.youtube.com/vi/yQpNf6XPNZA/0.jpg)](http://www.youtube.com/watch?v=yQpNf6XPNZA "Faust2Teensy")

Which has now evolved to this using Faust2Daisy, and the FX chain has got slightly more complex: 

[![IMAGE ALT TEXT](http://img.youtube.com/vi/7Nbb8VqF5pY/0.jpg)](http://www.youtube.com/watch?v=7Nbb8VqF5pY "Faust2Daisy")

License is to thrill. Do what you want, experiment/dissect/remix etc.  
