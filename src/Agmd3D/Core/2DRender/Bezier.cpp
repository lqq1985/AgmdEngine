#include <Core/2DRender/Bezier.h>
#include <Container/Vector.h>

namespace Agmd
{
    vec3 dc(a_vector<a_vector<vec3>>& points, const a_vector<vec3>& points_control, float t)
    {
        for (int j = 1; j < (int)points_control.size(); j++)
        {
            for (int i = 0; i < (int)points_control.size() - j; i++)
            {
                points[j][i] = points[j - 1][i] * (1 - t) + points[j - 1][i + 1] * t;
            }
        }
        return points[points_control.size() - 1][0];
    }

    vec3 dc2(vec3* points, const a_vector<vec3>& points_control, float t)
    {
        for (int j = 1; j < (int)points_control.size(); j++)
        {
            for (int i = 0; i < (int)points_control.size() - j; i++)
            {
                points[j * points_control.size() + i] = points[(j - 1) * points_control.size() + i] * (1 - t) + points[(j - 1) * points_control.size() + i + 1] * t;
            }
        }
        return points[(points_control.size() - 1) * points_control.size()];
    }

    void Bezier::compute(int pointIndex /*= -1*/)
    {
        a_vector<a_vector<vec3>> points;
        vec3* _points = new vec3[m_computedPoints.size() * m_computedPoints.size()];
        m_computedPoints.clear();
        float zero = 0.0f;
        int* value = (int*)&zero;
        memcpy(_points, &m_controlPoints[0], m_controlPoints.size() * sizeof(vec3));
        memset(_points + m_computedPoints.size(), *value, m_computedPoints.size() * (m_computedPoints.size() - 1));
        /*for(int i = 0; i < m_controlPoints.size(); i++)
        {
            //points.push_back(std::vector<vec3>());
            //points[i].resize(m_controlPoints.size(),vec3());
        }*/

        /*for(int i = 0; i < m_controlPoints.size();i++)
            points[0][i] = m_controlPoints[i];*/

        int m = 10 * m_controlPoints.size();
        float p = 1.0f / m;

        m_computedPoints.push_back(dc2(_points, m_controlPoints, 0));
        for (int i = 1; i <= m; i++)
        {
            m_computedPoints.push_back(dc2(_points, m_controlPoints, i * p));
        }
        delete _points;
    }
}
