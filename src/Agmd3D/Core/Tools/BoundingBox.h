/*
============================================================================
Agmd3D - 3D Engine
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <Vector3.h>
#include <Config/Export.h>
#include <Container/Vector.h>
#include <Core/Tools/BoundingSphere.h>

using namespace AgmdMaths;

namespace Agmd
{
    class AGMD3D_EXPORT BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(const a_vector<vec3>& vertex_pos);
        BoundingBox(const vec3&, const vec3&);
        BoundingBox(const BoundingBox& bbox);


        vec3 GetMin() const;
        vec3 GetMax() const;
        vec3 GetCenter() const;

        const BoundingBox getTransformedBoundingBox(const mat4& mat) const;
        void mult(const mat4&, const BoundingBox& bbox);
        BoundingBox operator/(float value) const;
        BoundingBox operator*(float value) const;
        BoundingBox& operator+=(vec3 translate);
        bool Within(BoundingSphere& bsphere);
    private:
        vec3 m_max;
        vec3 m_min;
    };
}
#endif /* _BOUNDINGBOX_H_ */
