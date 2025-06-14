#include "Effect.h"
#include "_config.h"
#include "_shader.h"
using namespace ffglex;

static ffglqs::PluginInstance p = ffglqs::Effect::CreatePlugin< Effect >({ 
		PLUGIN_ID,
		PLUGIN_NAME 
	} );

Effect::Effect()
{
	SetFragmentShader( fshader );
	AddParam( ffglqs::ParamRange::Create( "top", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "bottom", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "left", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "right", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );

	SetParamDisplayName( 0, "Top", true );
	SetParamDisplayName( 1, "Bottom", true );
	SetParamDisplayName( 2, "Left", true );
	SetParamDisplayName( 3, "Right", true );
}

Effect::~Effect()
{
}