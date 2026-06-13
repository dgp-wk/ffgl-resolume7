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
	

	AddParam( ffglqs::ParamRange::Create( "u_levelHigh",	1.0f,	ffglqs::ParamRange::Range( 0.0f, 10.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_levelMid",		1.0f,	ffglqs::ParamRange::Range( 0.0f, 10.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_levelLow",		1.0f,	ffglqs::ParamRange::Range( 0.0f, 10.0f ) ) );

	AddParam( ffglqs::ParamRange::Create( "u_lowOffset",	0.0f,	ffglqs::ParamRange::Range( -0.5f, 0.5f ) ) );

	AddParam( ffglqs::ParamRange::Create( "u_thresholdHigh",0.7f,	ffglqs::ParamRange::Range( 0.0f, 1.0f  ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_thresholdLow",	0.3f,	ffglqs::ParamRange::Range( 0.0f, 1.0f  ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_fade",			0.05f,	ffglqs::ParamRange::Range( 0.01f, 0.1f ) ) );

	SetParamDisplayName( 0, "High Gamma", true );
	SetParamDisplayName( 1, "Mid Gamma", true );
	SetParamDisplayName( 2, "Low Gamma", true );
	SetParamDisplayName( 3, "Low Offset", true );
	SetParamDisplayName( 4, "High Point", true );
	SetParamDisplayName( 5, "Low Point", true );
	SetParamDisplayName( 6, "Blend", true );

	SetFragmentShader( fshader );
}

Effect::~Effect()
{
}
