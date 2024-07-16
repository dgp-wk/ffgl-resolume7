#pragma once

#pragma region VertShader
static const char _vertexShaderCode[] = R"(#version 410 core
uniform vec2 MaxUV;

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV * MaxUV;
}
)";
#pragma endregion

#pragma region FragShader
static const char _fragmentShaderCode[] = R"(#version 410 core

// Most Shaders Derived from https://www.shadertoy.com/view/NstSWf

uniform sampler2D InputTexture;
uniform vec3 Uniform1;
uniform vec3 Uniform2;
uniform vec3 Uniform3;
uniform vec3 Uniform4;
uniform vec3 Uniform5;

float _exposure;
float _contrast;
float _brightness;
float _gamma;

vec3 _levels;
vec3 _thresholds;

float _saturation;
float _colorTemperature;
vec3 _colorFactor;

in vec2 uv;

out vec4 fragColor;

vec3 colorTemperature(vec3 color)
{
	mat3 m = (_colorTemperature <= 6500.0) ? 
		mat3(vec3(0.0, -2902.1955373783176, -8257.7997278925690),
	    vec3(0.0, 1669.5803561666639, 2575.2827530017594),
	    vec3(1.0, 1.3302673723350029, 1.8993753891711275)) : 
	 	mat3(vec3(1745.0425298314172, 1216.6168361476490, -8257.7997278925690),
   	    vec3(-2666.3474220535695, -2173.1012343082230, 2575.2827530017594),
	    vec3(0.55995389139931482, 0.70381203140554553, 1.8993753891711275)); 
	
	return color * mix(clamp(vec3(m[0] / (vec3(_colorTemperature) + m[1]) + m[2]), vec3(0.0), vec3(1.0)), vec3(1.0), smoothstep(1000.0, 0.0, _colorTemperature));
} 

void main()
{
	//Pull uniforms
	_colorTemperature	= Uniform2.b;
	//_colorTemperature	= 2000 + (_colorTemperature * 10000);

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = colorTemperature(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion