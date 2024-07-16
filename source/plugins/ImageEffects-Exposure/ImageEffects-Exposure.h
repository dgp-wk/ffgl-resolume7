#pragma once
#include <string>
#include <FFGLSDK.h>

class ImageEffects : public CFFGLPlugin
{
public:
	ImageEffects();
	~ImageEffects();

	//CFFGLPlugin
	FFResult	InitGL( const FFGLViewportStruct* vp ) override;
	FFResult	ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult	DeInitGL() override;

	char*		GetParameterDisplay( unsigned int index ) override;
	FFResult	SetFloatParameter( unsigned int dwIndex, float value ) override;
	float		GetFloatParameter( unsigned int index ) override;

	unsigned int Connect() override;

private:
	ffglex::FFGLShader shader;  //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad;//!< Utility to help us render a full screen quad.
	
	// Image Effects
	float exposure, power;
};
