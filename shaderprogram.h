#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include "shader.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram( std::string cspath );
	ShaderProgram( std::string vspath, std::string fspath );
	ShaderProgram( std::string vspath, std::string gspath, std::string fspath );
	~ShaderProgram();

public:
	void Create( std::string cspath );
	void Create( std::string vspath, std::string fspath );
	void Create( std::string vspath, std::string gspath, std::string fspath );
	void Delete();
	void Link();
	void Validate();
	void Reload();
	void Use();

public:
	int GetStatus( GLenum ) const;
	int GetDeleteStatus() const;
	int GetLinkStatus() const;
	int GetValidateStatus() const;

	int GetNumber( GLenum ) const;
	int GetNumAttachedShaders() const;
	int GetNumActiveAttributes() const;
	int GetNumActiveUniforms() const;
	int GetActiveAttributeMaxLength() const;
	int GetActiveUniformMaxLength() const;

	std::string GetInfoLog( void ) const;

	GLuint GetID() { return ID; }

public:
	GLint getUniformLocation( const GLchar* name ) const
	{
		return glGetUniformLocation( ID, name );
	}

	//@{
	/**
	Sets {1|2|3|4}-{unsigned integer|integer|float|double} uniform values by {name|location}. It converts double to float.
	*@param name Name of uniform variable.
	*@param location Location handle of uniform variable.
	*@param a First value.
	*@param b Second value.
	*@param c Third value.
	*@param d Fourth value.
	**/
	void SetUniform( GLint location, GLuint a, GLuint b, GLuint c, GLuint d );
	void SetUniform( GLint location, GLuint a, GLuint b, GLuint c );
	void SetUniform( GLint location, GLuint a, GLuint b );
	void SetUniform( GLint location, GLuint a );
	void SetUniform( const GLchar* name, GLuint a, GLuint b, GLuint c, GLuint d );
	void SetUniform( const GLchar* name, GLuint a, GLuint b, GLuint c );
	void SetUniform( const GLchar* name, GLuint a, GLuint b );
	void SetUniform( const GLchar* name, GLuint a );
	void SetUniform( GLint location, GLint a, GLint b, GLint c, GLint d );
	void SetUniform( GLint location, GLint a, GLint b, GLint c );
	void SetUniform( GLint location, GLint a, GLint b );
	void SetUniform( GLint location, GLint a );
	void SetUniform( const GLchar* name, GLint a, GLint b, GLint c, GLint d );
	void SetUniform( const GLchar* name, GLint a, GLint b, GLint c );
	void SetUniform( const GLchar* name, GLint a, GLint b );
	void SetUniform( const GLchar* name, GLint a );
	void SetUniform( GLint location, GLfloat a, GLfloat b, GLfloat c, GLfloat d );
	void SetUniform( GLint location, GLfloat a, GLfloat b, GLfloat c );
	void SetUniform( GLint location, GLfloat a, GLfloat b );
	void SetUniform( GLint location, GLfloat a );
	void SetUniform( const GLchar* name, GLfloat a, GLfloat b, GLfloat c, GLfloat d );
	void SetUniform( const GLchar* name, GLfloat a, GLfloat b, GLfloat c );
	void SetUniform( const GLchar* name, GLfloat a, GLfloat b );
	void SetUniform( const GLchar* name, GLfloat a );
	void SetUniform( GLint location, GLdouble a, GLdouble b, GLdouble c, GLdouble d );
	void SetUniform( GLint location, GLdouble a, GLdouble b, GLdouble c );
	void SetUniform( GLint location, GLdouble a, GLdouble b );
	void SetUniform( GLint location, GLdouble a );
	void SetUniform( const GLchar* name, GLdouble a, GLdouble b, GLdouble c, GLdouble d );
	void SetUniform( const GLchar* name, GLdouble a, GLdouble b, GLdouble c );
	void SetUniform( const GLchar* name, GLdouble a, GLdouble b );
	void SetUniform( const GLchar* name, GLdouble a );
	//@}

	//@{
	/**
	Sets a {integer|float} uniform vector value by {name|location}.
	*@param name Name of uniform variable.
	*@param location  Location handle of uniform variable
	*@param v  Vector value.
	*@param nvalues  Number of values in the vector v.
	*@param count  Number of elements of the uniform vector array to be modified.
	*       a count of 1 should be used if modifying the value of a single vector. 
	**/
	void SetUniform( const GLchar* name, const GLint* v, GLuint nvalues, GLsizei count = 1 );
	void SetUniform( const GLchar* name, const GLfloat* v, GLuint nvalues, GLsizei count = 1 );
	void SetUniform( GLint location, const GLint* v, GLuint nvalues, GLsizei count = 1 );
	void SetUniform( GLint location, const GLfloat* v, GLuint nvalues, GLsizei count = 1 );
	//@}

	//@{
	/**
	Sets a uniform matrix value by {name|location}.
	*@param name  Name of uniform variable.
	*@param location  Location handle of uniform variable.
	*@param m  Matrix value with dim^2 values
	*@param dim  Dimension of the matrix m.
	*@param transpose If transpose is GL_FALSE, each matrix is assumed to be supplied in
	*					in column major order, otherwise is in row major order.
	*@param count  Number of elements of the uniform matrix array to be modified.
	*				a count of 1 should be used if modifying the value of a single matrix.
	**/
	void SetUniform( const GLchar* name, const GLfloat* m, GLuint dim, GLboolean transpose = GL_FALSE, GLsizei count = 1 );
	void SetUniform( GLint location, const GLfloat* m, GLuint dim, GLboolean transpose = GL_FALSE, GLsizei count = 1 );
	//@}

	GLuint ID;
private:
	Shader vertexShader, geometryShader, fragmentShader, computeShader;
};
