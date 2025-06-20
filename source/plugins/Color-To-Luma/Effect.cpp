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
	AddParam( ffglqs::ParamRange::Create( "red", 1.0f, ffglqs::ParamRange::Range( 0.0f,1.0f) ) );
	AddParam( ffglqs::ParamRange::Create( "green", 1.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "blue", 1.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	SetParamDisplayName( 0, "Red", true );
	SetParamDisplayName( 1, "Green", true );
	SetParamDisplayName( 2, "Blue", true );
	AddRGBColorParam( "weight" );
	SetParamDisplayName( 3, "Red", true );
	SetParamDisplayName( 4, "Green", true );
	SetParamDisplayName( 5, "Blue", true );
}

Effect::~Effect()
{
}