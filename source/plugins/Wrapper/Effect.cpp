#include "_config.h"
#include "_shader.h"
#include "Effect.h"
using namespace ffglex;

enum ParamType : FFUInt32
{
	PT_BORDERSIZE,
	PT_BORDEROFFSETX,
	PT_BORDEROFFSETY
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< Effect >,	// Create method
	PLUGIN_ID,					// Plugin unique ID of maximum length 4.
	PLUGIN_NAME,				// Plugin name
	2,							// API major version number
	1,							// API minor version number
	PLUGIN_VERSION_MAJOR,		// Plugin major version number
	PLUGIN_VERSION_MINOR,		// Plugin minor version number
	PLUGIN_TYPE,				// Plugin type
	PLUGIN_DESCRIPTION,			// Plugin description
	PLUGIN_ABOUT				// About
);

Effect::Effect() :
	borderSize(0.0f),
	borderOffsetX(0.0f),
	borderOffsetY(0.0f)
{
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	SetParamInfo( PT_BORDERSIZE, "Size", FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_BORDEROFFSETX, "Offset X", FF_TYPE_STANDARD, 0.0f );
	SetParamInfo( PT_BORDEROFFSETY, "Offset Y", FF_TYPE_STANDARD, 0.0f );

	SetParamRange( PT_BORDERSIZE, 0.0f, 1.0f );
	SetParamRange( PT_BORDEROFFSETX, -1.0f, 1.0f );
	SetParamRange( PT_BORDEROFFSETY, -1.0f, 1.0f );


	FFGLLog::LogToHost( LOG_TEXT_CREATED );
}
Effect::~Effect()
{
}

unsigned int Effect::Connect()
{
	return FF_SUCCESS;
}

FFResult Effect::InitGL( const FFGLViewportStruct* vp )
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
FFResult Effect::ProcessOpenGL( ProcessOpenGLStruct* pGL )
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

	glUniform3f( shader.FindUniform( "Size" ), borderSize, borderOffsetX, borderOffsetY );

	quad.Draw();

	return FF_SUCCESS;
}
FFResult Effect::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();

	return FF_SUCCESS;
}

char* Effect::GetParameterDisplay( unsigned int index )
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
	case PT_BORDERSIZE:
		sprintf( displayValueBuffer, "%f", borderSize );
		return displayValueBuffer;
	case PT_BORDEROFFSETX:
		sprintf( displayValueBuffer, "%f", borderOffsetX );
		return displayValueBuffer;
	case PT_BORDEROFFSETY:
		sprintf( displayValueBuffer, "%f", borderOffsetY );
		return displayValueBuffer;

	default:
		return CFFGLPlugin::GetParameterDisplay( index );
	}
}

FFResult Effect::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PT_BORDERSIZE:
		borderSize = value;
		break;
	case PT_BORDEROFFSETX:
		borderOffsetX = value;
		break;
	case PT_BORDEROFFSETY:
		borderOffsetY = value;
		break;


	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float Effect::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
	case PT_BORDERSIZE:
		return borderSize;
	case PT_BORDEROFFSETX:
		return borderOffsetX;
	case PT_BORDEROFFSETY:
		return borderOffsetY;
	}

	return 0.0f;
}
