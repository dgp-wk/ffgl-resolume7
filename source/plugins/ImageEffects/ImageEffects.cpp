#include "ImageEffects.h"
#include "ShaderCode.h"
using namespace ffglex;

#define SATURATION_FUDGE -0.3f
#define GAMMA_FUDGE -0.04497f

enum ParamType : FFUInt32
{
	PT_EXPOSURE,
	PT_CONTRAST,
	PT_BRIGHTNESS,
	PT_GAMMA,
	PT_HIGHPOINT_GAIN,
	PT_MIDPOINT_GAIN,
	PT_LOWPOINT_GAIN,
	PT_HIGHPOINT_THRESHOLD,
	PT_LOWPOINT_THRESHOLD,
	PT_SATURATION,
	PT_COLOR_TEMP,
	PT_RED_FACTOR,
	PT_GREEN_FACTOR,
	PT_BLUE_FACTOR
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< ImageEffects >,                       // Create method
	"IE00",                      // Plugin unique ID of maximum length 4.
	",Image Effects",            // Plugin name
	2,                           // API major version number
	1,                           // API minor version number
	1,                           // Plugin major version number
	0,                           // Plugin minor version number
	FF_EFFECT,                   // Plugin type
	"Omnibus image effects",     // Plugin description
	"Collects several image effects into a single effect"      // About
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
	_fixCount( 0 )
{
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	SetParamInfo( PT_EXPOSURE,		"Exposure",				FF_TYPE_STANDARD, 0.0f);
	SetParamInfo( PT_CONTRAST,		"Contrast",				FF_TYPE_STANDARD, 0.5f);
	SetParamInfo( PT_BRIGHTNESS,	"Brightness Gain",		FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_GAMMA,			"Gamma",				FF_TYPE_STANDARD, 0.5f );

	SetParamInfo( PT_HIGHPOINT_GAIN,"Highs",				FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_MIDPOINT_GAIN, "Mids",					FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_LOWPOINT_GAIN, "Lows",					FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_HIGHPOINT_THRESHOLD, "Highpoint",		FF_TYPE_STANDARD, 0.666f );
	SetParamInfo( PT_LOWPOINT_THRESHOLD, "Lowpoint",		FF_TYPE_STANDARD, 0.333f );

	SetParamInfo( PT_SATURATION,	"Saturation",			FF_TYPE_STANDARD, 0.5f );
	SetParamInfo( PT_COLOR_TEMP,	"Color Temperature",	FF_TYPE_STANDARD, 6500.0f );

	SetParamInfo( PT_RED_FACTOR,	"Red Bias",				FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_GREEN_FACTOR,	"Green Bias",			FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_BLUE_FACTOR,	"Blue Bias",			FF_TYPE_STANDARD, 0.0f );

	SetParamRange( PT_EXPOSURE, -1.0f, 1.0f );
	SetParamRange( PT_CONTRAST, 0.0f, 1.0f );
	SetParamRange( PT_BRIGHTNESS, -1.0f, 1.0f );
	SetParamRange( PT_GAMMA, 0.0f, 1.0f );
	SetParamRange( PT_SATURATION, 0.0f, 1.0f );

	SetParamRange( PT_HIGHPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_MIDPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_LOWPOINT_GAIN, -1.0f, 1.0f );
	SetParamRange( PT_HIGHPOINT_THRESHOLD, 0.5f, 1.0f );
	SetParamRange( PT_LOWPOINT_THRESHOLD, 0.0f, 0.5f );

	SetParamRange( PT_COLOR_TEMP, 2000.0f, 12000.0f );

	SetParamRange( PT_RED_FACTOR, -1.0f, 1.0f );
	SetParamRange( PT_GREEN_FACTOR, -1.0f, 1.0f );
	SetParamRange( PT_BLUE_FACTOR, -1.0f, 1.0f );

	FFGLLog::LogToHost( "Created ImageEffects effect" );
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

	glUniform3f( shader.FindUniform( "Uniform1" ), exposure, contrast, brightness );
	glUniform3f( shader.FindUniform( "Uniform2" ), gamma + GAMMA_FUDGE, saturation + SATURATION_FUDGE, colorTemperature );
	glUniform3f( shader.FindUniform( "Uniform3" ), red, green, blue );
	glUniform3f( shader.FindUniform( "Uniform4" ), levelHigh, levelMid, levelLow );
	glUniform3f( shader.FindUniform( "Uniform5" ), highThresold, lowThresold, 0.0f );

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
	case PT_EXPOSURE:
		sprintf( displayValueBuffer, "%f", exposure );
		return displayValueBuffer;
	case PT_CONTRAST:
		sprintf( displayValueBuffer, "%f", contrast );
		return displayValueBuffer;
	case PT_BRIGHTNESS:
		sprintf( displayValueBuffer, "%f", brightness );
		return displayValueBuffer;
	case PT_GAMMA:
		sprintf( displayValueBuffer, "%f", gamma );
		return displayValueBuffer;
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
	case PT_SATURATION:
		sprintf( displayValueBuffer, "%f", saturation );
		return displayValueBuffer;
	case PT_COLOR_TEMP:
		sprintf( displayValueBuffer, "%f", colorTemperature );
		return displayValueBuffer;
	case PT_RED_FACTOR:
		sprintf( displayValueBuffer, "%f", red );
		return displayValueBuffer;
	case PT_GREEN_FACTOR:
		sprintf( displayValueBuffer, "%f", green );
		return displayValueBuffer;
	case PT_BLUE_FACTOR:
		sprintf( displayValueBuffer, "%f", blue );
		return displayValueBuffer;

	default:
		return CFFGLPlugin::GetParameterDisplay( index );
	}
}

FFResult ImageEffects::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PT_EXPOSURE:
		exposure   = value;
		break;
	case PT_CONTRAST:
		contrast   = value;
		break;
	case PT_BRIGHTNESS:
		brightness   = value;
		break;
	case PT_GAMMA:
		gamma   = value;
		break;
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
	case PT_SATURATION:
		saturation   = value;
		break;
	case PT_COLOR_TEMP:
		if( value < 2000.0f )
			value = 6500.0f;
		if( value == 2000.0f && _fixCount == 0 )
		{
			_fixCount += 1;
			value = 6500.0f;
		}

		colorTemperature = floor( value / 100 ) * 100;
		break;
	case PT_RED_FACTOR:
		red   = value;
		break;
	case PT_GREEN_FACTOR:
		green   = value;
		break;
	case PT_BLUE_FACTOR:
		blue   = value;
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
		case PT_EXPOSURE:
			return exposure;
		case PT_CONTRAST:
			return contrast;
		case PT_BRIGHTNESS:
			return brightness;
		case PT_GAMMA:
			return gamma;
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
		case PT_SATURATION:
			return saturation;
		case PT_COLOR_TEMP:
			return colorTemperature;
		case PT_RED_FACTOR:
			return red;
		case PT_GREEN_FACTOR:
			return green;
		case PT_BLUE_FACTOR:
			return blue;
	}

	return 0.0f;
}
