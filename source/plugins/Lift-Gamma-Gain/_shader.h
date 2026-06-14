#pragma once

/* Customary uniforms include:
*  Per https://github.com/resolume/ffgl/wiki/3.-Get-to-know-the-framework-better

		out vec4 fragColor;				// The output color.

		uniform vec2	resolution;		// The screen resolution in pixels.
		uniform float	time;			// The time in seconds since the application started.
		uniform float	deltaTime;		// The time elapsed since the last render call.
		uniform int		frame;			// The number of frames rendered since the beginning.
		uniform float	bpm;			// The current BPM set in resolume.
		uniform float	phase;			// The current phase position in a 4 beat represented as a float from 0 to 1 (0 is the first beat, 0.25 the second one, ect...).
	
		in vec2 i_uv;					// Sources and Effects. UV Position of this fragment. Top left = (0,0), bottom right = (1,1).
		uniform sampler2D inputTexture;	// Effects Only. A texture of whatever it is this effect is attached to.

		uniform sampler2D textureDest;	// Mixers Only. Mixers get two textures that they can.. mix. A layer is basically a texture in this context.
		uniform sampler2D textureSrc;	// Mixers Only.

		in vec2 i_uv_dest;				// Mixers Only. UV coordinates of this fragment relative to the two mixed textures.
		in vec2 i_uv_src;				// Mixers Only.

	If you are using the ffglqs Plugin classes and not manually submitting your own shader for compilation, you may wany to breakpoint lines 257 (Vertex) and 285 (Fragment) in FFGLShader.cpp. 
	That's where you'll get glShader Compilation messages (errors). 
	
	You'll also be able to watch what gets submitted for compilation at lines 246 (Vertex) and 268 (Fragment).
*/

static const std::string fshader = R"(

//Uniforms
// float u_lumaBandPoint
// float u_lumaBandWidth
// float u_fade
// vec4  u_tint
// float u_lift
// float u_gamma
// float u_gain
// float u_offset


void main()
{
	vec4 color = texture( inputTexture, i_uv );

	// The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	float luma				= dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

	float bandBottom		= u_lumaBandPoint - (u_lumaBandWidth * 0.5);
	float bandTop			= u_lumaBandPoint + (u_lumaBandWidth * 0.5);

	float lowEdge			= smoothstep(bandBottom - u_fade, bandBottom, luma);
	float highEdge			= 1.0 - smoothstep(bandTop, bandTop + u_fade, luma);
	float effectStrength	= lowEdge * highEdge * color.a;

	vec3 baseColor			= color.rgb;
	vec3 adjustedColor		= baseColor;

	// Lift
	adjustedColor			= adjustedColor * (1.0 - u_lift) + u_lift;

	// Gamma
	float gDelta            = u_gamma - 1.0;
	float gammaCurve         = gDelta * (-0.4621 + gDelta * (0.1983 - gDelta * 0.0461));
	adjustedColor           += adjustedColor * gammaCurve;

	// Gaim
	adjustedColor			*= u_gain;
	
	// Offset
	adjustedColor			+= u_offset;

	// Tint
	adjustedColor			+= u_tint.rgb;

	// Clamp color delta and mix with base color.
	adjustedColor			= clamp(adjustedColor, 0.0, 1.0);
	color.rgb				= mix(baseColor, adjustedColor, effectStrength);

	// The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	// ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";