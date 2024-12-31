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
uniform float Uniform1;

float _gamma;

in vec2 uv;

out vec4 fragColor;

vec3 gamma(vec3 color)
{
	return pow(color,vec3(_gamma) * 2.2);
}

void main()
{
	//Pull uniforms

	_gamma				= clamp(Uniform1,0,1);

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = gamma(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion