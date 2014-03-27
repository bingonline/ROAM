#include "gfx/shader.hpp"

#include <string.h>
#include <iostream>
#include <iterator>
#include <fstream>

Shader::Shader(const std::string &vertex, const std::string &fragment)
	: m_vertex(vertex)
	, m_fragment(fragment)
{
}

int Shader::init()
{
	m_handle = glCreateProgram();
	if ((m_vertexHandle = loadShader(m_vertex, GL_VERTEX_SHADER)) == -1) {
		return -1;
	}
	if ((m_fragmentHandle = loadShader(m_fragment, GL_FRAGMENT_SHADER)) == -1) {
		return -1;
	}

	glAttachShader(m_handle, m_vertexHandle);
	glAttachShader(m_handle, m_fragmentHandle);

	// force attribute locations
	// http://stackoverflow.com/questions/4635913/explicit-vs-automatic-attribute-location-binding-for-opengl-shaders/4638906#4638906
	glBindAttribLocation(m_handle, 0, "position");
	glBindAttribLocation(m_handle, 1, "color");
	glBindAttribLocation(m_handle, 2, "normalTexel");

	glLinkProgram(m_handle);

	// query the link status
	GLint linked = 0, len = 0, len2 = 0;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &linked);
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &len);

	GLchar *log = new GLchar[len];
	memset(log, 0, len);
	glGetProgramInfoLog(m_handle, len, &len, log);

	if (!linked) {
		printf("linking FAILED.\n");
	} else {
		printf("linking SUCCESS.\n");
	}

	printf("received %d (%d) chars of loginfo:\n\t%s\n", len, len2, log);
	delete [] log;

	if (!linked) {
		return -1;
	}
	
	return 0;
}

Shader::~Shader()
{
	disable();

	// detach shaders.
	glDetachShader(m_handle, m_vertexHandle);
	glDetachShader(m_handle, m_fragmentHandle);

	// delete shaders and program
	glDeleteShader(m_vertexHandle);
	glDeleteShader(m_fragmentHandle);
	glDeleteShader(m_handle);
}

GLuint Shader::loadShader(const std::string &path, GLenum shaderType)
{
	std::string type;
	switch (shaderType) {
	case GL_VERTEX_SHADER: type = "GL_VERTEX_SHADER"; break;
	case GL_FRAGMENT_SHADER: type = "GL_FRAGMENT_SHADER"; break;
	}

	std::string contents;

	if (path == "data/shaders/basic-fs.glsl") {
		contents = "void main()\n{\n\tgl_FragColor = gl_Color;\n}";
	} else if (path == "data/shaders/basic-vs.glsl") {
		contents = "void main()\n{\ngl_TexCoord[0] = gl_MultiTexCoord0;\ngl_FrontColor = gl_Color;\ngl_Position = ftransform();\n}";
	} else if (path == "data/shaders/textured-fs.glsl") {
		contents = "uniform sampler2D tex;\n"
		           "void main()\n"
		           "{\n"
		           "\tvec4 texel = texture2D(tex, gl_TexCoord[0].st);\n"
		           "\tgl_FragColor = gl_Color * texel;\n"
		           "}\n";
	} else {
		printf("attempting to load %s form %s.\n", type.c_str(), path.c_str());

		std::ifstream ifs(path.c_str(), std::ifstream::in);
		if (ifs.fail()) {
			printf("Foiled reading file: %s\n", path.c_str());
			return -1;
		}

		std::string file_c((std::istreambuf_iterator<char>(ifs)),
		                   (std::istreambuf_iterator<char>()));
		contents = file_c;
		ifs.close();
	}
	const GLchar* ccontents[1] = { (const GLchar *) contents.c_str() };

	GLuint handle;
	handle = glCreateShader(shaderType);

	glShaderSource(handle, 1, ccontents, NULL);
	glCompileShader(handle);

	GLint compiled = 0, len = 0, len2 = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

	GLchar *log = new GLchar[len];
	memset(log, 0, len);
	glGetShaderInfoLog(handle, len, &len2, log);

	if (!compiled) {
		printf("compilation of %s FAILED.\n", path.c_str());
	} else {
		printf("compilation of %s SUCCESS.\n", path.c_str());
	}

	printf("received %d (%d) of loginfo:\n\t%s\n", len, len2, log);

	if (!compiled) {
		return -1;
	}

	return handle;
}
