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
	AddParam( ffglqs::ParamRange::Create( "greyPoint", 0.5f, ffglqs::ParamRange::Range(0.0f,1.0f) ) );
	AddParam( ffglqs::ParamRange::Create( "contrast", 1.0f, ffglqs::ParamRange::Range(0.0f,10.0f ) ) );
	SetParamDisplayName( 0, "Grey Point", true );
	SetParamDisplayName( 1, "Contrast", true );
}

Effect::~Effect()
{
}
