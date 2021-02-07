// The MIT License
// Copyright © 2018 Ian Reichert-Watts
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#version 330 core

layout (location = 0) out vec4 fragColor;

uniform vec3 colour1;
uniform float shader_time;
uniform vec2 mouser;
uniform vec2 resolu;
//out vec4 fragColor;
uniform vec2 noteCords;
uniform sampler2D texture01;
uniform sampler2D texture02;

// SHARED PARAMS (Must be same as Image :/)

void main()
{
    vec2 iResolution = resolu;
    vec2 fragCoord = gl_FragCoord.xy;
    float iTime = shader_time;
    vec2 iMouse = mouser;
    iMouse.y = 1-iMouse.y+resolu.y; // flip upside co-ords
//    vec4 average = (
//                    texture(iChannel0,(U+vec2(0,1))/iResolution.xy)+
//                    texture(iChannel0,(U+vec2(1,0))/iResolution.xy)+
//                    texture(iChannel0,(U-vec2(0,1))/iResolution.xy)+
//                    texture(iChannel0,(U-vec2(1,0))/iResolution.xy))/4.;
//
//    fragColor = texture(iChannel0,U/iResolution.xy);
    
    
    fragColor = texture(texture01,fragCoord.xy/iResolution.xy);
    if (length(iMouse.xy-fragCoord.xy) < 10.) fragColor.x = 1.;
}
