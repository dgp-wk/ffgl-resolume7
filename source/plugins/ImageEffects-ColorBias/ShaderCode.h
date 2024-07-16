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


	return color;
}

vec3 exposure(vec3 color)
{
	float colorSum = color.x + color.y + color.z;
	return color * (pow(2, 1 + _exposure) -1);
}

vec3 brightness(vec3 color)
{
	return clamp(color + vec3(_brightness),vec3(0.0),vec3(1.0));
}

vec3 contrast(vec3 color)
{
	return (color - 0.5) * (_contrast * 2) + 0.5;
}

vec3 saturation(vec3 color)
{
	vec3 original = color;
	vec3 weights = vec3(0.2125, 0.7154, 0.0721);
    float luminance = dot(original.rgb, weights);
	return mix(vec3(luminance), original, vec3(_saturation) * 5.0);
}

vec3 gamma(vec3 color)
{
	return pow(color,vec3(_gamma) * 2.2);
}

vec3 levels(vec3 color)
{
	float luma = (color.r + color.g + color.b) * 0.333;
	float lowFactor = max(0,sign(luma - _thresholds.y) * -1);
	float highFactor = max(0,sign(luma - _thresholds.x));
	float midFactor = ((lowFactor + highFactor) - 1) * -1;

	highFactor = highFactor * (pow(2, 1 + _levels.r) -1);
	midFactor = midFactor * (pow(2, 1 + _levels.g) -1);
	lowFactor = lowFactor * (pow(2, 1 + _levels.b) -1);

	return (color * highFactor) + (color * midFactor) + (color * lowFactor);
}

vec3 colorFactor(vec3 color)
{
	// return clamp(color + (vec3(1.0) * _colorFactor), vec3(0.0),vec3(1.0));
	return color + (color * _colorFactor);
}

void main()
{
	//Pull uniforms
	_colorFactor		= Uniform3.rgb;

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = colorFactor(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion