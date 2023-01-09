#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

class Shader
{
public:
	Shader();
	Shader( std::string shaderPath, GLenum shaderType );
	~Shader();

public:
	void Create( std::string shaderPath, GLenum shaderType );
	void Delete();
	void Load();

public:
	int GetStatus( GLenum ) const;
	int GetDeleteStatus() const;
	int GetCompileStatus() const;

	std::string GetInfoLog() const;
	std::string GetSource() const;

	GLuint      GetID()   const { return ID;   }
	GLenum      GetType() const { return Type; }
	std::string GetPath() const { return Path; }

private:
	GLuint ID;
	GLenum Type;
	std::string Path;
};
