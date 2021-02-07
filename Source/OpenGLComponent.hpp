//
//  OpenGLComponent.hpp
//  OpenGL 3D App Template - App
//
//  Created by Tim Arterbury on 3/21/20.
//  Copyright © 2020 TesserAct Music Technology LLC. All rights reserved.
//

#pragma once

#include <JuceHeader.h>
#include "OpenGLUtil/OpenGLUtil.h"
#include "ShapeVertices.hpp"
#include "PluginProcessor.h"
#include <glm/vec2.hpp> // glm::vec2
#define vec2 glm::vec2
#include "DyamicTexture.h"

//#include "IncomingMessageCallback.h"
//#include "MidiProcessor.h"
/** A custom JUCE Component which renders using OpenGL. You can use this class
    as a template for any Component you want to create which renders OpenGL inside
    of its view. If you were creating a spectrum visualizer, you might name this
    class SpectrumVisualizer.
 */
class OpenGLComponent : public Component,
                        private OpenGLRenderer,
                        private AsyncUpdater,
                        private MidiKeyboardStateListener
{
public:
    OpenGLComponent (KarplunkOgAudioProcessor& p);
    ~OpenGLComponent();
    
    //JuceShaderPluginAudioProcessorEditor (JuceShaderPluginAudioProcessor&);
    
    // OpenGLRenderer Callbacks ================================================
    void newOpenGLContextCreated() override;
    void openGLContextClosing() override;
    void renderOpenGL() override; // this is where the drawing happings, this callback
    
    void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    
    // Component Callbacks =====================================================
    void paint (Graphics& g) override;
    void resized () override;
    void timeframes();
    
    uint frame_count = 0;
    float prev_time = 0;
    float frame_time =0;
    float s_time =0;
    
    // Used to connect Draggable3DOrientation to mouse movments
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    
    
    int cellID;
    bool mDown;
    // AsyncUpdater Callback ===================================================
    /** If the OpenGLRenderer thread needs to update some form JUCE GUI object
        reserved for the JUCE Message Thread, this callback allows the message
        thread to handle the update. */
    void handleAsyncUpdate() override;
private:
    
    /** Attempts to compile the OpenGL program at runtime and setup OpenGL variables. */
    void compileOpenGLShaderProgram();
    void compileOpenGLBufferShaderProgram();
    double startTime;
    Point<int> screen_resolution { 1690, 800 };

    // OpenGL Variables
    OpenGLContext openGLContext;
    OpenGLTexture texture_01;

    std::unique_ptr<OpenGLShaderProgram> shaderProgram;
    std::unique_ptr<OpenGLShaderProgram> bufferAProgram;

    OpenGLUtil::UniformWrapper colour1 { "colour1" }; // Uniforms labelled here
    OpenGLUtil::UniformWrapper resolu { "resolu" }; // link to GLCompo:renderOpenGL
    OpenGLUtil::UniformWrapper shader_time { "shader_time" };
    OpenGLUtil::UniformWrapper mouser { "mouser" }; //make sure there's a uniform called this in shader
    OpenGLUtil::UniformWrapper noteCords { "noteCords" }; //make sure there's a uniform called this in shader
    OpenGLUtil::UniformWrapper texture01 { "texture01" };
    OpenGLUtil::UniformWrapper texture02 { "texture02" };

    Point<float> lastMousePosition;

    GLuint VAO, VBO;
    std::vector<Vector3D<GLfloat>> vertices;
    
    float midiNotesArray[144] = {0.0};
    
    float timeOnArray[144] = {0};
    float timeOffArray[144] = {0};
    
    float startCount;
    
    //Array<float> midiKeyIn;
    DynamicTexture dynamicTexture;
    // GUI Mouse Drag Interaction
    Draggable3DOrientation draggableOrientation;
    KarplunkOgAudioProcessor& processor;
    
    Slider dampSlider;
    Label dampLabel;
    
    // put pointers below, destructs bottom up, get rid of attachements before components
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderAttach;

    //GUI overlay status text
    String openGLStatusText;
    Label openGLStatusLabel;
    MidiKeyboardState keyboardState;
    
   
};

