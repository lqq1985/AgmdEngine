#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include <Config\Fwd.h>
#include <Core\Shader/BaseShaderProgram.h>
#include <Core/Buffer/BaseBuffer.h>
#include <Renderer\OpenGL\GlShader.h>
#include <Core\Enums.h>
#include <AgmdDefines.h>
#include <string>
#include <map>
#include <vector>

namespace Agmd
{
	class GLShaderProgram : public BaseShaderProgram
	{
	public:

		enum ParameterType
		{
			PARAMETER_NOFOUND,
			PARAMETER_UNIFORM,
			PARAMETER_ATTRIBUT,
			PARAMETER_UNIFORM_BLOCK
		};

		struct Parameter
		{
			uint32 loc;
			uint32 size;
			uint32 typeValue;
			ParameterType type;
		};

		typedef std::map<std::string,Parameter> ParameterMap; 
		typedef std::vector<Parameter> APIMatrix;

		GLShaderProgram(uint32 id);
		virtual ~GLShaderProgram();

		virtual void SetParameter(std::string name, float value) const;
		virtual void SetParameter(std::string name, vec2 value) const;
		virtual void SetParameter(std::string name, vec3 value) const;
		virtual void SetParameter(std::string name, vec4 value) const;

		virtual void SetParameter(std::string name, int value) const;
		virtual void SetParameter(std::string name, ivec2 value) const;
		virtual void SetParameter(std::string name, ivec3 value) const;
		virtual void SetParameter(std::string name, ivec4 value) const;

		virtual void SetParameter(std::string name, mat2 value) const;
		virtual void SetParameter(std::string name, mat3 value) const;
		virtual void SetParameter(std::string name, mat4 value) const;

		virtual void SetParameter(std::string name,	float* value, uint32 count) const;
		virtual void SetParameter(std::string name,	vec2* value, uint32 count) const;
		virtual void SetParameter(std::string name,	vec3* value, uint32 count) const;
		virtual void SetParameter(std::string name,	vec4* value, uint32 count) const;
		virtual void SetParameter(std::string name, mat4* value, uint32 count) const;

		virtual void SetParameter(TMatrixType type,mat4 value) const;

		virtual void SetParameter(std::string name, const BaseBuffer* buf) const;

		virtual void Use(bool) const;
	private:
		void SetupShader();
		Parameter GetParameter(std::string name) const;

		ParameterMap m_UniformMap;
		ParameterMap m_AttributMap;
		Parameter    m_APIMatrix[MAX_APIMATRIX];
		uint32 m_id;
	};
}

#endif