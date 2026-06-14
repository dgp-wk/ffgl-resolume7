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

	AddParam( ffglqs::ParamRange::Create( "u_lumaBandPoint",	0.5f,  ffglqs::ParamRange::Range( 0.0f,  1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_lumaBandWidth",	1.0f,  ffglqs::ParamRange::Range( 0.01f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_fade",				0.05f, ffglqs::ParamRange::Range( 0.01f, 0.2f ) ) );
	AddElementSeparator( 0, 2 );
	
	AddParam( ffglqs::Param::Create( "u_tint", FF_TYPE_HUE, 0. ) );
	AddParam( ffglqs::Param::Create( "u_tint_saturation", FF_TYPE_SATURATION, 0. ) );
	AddParam( ffglqs::Param::Create( "u_tint_brightness", FF_TYPE_BRIGHTNESS, 0.0 ) );
	AddParam( ffglqs::Param::Create( "u_tint_alpha", FF_TYPE_ALPHA, 1.0 ) );


	AddParam( ffglqs::ParamRange::Create( "u_lift",				0.0f,  ffglqs::ParamRange::Range( -1.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_gamma",			1.0f,  ffglqs::ParamRange::Range(  0.5f, 4.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_gain",				1.0f,  ffglqs::ParamRange::Range( 0.0f,	 5.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "u_offset",			0.0f,  ffglqs::ParamRange::Range( -1.0f, 1.0f ) ) );

	SetParamDisplayName( 0, "Luma Point", true );
	SetParamDisplayName( 1, "Luma Width", true );
	SetParamDisplayName( 2, "Luma Blend", true );
	SetParamDisplayName( 3, "Color Tint", true );
	SetParamDisplayName( 7, "Lift", true );
	SetParamDisplayName( 8, "Gamma", true );
	SetParamDisplayName( 9, "Gain", true );
	SetParamDisplayName( 10, "Offset", true );
}

Effect::~Effect()
{
}
