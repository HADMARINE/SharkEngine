//
// Created by EunwooSong on 2020-10-28.
//

#ifndef SHARKENGINE_VULKANMESHSTRUCT_H
#define SHARKENGINE_VULKANMESHSTRUCT_H

#pragma once
/*--------------------------------------------------------------------------------------
Mesh data strucure and  and VertexFormat Data
--------------------------------------------------------------------------------------*/
struct VertexWithColor
{
    float x, y, z, w;   // Vertex Position
    float r, g, b, a;   // Color format Red, Green, Blue, Alpha
};

struct VertexWithUV
{
    float x, y, z, w;   // Vertex Position
    float u, v;         // Texture format U,V
};

static const VertexWithUV geometryData[] = {
        { -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f },  // -X side
        { -1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        { -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 0.0f },
        { -1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f },

        { -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f },  // -Z side
        {  1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 0.0f },
        {  1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
        { -1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 0.0f },

        { -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f }, // -Y
        {  1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
        {  1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { -1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        { -1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f },  // +Y side
        { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { -1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
        {  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        {  1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f },

        { 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f },  // +X side
        { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        { 1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f },

        { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },  // +Z side
        { -1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
        {  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
};



#endif//SHARKENGINE_VULKANMESHSTRUCT_H
