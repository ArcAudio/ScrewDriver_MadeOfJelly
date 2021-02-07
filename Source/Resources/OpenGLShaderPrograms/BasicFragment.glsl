#version 330 core

//layout (location = 0) out vec4 fragColor;

//uniform vec2 pixelPos;
//uniform mat4 resolutio;
uniform vec3 colour1;
uniform float shader_time;
uniform vec2 mouser;
uniform vec2 resolu;
out vec4 fragColor;
uniform vec2 noteCords;
uniform sampler2D texture01;
uniform sampler2D texture02;
//vec3 cubialo;// = vec3(0.0);
//uniform vec2 mouse; // mouse position
//uniform vec2 pixelPos;
/*
 NB: THIS IS REALLY IMPORTANT, MAKE SURE YOU SAVE FROM PROJUCER TO REFLECT SHADER CHANGES
 IM SORRY FOR SHOUTING
 */

#define PI 3.1415926535
#define TWO_PI 6.2831853
#define PI2 6.28

#define HX vec2(1.0, 0.0)
#define HY vec2(0.5, 0.866)
#define HMAT mat2(HX, HY)
#define HSCALE 0.16666
//#define HSCALE 0.3333

//uniform vec2 resolution;


float hexD(vec2 p)
{
    p = abs(p);
    float c = dot(p,normalize(vec2(1.0,1.73)));
    c = max(c,p.x);
    return c;
}

float circle(vec2 uv, float rad)
{
    float d = length(uv); //distance function
    return smoothstep(rad,rad-0.01,d); // circle draw
}

vec2 rotate2D(vec2 uv, float r){ // takes uv co-ords and rotates them by r
    uv =  mat2(cos(r),-sin(r),
               sin(r),cos(r)) * uv;
    return uv;
}

vec3 triD(vec2 uv,float s)
{
    vec2 repeat =vec2 (1.0,1.73);
    vec2 h = repeat*0.5;
    vec2 a1=mod(uv,repeat)-h; // gridify, but it's offset
    vec2 b1=mod(uv-h,repeat)-h; // gridify and center
    vec2 getv = dot(a1,a1)<dot(b1,b1)?a1:b1;//if else function
    float d = 0.0;
    // Number of sides of your shape
    int N = 3;
    
    // Angle and radius from the current pixel
    float a = atan(getv.x,getv.y)+PI;
    float r = TWO_PI/float(N);
    
    vec2 id = getv-uv;
    // Shaping function that modulate the distance
    d = cos(floor(0.5+a/r)*r-a)*length(getv);
    vec3 col = vec3(1.0-smoothstep(s,s+0.01,d));//*sin(id.x*id.y+iTime)));
    
    return col;
}

vec2 getV(vec2 uv)
{
    vec2 repeat =vec2 (1.0,1.73);
    vec2 h = repeat*0.5;
    vec2 a1=mod(uv,repeat)-h; // gridify, but it's offset
    vec2 b1=mod(uv-h,repeat)-h; // gridify and center
    vec2 getv = dot(a1,a1)<dot(b1,b1)?a1:b1;//if else function without if/else
    
    return getv;
}

vec4 hexF (vec2 uv)
{
    vec2 getv = getV(uv);
    float x = atan(getv.x,getv.y);
    float y = 0.5-hexD(getv);
    vec2 id = uv-getv;
    
    vec4 co = vec4(x,y,id.x,id.y); // coords and id
    return co;
}

vec3 hexCircs (vec2 uv)
{
    vec2 getv = getV(uv);
    vec2 id = uv-getv;
    float o = circle(getv,0.2);
    vec3 hx = vec3(id.x,id.y,o);
    return hx;
}

void pixelToCubial(vec2 uv, /*out??wtf??*/ out vec3 cubial, out vec3 grid)
{
    vec2 axial = inverse(HMAT) * uv;
    cubial = vec3(axial, -axial.x - axial.y) / HSCALE;
    grid = round(cubial);
    vec3 dist = abs(cubial - grid);
    if (dist.x > dist.y && dist.x > dist.z)
    {
        
        grid.x = -grid.y - grid.z;
    }
    else if (dist.y > dist.z) {
        
        grid.y = -grid.x - grid.z;
    }
    else {
        
        grid.z = -grid.x - grid.y;
    }
}
float cubialDistance(vec3 a, vec3 b)
{
    vec3 c = abs(a - b);
    return max(c.x, max(c.y, c.z));
}

float normalizer(float x)
{
    float y = (x + 1.0) / 2.0;
    return y;
}

vec2 hex2Pix(vec2 hex, float size)
{

    float x = size * (sqrt(3.)*hex.x + sqrt(3)/2.*hex.y);
    float y = size * (3.0/2. * hex.y);
    return vec2(x,y);
}


float hash21(vec2 p)
{
    float f = p.x + 12.0 * p.y;
    return f;
}

int Id2Uv(vec2 id)
{
    return int ((id.x*4.0) + (id.y*7.0));
}

vec2 offsetToAxial(vec2 id) // Odd version
{
    float x = id.y - (id.x-(mod(id.x,2.))) / 2.;
    float z = id.x;
    float y = -x-z; // this isn't needed
    
    return vec2(z,x); // equivalent of q and r
}

vec2 offsetToAxialE(vec2 id) // Even version
{
    int x = int(round(id.y - (id.x+(mod(id.x,1.0))) / 2.));
    int z = int(round(id.x+0.01));
    int y = int(-x-z); // this isn't needed
    
    return vec2(x,z); // equivalent of q and r
}


void main()
{

    vec2 coords = gl_FragCoord.xy;

    vec2 resolution = resolu;
    vec2 uv = (coords.xy-0.5*resolution.xy)/resolution.y;
    vec2 nuv = coords.xy/resolution.xy;

    vec2 texUv = (coords.xy / resolution.xy);
    uv += 1.2;
    nuv*=3; // was 3

    //uv.y = 1-uv.y; // flip upside co-ords
    //uv*=3.0;
    vec2 uv2 = uv;
    vec2 uv3 = uv;
    vec3 col = vec3(0.0);

    uv2*=6.0;// scale of space // was 6
    uv2.y+=0.6; //offset up
    uv3*=(6.0);// scale of space // was 6
    float iTime = shader_time;
    iTime *= 0.1;
    vec4 iMouse = vec4(mouser,0,0.);

    vec3 grid;//= vec3(0.0);
    vec3 mouse;
    vec3 cubial;// = vec3(0.0);
    pixelToCubial((iMouse.xy - resolution.xy) / resolution.y, cubial, mouse);

    pixelToCubial(uv, cubial, grid);

    // get per tile coordinates

    vec3 coord = cubial - grid;
    vec3 dist = abs(coord.xyz - coord.zxy);
    
    vec4 h = hexF(uv2/(HSCALE/1.));//was1
    
    col -= (sin(grid*7333.0 + iTime) * 0.5 + 0.5)*0.5; // this should be a grid of hexagons
    
    //uv3.y+=0.02;
    float circ = hexCircs(uv).z;

    vec3 vis = hexCircs(uv3);

    vec4 h2 = hexF(uv3);
    vec3 t = triD(uv2,0.29);
    vec3 t2 = triD(uv2,0.27);
    float c = smoothstep(0.02,0.01,h2.y);

   // vec3 e2 = (0.5 + 0.5*cos(iTime+uv.xyx+vec3(0.0,2,4)) * 0.75);
    
    col+=vis.z;//good circles reversing sign also cool
    
    //col+=h2.xyz; //visual debug
    //col+=vis.y; //visual debug
    
    col+=t;
    col-=t2;
    col+=c;//Outlines of hexagons
//    float mouseDist;
//    if (iMouse.x > .0 && iMouse.y > 0.0 && (mouseDist = cubialDistance(grid, mouse)) < 1.0)
//    {
//        float mouseDist = cubialDistance(grid, mouse);
//        col = mix(col, vec3(1.0), max(0.0, 0.5));
//    }
    
    vec4 colour = vec4 (0.0, 0.0, 0.0, 1.0);

    vec4 hexFunc = hexF(uv*6.0f);
    
    vec4 colH = vec4(0);
    colH.rg += hexFunc.zw*0.1;

    float hasher = hash21(vec2(hexFunc.z,hexFunc.w));
    
    vec2 txRes = vec2(12,12);
    vec2 nnuv = (coords.xy/resolution.xy);
    
    //hexFunc.z -= .1;
    
    ivec2 axialID = ivec2(offsetToAxialE(hexFunc.wz));
    
    int ax = (axialID.x*7)+(axialID.y*4);
    
    vec4 argb = texelFetch( texture01, ivec2(ax, 2), 0); //on
    //int timeVal = (int(argb.b) << 24) + (int(argb.g) << 16) + (int(argb.r) << 8) + int(argb.a);
    
    //vec4 argb = texelFetch( texture01, ivec2(x, 2), 0); //on
    //int timeVal = (argb.b << 24) | (argb.g << 16) | (argb.r << 8) | argb.a;
    
   // int timeVal = (int(argb.b) << 24) | (int(argb.g) << 16) | (int(argb.r) << 8) | int(argb.a);
    //uint timeVal = uint( (uint(argb.a) << 24) + (uint(argb.b) << 16) + (uint(argb.b) << 8) + uint(argb.r));
    
    vec4 argb2 = texelFetch( texture01, ivec2(ax, 1), 0); //off
   // int timeVal2 = int( (int(argb2.b) << 24) | (int(argb2.g) << 16) | (int(argb2.r) << 8) | int(argb2.a));
    
    vec4 keyTex2 = texelFetch( texture01, ivec2(ax, 3), 0);
    vec4 keyTex  = texture( texture01, vec2((hexFunc.z+6.0)/12.0,(hexFunc.w+3.0)/6.0));
    
    vec4 keys  = vec4(0);
    //int tval = timeVal2 - timeVal;
    //uv *= (timeVal*255.0);
    vec3 e2 = (0.5 + 0.5*cos(iTime+uv.xyx+vec3(0.0,2,4)) * 0.75);
   // keys += vec4(keyTex*1.0f);

    vec4 e = vec4(vec3(col+keys.xyz),1.0);
    fragColor = vec4(e+keyTex2);
   //fragColor = vec4(argb2);
} 
