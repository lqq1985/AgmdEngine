/*
============================================================================
Agmd3D - 3D Engine
Author : Cyril Basset (basset.cyril@gmail.com - https://github.com/Agamand)
https://github.com/Agamand/AgmdEngine
============================================================================
*/

#include <Core/SceneNode/SceneNode.h>
#include <Core/SceneObject/Material.h>


namespace Agmd
{

    SceneNode::SceneNode(NodeType type,Transform* transform) : m_type(type), m_parent(NULL)
    {
		if(!transform)
			m_transform = new Transform();
		else m_transform = transform;
	}

    SceneNode::~SceneNode()
    {}

    Transform& SceneNode::GetTransform() 
    { 
        return *m_transform;
    }

	bool SceneNode::Update( Transform* transform, bool updateChildren, bool transformChanged )
	{
		bool transformUpdate = transformChanged || m_transform->needUpdate();
		if(transformUpdate)
			m_transform->Update(transform);
		if(updateChildren && !m_children.empty())
		{
			for(a_uint32 i = 0,len = m_children.size(); i < len; i++)
				m_children[i]->Update(m_transform,updateChildren,transformUpdate);
		}
		return transformUpdate;
	}

	bool SceneNode::isEmpty()
	{
		return m_children.empty();
	}


	void SceneNode::clear()
	{
		m_children.clear();
	}

}

