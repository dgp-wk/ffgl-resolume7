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
uniform sampler2D InputTexture;
uniform vec3 Uniform1;
uniform vec3 Uniform2;
uniform vec3 Uniform3;

float _exposure;
float _contrast;
float _brightness;
float _gamma;
float _saturation;
float _colorTemperature;
vec3 _colorFactor;

in vec2 uv;

out vec4 fragColor;

vec3 colorTemperature(vec3 color)
{
	return color;
}

vec3 exposure(vec3 color)
{
	colorSum = color.r + color.g + color.b;
	return color * (pow(2, colorSum * _exposure) -1);
}

vec3 brightness(vec3 color)
{
	return color + vec3(_brightness);
}

vec3 contrast(vec3 color)
{
	return color;
}

vec3 saturation(vec3 color)
{
	return color;
}

vec3 gamma(vec3 color)
{
	return color;
}

vec3 colorFactor(vec3 color)
{
	return color;
}

void main()
{
	//Pull uniforms
	_exposure			= Uniform1.r;
	_contrast			= Uniform1.g;
	_brightness			= Uniform1.b;
	_gamma				= Uniform2.r;
	_saturation			= Uniform2.g;
	_colorTemperature	= Uniform2.b;
	_colorFactor		= Uniform3.rgb;

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = exposure(color.rgb);
	color.rgb = contrast(color.rgb);
	color.rgb = brightness(color.rgb);
	color.rgb = gamma(color.rgb);
	color.rgb = saturation(color.rgb);
	color.rgb = colorTemperature(color.rgb);
	color.rgb = colorFactor(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion