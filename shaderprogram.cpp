#include "shaderprogram.h"
#include <iostream>

/*=================================================================================================
  CONSTRUCTORS
=================================================================================================*/

ShaderProgram::ShaderProgram()
{
	ID = 0;
}

ShaderProgram::ShaderProgram( std::string cspath )
{
	Create( cspath );
}

ShaderProgram::ShaderProgram( std::string vspath, std::string fspath )
{
	Create( vspath, fspath );
}

ShaderProgram::ShaderProgram( std::string vspath, std::string gspath, std::string fspath )
{
	Create( vspath, gspath, fspath );
}

/*=================================================================================================
  DESTRUCTOR
=================================================================================================*/

ShaderProgram::~ShaderProgram()
{
	Delete();
}

/*=================================================================================================
  CREATE
=================================================================================================*/

void ShaderProgram::Create( std::string cspath )
{
	ID = glCreateProgram();

	if( ID != 0 )
	{
		computeShader.Create( cspath, GL_COMPUTE_SHADER );
		glAttachShader( ID, computeShader.GetID() );

		Link();
	}
}

void ShaderProgram::Create( std::string vspath, std::string fspath )
{
	ID = glCreateProgram();

	if( ID != 0 )
	{
		vertexShader.Create( vspath, GL_VERTEX_SHADER );
		glAttachShader( ID, vertexShader.GetID() );

		fragmentShader.Create( fspath, GL_FRAGMENT_SHADER );
		glAttachShader( ID, fragmentShader.GetID() );

		Link();
	}
}

void ShaderProgram::Create( std::string vspath, std::string gspath, std::string fspath )
{
	ID = glCreateProgram();

	if( ID != 0 )
	{
		vertexShader.Create( vspath, GL_VERTEX_SHADER );
		glAttachShader( ID, vertexShader.GetID() );

		geometryShader.Create( gspath, GL_GEOMETRY_SHADER );
		glAttachShader( ID, geometryShader.GetID() );

		fragmentShader.Create( fspath, GL_FRAGMENT_SHADER );
		glAttachShader( ID, fragmentShader.GetID() );

		Link();
	}
}

/*=================================================================================================
  DELETE
=================================================================================================*/

void ShaderProgram::Delete( void )
{
	if( ID != 0 )
	{
		glDetachShader( ID, vertexShader.GetID() );
		glDetachShader( ID, geometryShader.GetID() );
		glDetachShader( ID, fragmentShader.GetID() );
		glDetachShader( ID, computeShader.GetID() );

		glDeleteProgram( ID );

		ID = 0;
	}
}

/*=================================================================================================
  LINK
=================================================================================================*/

void ShaderProgram::Link( void )
{
	glLinkProgram( ID );

	// If the program didn't link successfully, print log
	if( GetLinkStatus() == 0 )
		std::cerr << "shader program " << ID << " link log" << std::endl << GetInfoLog() << std::endl;
}

/*=================================================================================================
  VALIDATE
=================================================================================================*/

void ShaderProgram::Validate( void )
{
	glValidateProgram( ID );

	// If the program didn't validate successfully, print log
	if( GetValidateStatus() == 0 )
		std::cerr << "shader program " << ID << " validate log" << std::endl << GetInfoLog() << std::endl;
}

/*=================================================================================================
  RELOAD
=================================================================================================*/

void ShaderProgram::Reload( void )
{
	vertexShader.Load();
	geometryShader.Load();
	fragmentShader.Load();
	computeShader.Load();
	Link();
}

/*=================================================================================================
  USE
=================================================================================================*/

void ShaderProgram::Use( void )
{
	glUseProgram( ID );
}

/*=================================================================================================
  GET STATUS
=================================================================================================*/

//-1: invalid/uninitialized program
int ShaderProgram::GetStatus( GLenum en ) const
{
	if( ID == 0 )
		return -1;

	GLint status;
	glGetProgramiv( ID, en, &status );

	return status == GL_TRUE ? 1 : 0;
}

// 0: program is currently not flagged for deletion
// 1: program is flagged for deletion
int ShaderProgram::GetDeleteStatus( void ) const {
	return GetStatus( GL_DELETE_STATUS );
}

// 0: program did not link successfully
// 1: program linked without errors
int ShaderProgram::GetLinkStatus( void ) const {
	return GetStatus( GL_LINK_STATUS );
}

// 0: program did not validate
// 1: program validated
int ShaderProgram::GetValidateStatus( void ) const {
	return GetStatus( GL_VALIDATE_STATUS );
}

/*=================================================================================================
  GET NUMBER
=================================================================================================*/

//-1: invalid/uninitialized program
int ShaderProgram::GetNumber( GLenum en ) const
{
	if( ID == 0 )
		return -1;

	GLint num;
	glGetProgramiv( ID, en, &num );

	return num;
}

int ShaderProgram::GetNumAttachedShaders( void ) const {
	return GetNumber( GL_ATTACHED_SHADERS );
}

int ShaderProgram::GetNumActiveAttributes( void ) const {
	return GetNumber( GL_ACTIVE_ATTRIBUTES );
}

int ShaderProgram::GetNumActiveUniforms( void ) const {
	return GetNumber( GL_ACTIVE_UNIFORMS );
}

int ShaderProgram::GetActiveAttributeMaxLength( void ) const {
	return GetNumber( GL_ACTIVE_ATTRIBUTE_MAX_LENGTH );
}

int ShaderProgram::GetActiveUniformMaxLength( void ) const {
	return GetNumber( GL_ACTIVE_UNIFORM_MAX_LENGTH );
}

/*=================================================================================================
  GET INFO LOG
=================================================================================================*/

std::string ShaderProgram::GetInfoLog( void ) const
{
	if( ID == 0 )
		return "";

	GLint logLength = 0;
	GLsizei charsWritten = 0;
	GLchar* infoLog;
	std::string stringLog = "";

	glGetProgramiv( ID, GL_INFO_LOG_LENGTH, &logLength );

	if( logLength > 0 )
	{
		infoLog = (GLchar*)malloc( sizeof(GLchar) * logLength );
		glGetProgramInfoLog( ID, (GLsizei)logLength, &charsWritten, infoLog );
		stringLog = infoLog;
		free( infoLog );
	}

	return stringLog;
}

/*=================================================================================================
  UNIFORM SETTERS
=================================================================================================*/

//Setting uniforms by value:
//Unsigned Integer & Location:
void ShaderProgram::SetUniform( GLint location, GLuint a, GLuint b, GLuint c, GLuint d ) {
	glUniform4ui( location, a, b, c, d );
}
void ShaderProgram::SetUniform( GLint location, GLuint a, GLuint b, GLuint c ) {
	glUniform3ui( location, a, b, c );
}
void ShaderProgram::SetUniform( GLint location, GLuint a, GLuint b ) {
	glUniform2ui( location, a, b );
}
void ShaderProgram::SetUniform( GLint location, GLuint a ) {
	glUniform1ui( location, a );
}
//Unsigned Integer & Name:
void ShaderProgram::SetUniform( const GLchar* name, GLuint a, GLuint b, GLuint c, GLuint d ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c, d );
}
void ShaderProgram::SetUniform( const GLchar* name, GLuint a, GLuint b, GLuint c ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c );
}
void ShaderProgram::SetUniform( const GLchar* name, GLuint a, GLuint b ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b );
}
void ShaderProgram::SetUniform( const GLchar* name, GLuint a ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a );
}

//Integer & Location:
void ShaderProgram::SetUniform( GLint location, GLint a, GLint b, GLint c, GLint d ) {
	glUniform4i( location, a, b, c, d );
}
void ShaderProgram::SetUniform( GLint location, GLint a, GLint b, GLint c ) {
	glUniform3i( location, a, b, c );
}
void ShaderProgram::SetUniform( GLint location, GLint a, GLint b ) {
	glUniform2i( location, a, b );
}
void ShaderProgram::SetUniform( GLint location, GLint a ) {
	glUniform1i( location, a );
}
//Integer & Name:
void ShaderProgram::SetUniform( const GLchar* name, GLint a, GLint b, GLint c, GLint d ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c, d );
}
void ShaderProgram::SetUniform( const GLchar* name, GLint a, GLint b, GLint c ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c );
}
void ShaderProgram::SetUniform( const GLchar* name, GLint a, GLint b ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b );
}
void ShaderProgram::SetUniform( const GLchar* name, GLint a ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a );
}

//Float & Location:
void ShaderProgram::SetUniform( GLint location, GLfloat a, GLfloat b, GLfloat c, GLfloat d ) {
	glUniform4f( location, a, b, c, d );
}
void ShaderProgram::SetUniform( GLint location, GLfloat a, GLfloat b, GLfloat c ) {
	glUniform3f( location, a, b, c );
}
void ShaderProgram::SetUniform( GLint location, GLfloat a, GLfloat b ) {
	glUniform2f( location, a, b );
}
void ShaderProgram::SetUniform( GLint location, GLfloat a ) {
	glUniform1f( location, a );
}
//Float & Name:
void ShaderProgram::SetUniform( const GLchar* name, GLfloat a, GLfloat b, GLfloat c, GLfloat d ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c, d );
}
void ShaderProgram::SetUniform( const GLchar* name, GLfloat a, GLfloat b, GLfloat c ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c );
}
void ShaderProgram::SetUniform( const GLchar* name, GLfloat a, GLfloat b ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b );
}
void ShaderProgram::SetUniform( const GLchar* name, GLfloat a ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a );
}

//Double & Location:
void ShaderProgram::SetUniform( GLint location, GLdouble a, GLdouble b, GLdouble c, GLdouble d ) {
	glUniform4f( location, (GLfloat)a, (GLfloat)b, (GLfloat)c, (GLfloat)d );
}
void ShaderProgram::SetUniform( GLint location, GLdouble a, GLdouble b, GLdouble c ) {
	glUniform3f( location, (GLfloat)a, (GLfloat)b, (GLfloat)c );
}
void ShaderProgram::SetUniform( GLint location, GLdouble a, GLdouble b ) {
	glUniform2f( location, (GLfloat)a, (GLfloat)b );
}
void ShaderProgram::SetUniform( GLint location, GLdouble a ) {
	glUniform1f( location, (GLfloat)a );
}
//Double & name:
void ShaderProgram::SetUniform( const GLchar* name, GLdouble a, GLdouble b, GLdouble c, GLdouble d ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c, d );
}
void ShaderProgram::SetUniform( const GLchar* name, GLdouble a, GLdouble b, GLdouble c ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b, c );
}
void ShaderProgram::SetUniform( const GLchar* name, GLdouble a, GLdouble b ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a, b );
}
void ShaderProgram::SetUniform( const GLchar* name, GLdouble a ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, a );
}

//Setting Uniforms by vector:
//Location & Integer:
void ShaderProgram::SetUniform( GLint location, const GLint* v, GLuint nvalues, GLsizei count ) {
	switch( nvalues ) {
		case 1: glUniform1iv( location, count, v ); break;
		case 2: glUniform2iv( location, count, v ); break;
		case 3: glUniform3iv( location, count, v ); break;
		case 4: glUniform4iv( location, count, v ); break;
	}
}
//Location & Float:
void ShaderProgram::SetUniform( GLint location, const GLfloat* v, GLuint nvalues, GLsizei count ) {
	switch( nvalues ) {
		case 1: glUniform1fv( location, count, v ); break;
		case 2: glUniform2fv( location, count, v ); break;
		case 3: glUniform3fv( location, count, v ); break;
		case 4: glUniform4fv( location, count, v ); break;
	}
}

//Name & Integer:
void ShaderProgram::SetUniform( const GLchar* name, const GLint* v, GLuint nvalues, GLsizei count ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, v, nvalues, count );
}
//Name & Float:
void ShaderProgram::SetUniform( const GLchar* name, const GLfloat* v, GLuint nvalues, GLsizei count ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, v, nvalues, count );
}

//Setting uniform matrix value:
//Location:
void ShaderProgram::SetUniform( GLint location, const GLfloat* m, GLuint dim, GLboolean transpose, GLsizei count ) {
	switch( dim ) {
		case 2: glUniformMatrix2fv( location, count, transpose, m ); break;
		case 3: glUniformMatrix3fv( location, count, transpose, m ); break;
		case 4: glUniformMatrix4fv( location, count, transpose, m ); break;
	}
}
//Name:
void ShaderProgram::SetUniform( const GLchar* name, const GLfloat* m, GLuint dim, GLboolean transpose, GLsizei count ) {
	GLint location = getUniformLocation( name );
	SetUniform( location, m, dim, transpose, count );
}
