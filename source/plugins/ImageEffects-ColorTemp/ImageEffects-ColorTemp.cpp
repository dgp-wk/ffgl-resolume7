#include "ImageEffects-ColorTemp.h"
#include "ShaderCode.h"
using namespace ffglex;

#define SATURATION_FUDGE -0.3f
#define GAMMA_FUDGE -0.04497f

enum ParamType : FFUInt32
{
	PT_COLOR_TEMP
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< ImageEffects >,                       // Create method
	"IE02",                      // Plugin unique ID of maximum length 4.
	",Color Temp",				// Plugin name
	2,                           // API major version number
	1,                           // API minor version number
	1,                           // Plugin major version number
	0,                           // Plugin minor version number
	FF_EFFECT,                   // Plugin type
	"Color Temperature",     // Plugin description
	"Sets the whitepoint according to the Kelvin temperature scale"      // About
);



ImageEffects::ImageEffects() :
	colorTemperature( 6500.0f ), 
	exposure( 0.0f ), 
	brightness( 0.0f ), 
	contrast( 0.0f ), 
	saturation( 0.0f ), 
	gamma( 0.0f ),
	levelHigh( 0.0f ), 
	levelMid( 0.0f ), 
	levelLow( 0.0f ), 
	highThresold( 0.0f ), 
	lowThresold( 0.0f ), 
	red( 0.0f ), 
	green( 0.0f ), 
	blue( 0.0f ),
	_fixCount(0)
{
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	SetParamInfo( PT_COLOR_TEMP,	"Color Temperature",	FF_TYPE_STANDARD, 6500.0f );

	SetParamRange( PT_COLOR_TEMP, 2000.0f, 12000.0f );

	FFGLLog::LogToHost( "Created ImageEffects-Color Temperature effect" );
}
ImageEffects::~ImageEffects()
{
}

unsigned int ImageEffects::Connect()
{
	return FF_SUCCESS;
}

FFResult ImageEffects::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( _vertexShaderCode, _fragmentShaderCode ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !quad.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

	//Use base-class init as success result so that it retains the viewport.
	return CFFGLPlugin::InitGL( vp );
}
FFResult ImageEffects::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( pGL->numInputTextures < 1 )
		return FF_FAIL;

	if( pGL->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//The shader's sampler is always bound to sampler index 0 so that's where we need to bind the texture.
	//Again, we're using the scoped bindings to help us keep the context in a default state.
	ScopedSamplerActivation activateSampler( 0 );
	Scoped2DTextureBinding textureBinding( pGL->inputTextures[ 0 ]->Handle );

	shader.Set( "InputTexture", 0 );

	//The input texture's dimension might change each frame and so might the content area.
	//We're adopting the texture's maxUV using a uniform because that way we dont have to update our vertex buffer each frame.
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *pGL->inputTextures[ 0 ] );
	shader.Set( "MaxUV", maxCoords.s, maxCoords.t );

	glUniform3f( shader.FindUniform( "Uniform2" ), gamma + GAMMA_FUDGE, saturation + SATURATION_FUDGE, colorTemperature );

	quad.Draw();

	return FF_SUCCESS;
}
FFResult ImageEffects::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();

	return FF_SUCCESS;
}

char* ImageEffects::GetParameterDisplay( unsigned int index )
{
	/**
	 * We're not returning ownership over the string we return, so we have to somehow guarantee that
	 * the lifetime of the returned string encompasses the usage of that string by the host. Having this static
	 * buffer here keeps previously returned display string alive until this function is called again.
	 * This happens to be long enough for the hosts we know about.
	 */
	static char displayValueBuffer[ 15 ];
	memset( displayValueBuffer, 0, sizeof( displayValueBuffer ) );

	switch( index )
	{
	case PT_COLOR_TEMP:
		sprintf( displayValueBuffer, "%f", colorTemperature );
		return displayValueBuffer;

	default:
		return CFFGLPlugin::GetParameterDisplay( index );
	}
}

FFResult ImageEffects::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PT_COLOR_TEMP:
		if( value < 2000.0f )
			value = 6500.0f;
		if( value == 2000.0f && _fixCount == 0 )
		{
			_fixCount += 1;
			value = 6500.0f;
		}

		colorTemperature = floor(value/100) * 100;
		break;


	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float ImageEffects::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
		case PT_COLOR_TEMP:
			return colorTemperature;
	}

	return 0.0f;
}
