/*
============================================================================
GLRender - Opengl Renderer specialization
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

template < int bufferCount>
inline DXBuffer<bufferCount>::DXBuffer(unsigned long count, ID3D11Buffer** buffer) :
BaseBuffer    (count),
//m_Buffer   (buffer),
m_currentBuffer(0),
m_bufferCount(bufferCount)
{
    for(a_uint32 i = 0; i < m_bufferCount; i++)
        m_Buffer[i] = buffer[i]; //temp
}

template < int bufferCount>
inline DXBuffer<bufferCount>::~DXBuffer()
{
    if (m_Buffer) // fail 
       ; //DXDriver::glDeleteBuffers(m_bufferCount, m_Buffer);
    //if(m_sync)
        //DXDriver::glDeleteSync(m_sync);
}

template < int bufferCount>
inline ID3D11Buffer* DXBuffer<bufferCount>::GetBuffer() const
{
    return m_Buffer[m_currentBuffer];
}

template < int bufferCount>
inline void* DXBuffer<bufferCount>::Lock(unsigned long offset, unsigned long size, unsigned long flags)
{
    DXDriver& dxDriver =  DXDriver::Instance();
    //DXDriver::glBindBuffer(m_Buffer[m_currentBuffer]);
    D3D11_MAPPED_SUBRESOURCE ms;
    dxDriver.DeviceContext()->Map(m_Buffer[m_currentBuffer],NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    unsigned char* buffer = reinterpret_cast<unsigned char*>(ms.pData);
    return buffer + offset;
}

template < int bufferCount>
inline void* DXBuffer<bufferCount>::LockByte(unsigned long offset, unsigned long size, unsigned long flags)
{
    //DXDriver::glBindBuffer(m_Buffer[m_currentBuffer]);
    unsigned char* buffer = NULL;//reinterpret_cast<unsigned char*>(DXDriver::glMapBufferRange(offset, size, DXEnum::LockBitsFlags(flags) | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT));

    //if(flags & LOCK_SYNC_WAIT)
        //m_sync = DXDriver::glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE,0);
    return Lock(offset,size,flags);
}

template < int bufferCount>
inline void DXBuffer<bufferCount>::FillByte(unsigned char* data, unsigned long offset, unsigned long size)
{
    //DXDriver::glBindBuffer(m_Buffer[m_currentBuffer]);
    //DXDriver::DXBufferSubData(ID3D11Buffer*,offset,size,data);
}

template < int bufferCount>
inline void DXBuffer<bufferCount>::Unlock()
{
    DXDriver& dxDriver = DXDriver::Instance();
    dxDriver.DeviceContext()->Unmap(m_Buffer[m_currentBuffer],NULL);
}


template < int bufferCount>
inline void DXBuffer<bufferCount>::Flush()
{
    //glFlush();
}

template < int bufferCount>
inline void DXBuffer<bufferCount>::Bind(a_uint32 /*bindpoint*/)
{}

#define TIMEOUT 100000
template < int bufferCount>
void DXBuffer<bufferCount>::WaitSync()
{
//     DXDriver::glClientWaitSync(m_sync,0,TIMEOUT);
//     DXDriver::glDeleteSync(m_sync);
//     m_sync = NULL;
}
#undef TIMEOUT

/* partial template
template <class ID3D11Buffer*>
void DXBuffer<1>::SwapBuffers()
{}

template <class ID3D11Buffer*>
void DXBuffer<2>::SwapBuffers()
{
    m_currentBuffer = m_currentBuffer ? 0 : 1;
}*/

template < int bufferCount>
void DXBuffer<bufferCount>::SwapBuffers()
{
    m_currentBuffer = (m_currentBuffer+1)%m_bufferCount;
}