//  Created by Tim Arterbury on 3/21/20.
//  Copyright © 2020 TesserAct Music Technology LLC. All rights reserved.
//
#include "OpenGLComponent.hpp"


OpenGLComponent::OpenGLComponent(KarplunkOgAudioProcessor& p)
: processor (p)
{
    // Sets the OpenGL version to 3.2
    openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
    OpenGLPixelFormat pixelFormat;
    pixelFormat.multisamplingLevel = 4; // Change this value to your needs.
    openGLContext.setPixelFormat(pixelFormat);
    // Setup midi keyboard listening:
    processor.midiKeyState.addListener(this);
    
    dampSlider.setRange (0.0, 0.99);
    addAndMakeVisible (dampSlider);
    sliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.vTreeState, "SLIDER", dampSlider);

//    dampSlider.onValueChange = [this] {
//        processor.setDamp(dampSlider.getValue());
//    };
    
        //dampSlider.setValue(0.9);
    
    addAndMakeVisible(dampLabel);
    dampLabel.setText ("Damp", dontSendNotification);
    dampLabel.attachToComponent (&dampSlider, true);
    // Set default 3D orientation for the draggable GUI tool
    draggableOrientation.reset ({ 0.0, 1.0, 0.0 });
    // Attach the OpenGL context
    openGLContext.setRenderer(this); // associating context with renderer: important
    openGLContext.attachTo(*this);
    openGLContext.setContinuousRepainting(true); // Enable rendering
    

    
    // Setup OpenGL GUI Overlay Label: Status of Shaders, compiler errors, etc.
//    addAndMakeVisible (openGLStatusLabel);
//    openGLStatusLabel.setJustificationType (Justification::topLeft);
//    openGLStatusLabel.setFont (Font (14.0f));
    setSize(screen_resolution.x, screen_resolution.y);
    
}

OpenGLComponent::~OpenGLComponent()
{
    texture_01.unbind();
    processor.midiKeyState.removeListener(this);
   
    openGLContext.setContinuousRepainting (false);
    openGLContext.detach();
}

// OpenGLRenderer Callbacks ================================================
void OpenGLComponent::newOpenGLContextCreated()
{
    compileOpenGLShaderProgram();
    vertices = ShapeVertices::generateTriangle();
    // Generate opengl vertex objects ==========================================
    openGLContext.extensions.glGenVertexArrays(1, &VAO); // Vertex Array Object
    openGLContext.extensions.glGenBuffers (1, &VBO);     // Vertex Buffer Object
    
    // Bind opengl vertex objects to data ======================================
    openGLContext.extensions.glBindVertexArray (VAO);
    
    // Fill VBO buffer with vertices array
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, VBO);
    openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER,
                                           sizeof (GLfloat) * vertices.size() * 3,
                                           vertices.data(),
                                           GL_STATIC_DRAW);


    // Define that our vertices are laid out as groups of 3 GLfloats
    openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE,
                                                    3 * sizeof (GLfloat), NULL);
    openGLContext.extensions.glEnableVertexAttribArray (0);
    
    startCount = Time::getMillisecondCounter();
}

void OpenGLComponent::openGLContextClosing()
{
     texture_01.release();
}

 void OpenGLComponent::handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //if (velocity > 0.0)
    //{
        midiNotesArray[midiNoteNumber] = velocity;
   // }
   // float now = Time::getMillisecondCounter() - startCount;
    
    //timeOnArray[midiNoteNumber] = now;
    
   // processor.keyOn(midiNoteNumber,int(127*velocity));

   // DBG(String::formatted("%i Note=", midiNoteNumber));
   // DBG(String::formatted("%f on=", timeOnArray[midiNoteNumber]));
    //DBG(String::formatted("%f Velocity=", midiNotesArray[midiNoteNumber] ));
};




 void OpenGLComponent::handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
   // float t = Time::getMillisecondCounter() - startCount;
    midiNotesArray[midiNoteNumber] = 0.0;
    //processor.keyOff(midiNoteNumber);
    //timeOffArray[midiNoteNumber] = t;

    //DBG(Time::currentTimeMillis());
    
   // DBG(timeOffArray[midiNoteNumber]);
    //DBG(String::formatted("%f Velocity=", midiNotesArray[midiNoteNumber]));
   // DBG(String::formatted("%f off=", timeOffArray[midiNoteNumber]));
};



void OpenGLComponent::renderOpenGL()
{
    jassert (OpenGLHelpers::isContextActive());
    // Scale viewport
    const float renderingScale = (float) openGLContext.getRenderingScale();
    glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
    
    // Set background color
    OpenGLHelpers::clear (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    shaderProgram->use();
    
    shaderProgram->setUniform("colour1", 1.f, 0.f, 0.f);
    float nx = processor.noteCordX;
    float ny = processor.noteCordY;
    shaderProgram->setUniform("noteCords", nx, ny);
    dynamicTexture.applyTo(texture_01 ,midiNotesArray, timeOnArray,timeOffArray); // This is midi keyboard texture
    texture_01.bind(); // this is binging midi texture
    shaderProgram->setUniform("texture01", 0); // this is setting midi texture as the uniform texture01
    timeframes();
    float  tmr = frame_count; // This seems to be dragging and breaks into quads, but doesn't wrap or start from 0 every second
    tmr*=0.1; // scale of time/framecount here
    shaderProgram->setUniform("shader_time", tmr);
    shaderProgram->setUniform("resolu", (renderingScale * getWidth()) ,(renderingScale * getHeight()));
    //float mx = getMouseXYRelative().getX(); // Get relative pos in relation to top left of component, not pix co-ords
    //float my = getMouseXYRelative().getY();
    //shaderProgram->setUniform("mouser", mx,my);

    openGLContext.extensions.glBindVertexArray(VAO);
    glDrawArrays (GL_TRIANGLES, 0, (int) vertices.size());
    openGLContext.extensions.glBindVertexArray (0);
}

// JUCE Component Callbacks ====================================================
void OpenGLComponent::paint (Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OpenGLComponent::resized ()
{
    const int sliderLeft = 80;
    dampSlider.setBounds (sliderLeft, 120, getWidth() - sliderLeft - 20, 20);
    draggableOrientation.setViewport (getLocalBounds());
    openGLStatusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
}

void OpenGLComponent::timeframes()
{ // Frame counter taken from Juce forum 
    float current_time = Time::currentTimeMillis(); frame_count++; if (current_time - prev_time >= 1000.0 ){ frame_time = 1000. / frame_count;
        //DBG(String::formatted("%f ms/frame", frame_time));
        frame_count = 0; prev_time = current_time;
    }
}

void OpenGLComponent::mouseDown (const MouseEvent& e)
{
    mouseDrag(e);
    processor.setMouseDown(true);
    vec2 coOrds = vec2(getMouseXYRelative().getX(),getMouseXYRelative().getY());
    draggableOrientation.mouseDown (e.getPosition());
}


void OpenGLComponent::mouseDrag (const MouseEvent& e)
{
    
    mDown = (true);
    lastMousePosition = e.position;
    repaint();
}

void OpenGLComponent::mouseUp (const MouseEvent& e)
{
    mDown = (false);
    processor.setMouseUp(true);
    repaint();
}

void OpenGLComponent::handleAsyncUpdate()
{
    openGLStatusLabel.setText (openGLStatusText, dontSendNotification);
}

// OpenGL Related Member Functions =============================================
void OpenGLComponent::compileOpenGLShaderProgram()
{
    std::unique_ptr<OpenGLShaderProgram> shaderProgramAttempt
        = std::make_unique<OpenGLShaderProgram> (openGLContext);
    
    // Attempt to compile the program
    if (shaderProgramAttempt->addVertexShader ({ BinaryData::BasicVertex_glsl })
        && shaderProgramAttempt->addFragmentShader ({ BinaryData::BasicFragment_glsl })
        && shaderProgramAttempt->link())
    {
        shaderProgram.reset (shaderProgramAttempt.release());
        openGLStatusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        openGLStatusText = shaderProgramAttempt->getLastError();
    }

    triggerAsyncUpdate(); // Update status text
}

void OpenGLComponent::compileOpenGLBufferShaderProgram()
{
    std::unique_ptr<OpenGLShaderProgram> shaderProgramAttempt
    = std::make_unique<OpenGLShaderProgram> (openGLContext);
    // Attempt to compile the program
    if (shaderProgramAttempt->addVertexShader ({ BinaryData::BasicVertex_glsl })
        && shaderProgramAttempt->addFragmentShader ({ BinaryData::mainShader1_glsl })
        && shaderProgramAttempt->link())
    {
        bufferAProgram.reset (shaderProgramAttempt.release());
        openGLStatusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        openGLStatusText = shaderProgramAttempt->getLastError();
    }
    
    triggerAsyncUpdate(); // Update status text
}
