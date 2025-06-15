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
	AddParam( ffglqs::ParamRange::Create( "topFade", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "bottom", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "bottomFade", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "left", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "leftFade", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "right", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "rightFade", 0.0f, ffglqs::ParamRange::Range( 0.0f, 1.0f ) ) );

	SetParamDisplayName( 0, "Top", true );
	SetParamDisplayName( 1, "Top Fade", true );
	SetParamDisplayName( 2, "Bottom", true );
	SetParamDisplayName( 3, "Bottom Fade", true );
	SetParamDisplayName( 4, "Left", true );
	SetParamDisplayName( 5, "Left Fade", true );
	SetParamDisplayName( 6, "Right", true );
	SetParamDisplayName( 7, "Right Fade", true );
}

Effect::~Effect()
{
}