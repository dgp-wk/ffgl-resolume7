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
	AddParam( ffglqs::ParamRange::Create( "gammaValue", 1.0f, ffglqs::ParamRange::Range(0.5f,4.0f) ) );
	SetParamDisplayName( 0, "Gamma", true );
}

Effect::~Effect()
{
}
