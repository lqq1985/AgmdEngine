#ifndef SCENE_H
#define SCENE_H

#include <Config/Export.h>
#include <Config/Fwd.h>
#include <Core/Texture/Texture.h>
#include <Core/Shader/ShaderProgram.h>
#include <Core/SceneObject/Light.h>

#include <AgmdDefines.h>

#include <vector>

namespace Agmd
{
	enum SceneOption
	{
		SC_NOOPTION 		= 0x00,
		SC_DRAW_MODEL		= 0x01,
		SC_DRAW_TERRAIN		= 0x02,
		SC_DRAW_WATER		= 0x04,
		SC_DRAW_SKY			= 0x08,
		SC_APPLY_SHADOW		= 0x10,
		SC_APPLY_LIGHTING	= 0x20,
		SC_DRAW_ALL		= SC_DRAW_MODEL | SC_DRAW_TERRAIN | SC_DRAW_WATER | SC_DRAW_SKY,


	};

	typedef std::vector<Model*> vModel;
	typedef std::vector<Terrain*> vMap;
	typedef std::vector<Water*> vWater;

	class AGMD_EXPORT Scene
	{
	public:
		Scene();
		~Scene();
		void Render(TRenderPass pass) const;
        void Draw() const;

		void GenerateShadowMap();

		void RenderDiffusePass(uint32 flag) const;
		void RenderLightingPass(uint32 flag) const;
		void RenderShadowPass(uint32 flag) const;
		


		void Update(uint64 t_diff);

		void Prepare();

		void AddModel(Model*);
		void AddTerrain(Terrain*);
		void AddWater(Water*);

		void SetSky(Sky*);
		Sky* GetSky();

		void RemoveModel(Model*);
		void RemoveTerrain(Terrain*);
		void RemoveWater(Water*);
		
		Texture	getShadowMap();

        std::vector<Light*> GetLights();

		uint64 GetTime()
		{
			return m_deltaTime;
		}

	private:

		void RenderShadow() const;

		vModel	m_vModels;
		vMap	m_vMaps;
		vWater	m_vWaters;
		Sky*	m_Sky;
		uint64	m_deltaTime;

		FrameBuffer*	m_shadowmapping_fbo;
		RenderBuffer*	m_depth_rbo;
		Texture			m_shadowMap;
		Texture			m_debugMap;
		ShaderProgram	m_shadowShader;
		mat4			m_matShadow;
		vec3            m_light_dir;
		float			m_light_angle;
		FrameBuffer*	m_renderBuffer;
		std::vector<Light*> m_lights;
		Texture			m_renderTexture[4];
	};


}


#endif //SCENE_H