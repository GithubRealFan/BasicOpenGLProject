#include "shader.h"
#include <iostream>
#include <fstream>

/*=================================================================================================
  CONSTRUCTORS
=================================================================================================*/

Shader::Shader()
{
	ID = 0;
	Type = GL_INVALID_ENUM;
	Path = "";
}

Shader::Shader( std::string shaderPath, GLenum shaderType )
{
	Create( shaderPath, shaderType );
}

/*=================================================================================================
  DESTRUCTOR
=================================================================================================*/

Shader::~Shader()
{
	Delete();
}

/*=================================================================================================
  CREATE
=================================================================================================*/

void Shader::Create( std::string shaderPath, GLenum shaderType )
{
	ID = glCreateShader( shaderType );

	Type = shaderType;
	Path = shaderPath;

	Load();
}

/*=================================================================================================
  DELETE
=================================================================================================*/

void Shader::Delete( void )
{
	glDeleteShader( ID );

	ID = 0;
	Type = GL_INVALID_ENUM;
	Path = "";
}

/*=================================================================================================
  LOAD
=================================================================================================*/

void Shader::Load( void )
{
	if( ID == 0 )
		return;

	std::ifstream srcFile( Path );
	std::string shaderSrc, line;

	if( srcFile.is_open() == true )
	{
		while( std::getline( srcFile, line ) )
		{
			shaderSrc += line;
			shaderSrc += '\n';
		}
		srcFile.close();

		const char* src = shaderSrc.c_str();

		glShaderSource( ID, 1, &src, NULL );

		glCompileShader( ID );

		// If the shader didn't compile successfully, print log
		if( GetCompileStatus() == 0 )
			std::cerr << Path << std::endl << GetInfoLog() << std::endl;
	}
	else
		std::cerr << "Unable to open shader file: " << Path << std::endl;
}

/*=================================================================================================
  GET STATUS
=================================================================================================*/

//-1: invalid/uninitialized shader
int Shader::GetStatus( GLenum en ) const
{
	if( ID == 0 )
		return -1;

	GLint status;
	glGetShaderiv( ID, en, &status );

	return status == GL_TRUE ? 1 : 0;
}

// 0: shader is currently not flagged for deletion
//+1: shader is flagged for deletion
int Shader::GetDeleteStatus( void ) const {
	return GetStatus( GL_DELETE_STATUS );
}

// 0: shader did not compile successfully
//+1: shader compiled without errors
int Shader::GetCompileStatus( void ) const {
	return GetStatus( GL_COMPILE_STATUS );
}

/*=================================================================================================
  GET INFO LOG
=================================================================================================*/

std::string Shader::GetInfoLog( void ) const
{
	if( ID == 0 )
		return "";

	GLint logLength = 0;
	GLsizei charsWritten = 0;
	GLchar* infoLog;
	std::string stringLog = "";

	glGetShaderiv( ID, GL_INFO_LOG_LENGTH, &logLength );

	if( logLength > 0 )
	{
		infoLog = (GLchar*)malloc( sizeof(GLchar) * logLength );
		glGetShaderInfoLog( ID, (GLsizei)logLength, &charsWritten, infoLog );
		stringLog = infoLog;
		free( infoLog );
	}

	return stringLog;
}

/*=================================================================================================
  GET SOURCE (from GPU)
=================================================================================================*/

std::string Shader::GetSource( void ) const
{
	if( ID == 0 )
		return "";

	GLint bufSize = 0;
	GLsizei charsWritten = 0;
	GLchar* src;
	std::string stringSrc = "";

	glGetShaderiv( ID, GL_SHADER_SOURCE_LENGTH, &bufSize );

	if( bufSize > 0 )
	{
		src = (GLchar*)malloc( sizeof(GLchar) * bufSize );
		glGetShaderSource( ID, (GLsizei)bufSize, &charsWritten, src );
		stringSrc = src;
		free( src );
	}

	return stringSrc;
}
