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
	AddParam( ffglqs::ParamRange::Create( "box", 0.0f, ffglqs::ParamRange::Range( 0.0f, 0.5f ) ) );
	AddParam( ffglqs::ParamRange::Create( "offsetX", 0.0f, ffglqs::ParamRange::Range( -1.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "offsetY", 0.0f, ffglqs::ParamRange::Range( -1.0f, 1.0f ) ) );
	AddParam( ffglqs::ParamRange::Create( "helperSize", 0.0f, ffglqs::ParamRange::Range( 0.0f, 100.0f ) ) );
	AddHueColorParam( "helperColor" );

	SetParamDisplayName( 0, "Size", true );
	SetParamDisplayName( 1, "X Offset", true );
	SetParamDisplayName( 2, "Y Offset", true );
	SetParamDisplayName( 3, "Helper Border", true );
	SetParamDisplayName( 4, "Helper Color", true );
}

Effect::~Effect()
{
}

FFResult Effect::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case 0:
		GetParam( "box" ).get()->SetValue( value );
		break;
	case 1:
		GetParam( "offsetX" ).get()->SetValue( value );
		break;
	case 2:
		GetParam( "offsetY" ).get()->SetValue( value );
		break;
	case 3:
		GetParam( "helperSize" ).get()->SetValue( floor( value / 1 ) * 1 );
		break;
	default:
		break;
	}

	return FF_SUCCESS;
}