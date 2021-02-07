#version 330 core

layout (location = 0) out vec4 fragColor;

//out vec4 fragColor;
uniform sampler2D texture01;
uniform vec2 resolu;
//uniform sampler2D texture01;


void main()
{
    vec2 iResolution = resolu;
    vec2 p = gl_FragCoord.xy/iResolution.xy;
    vec4 col = vec4(1.0,0.0,0.0,0.0);
    fragColor = texture(texture01, p)+col;
    //fragColor = col;
   //
}
