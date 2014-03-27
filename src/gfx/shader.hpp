#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

class Shader
{
private:
	// program handle, unique id.
	GLuint m_handle;

	// fragment and vertex handles, single program must have both.
	GLuint m_vertexHandle;
	GLuint m_fragmentHandle;

	// file paths
	std::string m_vertex, m_fragment;

public:
	/**
	 * Consturctor.
	 *
	 * @param path to the vertex shader 
	 * @param path to the fragment shader 
	 */
	Shader(const std::string &vertex, const std::string &fragment);
	~Shader();

	/**
	 * Initializexs the shader.
	 *
	 * Tries to load the resources and complie them on the GPU.
	 *
	 * @return on success return 0, on failure != 0
	 */
	int init();

	virtual void enable() const;
	static void disable();

	GLuint getHandle() const;
	GLuint getVertexHandle() const;
	GLuint getFragmentHandle() const;

	/**
	 * Query the uniform location from the shaders.
	 *
	 * just returns the value from glGetUniformLocation.
	 *
	 * @param name
	 * @return -1 if not found, else id.
	 */
	GLint getUniformLocation(const GLchar *name) const;

	int loadShaders();
	int loadVertexShader();
	int loadFragmentShader();

protected:
	/**
	 * Loads a single shader file
	 *
	 * @param path to the shader.
	 * @param shaderType - GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	 *
	 * @return handle to the loaded shader.
	 */
	GLuint loadShader(const std::string &path, GLenum shaderType);

};

inline void Shader::enable() const
{
	glUseProgram(m_handle);
}

inline void Shader::disable()
{
	glUseProgram(0);
}

inline GLuint Shader::getHandle() const
{
	return m_handle;
}

inline GLuint Shader::getVertexHandle() const
{
	return m_vertexHandle;
}

inline GLuint Shader::getFragmentHandle() const
{
	return m_fragmentHandle;
}

inline GLint Shader::getUniformLocation(const GLchar *name) const
{
	return glGetUniformLocation(m_handle, name);
}

#endif // SHADER_HPP
