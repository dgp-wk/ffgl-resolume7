#include "ImageEffects-Levels.h"
#include "ShaderCode.h"
using namespace ffglex;

#define SATURATION_FUDGE -0.3f
#define GAMMA_FUDGE -0.04497f

enum ParamType : FFUInt32
{
	PT_HIGHPOINT_GAIN,
	PT_MIDPOINT_GAIN,
	PT_LOWPOINT_GAIN,
	PT_HIGHPOINT_THRESHOLD,
	PT_LOWPOINT_THRESHOLD
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< ImageEffects >,                       // Create method
	"IE04",                      // Plugin unique ID of maximum length 4.
	",Levels",            // Plugin name
	2,                           // API major version number
	1,                           // API minor version number
	1,                           // Plugin major version number
	0,                           // Plugin minor version number
	FF_EFFECT,                   // Plugin type
	"Adjusts highs, mids, and lows.",     // Plugin description
	"Adjusts highs, mids, and lows."      // About
);



ImageEffects::ImageEffects() :
	levelHigh( 0.0f ), 
	levelMid( 0.0f ), 
	levelLow( 0.0f ), 
	highThresold( 0.0f ), 
	lowThresold( 0.0f )
{
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	SetParamInfo( PT_HIGHPOINT_GAIN,"Highs",				FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_MIDPOINT_GAIN, "Mids",					FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_LOWPOINT_GAIN, "Lows",					FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_HIGHPOINT_THRESHOLD, "Highpoint",		FF_TYPE_STANDARD, 0.666f );
	SetParamInfo( PT_LOWPOINT_THRESHOLD, "Lowpoint",		FF_TYPE_STANDARD, 0.333f );

	SetParamRange( PT_HIGHPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_MIDPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_LOWPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_HIGHPOINT_THRESHOLD, 0.5f, 1.0f );
	SetParamRange( PT_LOWPOINT_THRESHOLD, 0.0f, 0.5f );

	FFGLLog::LogToHost( "Created ImageEffects-Levels effect" );
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

	glUniform3f( shader.FindUniform( "Uniform1" ), levelHigh, levelMid, levelLow );
	glUniform3f( shader.FindUniform( "Uniform2" ), highThresold, lowThresold, 0.0f );

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
	case PT_HIGHPOINT_GAIN:
		sprintf( displayValueBuffer, "%f", levelHigh );
		return displayValueBuffer;
	case PT_MIDPOINT_GAIN:
		sprintf( displayValueBuffer, "%f", levelMid );
		return displayValueBuffer;
	case PT_LOWPOINT_GAIN:
		sprintf( displayValueBuffer, "%f", levelLow );
		return displayValueBuffer;
	case PT_HIGHPOINT_THRESHOLD:
		sprintf( displayValueBuffer, "%f", highThresold );
		return displayValueBuffer;
	case PT_LOWPOINT_THRESHOLD:
		sprintf( displayValueBuffer, "%f", lowThresold );
		return displayValueBuffer;

	default:
		return CFFGLPlugin::GetParameterDisplay( index );
	}
}

FFResult ImageEffects::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PT_HIGHPOINT_GAIN:
		levelHigh = value;
		break;
	case PT_MIDPOINT_GAIN:
		levelMid = value;
		break;
	case PT_LOWPOINT_GAIN:
		levelLow = value;
		break;
	case PT_HIGHPOINT_THRESHOLD:
		highThresold = value;
		break;
	case PT_LOWPOINT_THRESHOLD:
		lowThresold = value;
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
		case PT_HIGHPOINT_GAIN:
			return levelHigh;
		case PT_MIDPOINT_GAIN:
			return levelMid;
		case PT_LOWPOINT_GAIN:
			return levelLow;
		case PT_HIGHPOINT_THRESHOLD:
			return highThresold;
		case PT_LOWPOINT_THRESHOLD:
			return lowThresold;
	}

	return 0.0f;
}
