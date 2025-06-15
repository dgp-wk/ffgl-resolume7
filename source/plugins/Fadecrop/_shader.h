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
	vec4 color = texture(inputTexture, i_uv);

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	float leftCrop = 1, rightCrop = 1, topCrop = 1, bottomCrop = 1;

	if (i_uv.x > left && i_uv.x < left + leftFade){
		leftCrop = smoothstep(left, left + leftFade, i_uv.x);
	}

	if (i_uv.x < 1-right &&
		i_uv.x > 1-(right + rightFade))
	{
		rightCrop = smoothstep(1-right, 1-(right + rightFade), i_uv.x);
	}

	if (i_uv.y < 1-top &&
		i_uv.y > 1-(top + topFade))
	{
		topCrop = smoothstep(1-top, 1- (top + topFade), i_uv.y);
	}

	if (i_uv.y > bottom &&
		i_uv.y < bottom + bottomFade)
	{
		bottomCrop = smoothstep(bottom, bottom + bottomFade, i_uv.y);
	}

	color.a *= (leftCrop * rightCrop * topCrop * bottomCrop);

	leftCrop	= step(left,	i_uv.x);
	rightCrop	= step(i_uv.x,	1-right);
	topCrop		= step(i_uv.y,	1-top);
	bottomCrop	= step(bottom,	i_uv.y);

	color.a *= leftCrop * rightCrop * topCrop * bottomCrop;

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";