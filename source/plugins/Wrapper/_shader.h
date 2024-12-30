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
uniform vec3 Size;
uniform vec3 Offset;

in vec2 uv;

out vec4 fragColor;

void main()
{

	vec4 color = texture
		(	
			InputTexture, 
			vec2
			(
				min(max(uv.x,Size.x + Size.y), 1-Size.x + Size.y),
				min(max(uv.y,Size.x + Size.z), 1-Size.x + Size.z)
			)
		);

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );

	fragColor = color;
}

)";
#pragma endregion