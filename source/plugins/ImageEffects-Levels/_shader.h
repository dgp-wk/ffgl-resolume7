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
// float u_levelHigh
// float u_levelMid
// float u_levelLow
// float u_lowOffset
// float u_thresholdHigh
// float u_thresholdLow
// float u_fade

void main()
{
	vec4 color = texture( inputTexture, i_uv );

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	float luma		 = (color.r + color.g + color.b) * 0.333;

	float lowFactor	 = 1.0 - smoothstep(u_thresholdLow, u_thresholdLow + u_fade, luma);

	float highFactor = smoothstep(u_thresholdHigh - u_fade, u_thresholdHigh, luma);

	float midFactor  = ((lowFactor + highFactor) - 1) * -1;

	float highAdjust = highFactor * (pow(2, u_levelHigh) -1);
	float midAdjust  = midFactor  * (pow(2, u_levelMid)  -1);
	float lowAdjust  = lowFactor  * (pow(2, u_levelLow)  -1);

	color = (color * highAdjust) + (color * midAdjust) + (color * lowAdjust) + (u_lowOffset * lowFactor);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}

)";