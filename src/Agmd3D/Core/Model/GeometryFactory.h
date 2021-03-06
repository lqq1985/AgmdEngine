#ifndef _GEOMETRYFACTORY_H_
#define _GEOMETRYFACTORY_H_

#include <Core/2DRender/BaseSpline.h>
#include <Core/Model/Model.h>

namespace Agmd
{
    class AGMD3D_EXPORT GeometryFactory
    {
    public:
        static void lathe(BaseSpline* p1, BaseSpline* p2, a_vector<Model::TVertex>& vertices, a_vector<Model::TIndex>& index, int splice);
        static void simpleExtrusion(BaseSpline* p1,a_vector<Model::TVertex>& vertices, a_vector<Model::TIndex>& index, int slice, float scalemin, float scalemax, float height);
        static void generalizedExtrusion(BaseSpline* p1, BaseSpline* p2, a_vector<Model::TVertex>& vertices,a_vector<Model::TIndex>& index);
        static void createPlane(vec3 orientation, quat rot, int size, int offset_index, a_vector<Model::TVertex>& vertices, a_vector<Model::TIndex>& index);
        static Model* createPlane(ivec2 size, ivec2 n_poly);
        static Model* createSphere(float r, float stack, float slice, float angle);
        static Model* createBox(vec3 size);
        static void createBox(a_vector<vec3>& vert, a_vector<a_uint16>& index, vec3 size);
        static Model* createMetaSphere(float r, int stack, int slice);
        static void jarvis(a_vector<vec2>& points,a_vector<vec2>& poly_out);
        static void delaunay(a_vector<vec2>& points,a_vector<a_uint16>& triangles);
        static void voronoi(a_vector<vec2>& points,a_vector<vec2>& out_points, a_vector<a_uint16>& triangles);
        static void BBox(a_vector<vec3>& vertices,a_vector<vec3> box);
        static void Indexize(a_vector<vec3>& in, a_vector<vec3>& out,a_vector<a_uint16>& index);
        static void loopSubdivize(a_vector<vec3>& vert, a_vector<a_uint16>& index,a_vector<vec3>& out_vert, a_vector<a_uint16>& out_index);
        static void createIndexedBox(a_vector<vec3>& vert, a_vector<a_uint16>& indices, vec3 size);
    };
}

#endif /* _GEOMETRYFACTORY_H_ */
