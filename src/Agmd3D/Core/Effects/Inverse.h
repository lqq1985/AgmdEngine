/*
============================================================================
Agmd3D - 3D Engine
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

#ifndef _INVERSE_H_
#define _INVERSE_H_

#include <Core/Effects/PostEffect.h>

namespace Agmd
{
    class AGMD3D_EXPORT Inverse : public PostEffect
    {
    public:
        Inverse();

        virtual void Init();
        virtual void Update(a_uint64 /*t_diff*/);
        virtual void ApplyEffect(Texture& input, Texture& output);
    private:
    };
}
#endif /* _INVERSE_H_ */
