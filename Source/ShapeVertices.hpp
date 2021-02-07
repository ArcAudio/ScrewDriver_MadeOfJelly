//
//  ShapeVertices.hpp
//  OpenGL 3D App Template - App
//
//  Created by Tim Arterbury on 3/22/20.
//  Copyright © 2020 TesserAct Music Technology LLC. All rights reserved.
//

#pragma once

#include <JuceHeader.h>

namespace ShapeVertices
{

static std::vector<Vector3D<GLfloat>> generateTriangle()
{
    ///        Left vert                 Right vert            Middle/top vert
    return { { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { -1.0f,  1.0f, 0.0f },
        
            { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { -1.0f,  1.0f, 0.0f },
        
    };
}

} // namespace ShapeVertices
