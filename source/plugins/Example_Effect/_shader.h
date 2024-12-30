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
void main()
{
	fragColor = color;
}
)";