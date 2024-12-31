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

vec3 _levels;
vec3 _thresholds;

in vec2 uv;

out vec4 fragColor;

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

void main()
{
	//Pull uniforms

	_levels				= Uniform1.rgb;
	_thresholds			= Uniform2.rgb;

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = levels(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion