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
vec3 colorTemperature(vec3 color, float _colorTemp)
{
	mat3 m = (_colorTemp <= 6500.0) ? 
		mat3(vec3(0.0, -2902.1955373783176, -8257.7997278925690),
	    vec3(0.0, 1669.5803561666639, 2575.2827530017594),
	    vec3(1.0, 1.3302673723350029, 1.8993753891711275)) : 
	 	mat3(vec3(1745.0425298314172, 1216.6168361476490, -8257.7997278925690),
   	    vec3(-2666.3474220535695, -2173.1012343082230, 2575.2827530017594),
	    vec3(0.55995389139931482, 0.70381203140554553, 1.8993753891711275)); 
	
	return color * mix(clamp(vec3(m[0] / (vec3(_colorTemp) + m[1]) + m[2]), vec3(0.0), vec3(1.0)), vec3(1.0), smoothstep(1000.0, 0.0, _colorTemp));
} 

void main()
{
	vec4 color = texture( inputTexture, i_uv );

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = colorTemperature(color.rgb, colorTemp);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";