/*
============================================================================
Agmd3D - 3D Engine
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <Core/Texture/Texture.h>
#include <Core/Buffer/RenderBuffer.h>
#include <Core/Enums.h>
#include <Utilities/SmartPtr.h>

#include <Container/Vector.h>

namespace Agmd
{
    typedef a_vector<Texture> TextureMap;
    typedef a_vector<RenderBuffer*> RenderBufferMap;

    class AGMD3D_EXPORT FrameBuffer
    {
    public:
        FrameBuffer();
        virtual ~FrameBuffer();

        virtual void SetTexture(const Texture&, a_uint32) = 0;
        virtual void SetTextureCube(const Texture&, a_uint32, int face) = 0;
        virtual void SetRender(RenderBuffer*, a_uint32) = 0;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void Clear(a_uint32 flag = CLEAR_ALL) = 0;

        virtual void DrawBuffer(a_uint32 flag) = 0;
        virtual void DrawBuffers(a_uint32 nbuffer, a_uint32 flag[]) = 0;
        virtual a_uint32* GenerateBufferFlags(a_uint32 count, a_uint32 flags[]) = 0;
        virtual a_uint32* GenerateBufferFlags(a_uint32*, a_uint32 count, a_uint32 flags[]) = 0;
        virtual void ReadBuffer(a_uint32 flag) = 0;

    protected:
        TextureMap m_TextureMap;
        RenderBufferMap m_RenderBufferMap;
    };

    typedef SmartPtr<FrameBuffer> FrameBufferPtr;
}

#endif //FRAMEBUFFER_H
