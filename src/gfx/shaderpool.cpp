#include "gfx/shaderpool.hpp"
#include "gfx/shader.hpp"

ShaderPool *ShaderPool::m_instance = NULL;

ShaderPool::ShaderPool()
{
}

ShaderPool *ShaderPool::instance()
{
	if (m_instance == NULL) {
		m_instance = new ShaderPool;
	}

	return m_instance;
}

Shader *ShaderPool::get(const std::string &vertex, const std::string &fragment)
{
	if (m_shaders.find(vertex + fragment) == m_shaders.end()) {
		Shader *tmp = new Shader(vertex, fragment);
		if (tmp->init() != 0) {
			return NULL;
		}
		m_shaders[vertex + fragment] = tmp;
	}

	return m_shaders[vertex + fragment];
}
