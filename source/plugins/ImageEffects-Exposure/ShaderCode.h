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
uniform vec2 Uniform1;

float _exposure;
float _power;

in vec2 uv;

out vec4 fragColor;


vec3 exposure(vec3 color)
{
	return vec3(	pow(1 + color.r , _power + _exposure) -1,
					pow(1 + color.g , _power + _exposure) -1,
					pow(1 + color.b , _power + _exposure) -1);
}



void main()
{
	//Pull uniforms

	_exposure		= Uniform1.x;
	_power			= Uniform1.y;

	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = exposure(color.rgb);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";
#pragma endregion