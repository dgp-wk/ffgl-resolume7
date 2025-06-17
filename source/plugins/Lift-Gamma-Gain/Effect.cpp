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
	AddHueColorParam( "u_lift" );
	AddParam( ffglqs::ParamRange::Create( "liftStrength", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddHueColorParam( "u_gamma" );
	AddHueColorParam( "u_gain" );

	SetParamDisplayName( 0, "Lift", true );
	SetParamDisplayName( 4, "Lift Strength", true );
	SetParamDisplayName( 5, "Gamma", true );
	SetParamDisplayName( 9, "Gain", true );
}

Effect::~Effect()
{
}
