#ifndef SHADERPOOL_HPP
#define SHADERPOOL_HPP

#include <map>
#include <string>

class Shader;

class ShaderPool
{
private:
	// key is (vertex + shader filename), shader is value
	std::map<std::string, Shader *> m_shaders;

	ShaderPool();
	static ShaderPool *m_instance;

public:
	static ShaderPool *instance();

	/**
	 * Get the shader.
	 *
	 * @param vertex shader.
	 * @param fragment shader.
	 * @return name
	 */
	Shader *get(const std::string &vertex, const std::string &fragment);

};

#endif // SHADERPOOL_HPP
