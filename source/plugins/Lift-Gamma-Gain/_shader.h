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

static const std::string fshader1 = R"(

float saturate(float value)
{
	return max(0.0,min(1.0,value));
}

float applyLGG(float lift, float invGamma, float gain, float value)
{
	float lerpValue = saturate(pow(value, invGamma));
	value = gain * lerpValue + lift * ( 1.0 - lerpValue );

	return value;
}

void main()
{
	vec4 color = texture( inputTexture, i_uv );

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;
	
	vec4 liftColor = u_liftColor, gammaColor = u_gammaColor, gainColor = u_gainColor;
	vec4 liftAdjust, gammaAdjust, gainAdjust, inverse_gammaAdjust;
	vec4 grey;

	float avgLift	= (liftColor.r + liftColor.g + liftColor.b)/3.0;
	float avgGamma	= (gammaColor.r + gammaColor.g + gammaColor.b)/3.0;
	float avgGain	= (gainColor.r + gainColor.g + gainColor.b)/3.0;

//	liftColor		= liftColor - avgLift;
//	gammaColor		= gammaColor - avgGamma;
//	gainColor		= gainColor - avgGain;

	grey			= 0.5 + (gammaColor + midtoneOffset);

	liftAdjust		= 0.0 + (liftColor + shadowOffset);
	gammaAdjust.r	= log( (0.5 - liftAdjust.r) / (gainAdjust.r - liftAdjust.r) ) / log(grey.r);
	gammaAdjust.g	= log( (0.5 - liftAdjust.g) / (gainAdjust.g - liftAdjust.g) ) / log(grey.g);
	gammaAdjust.b	= log( (0.5 - liftAdjust.b) / (gainAdjust.b - liftAdjust.b) ) / log(grey.b);
	gainAdjust		= 1.0 + (gainColor + highlightOffset);

	inverse_gammaAdjust.r	= 1.0/ gammaAdjust.r;
	inverse_gammaAdjust.g	= 1.0/ gammaAdjust.g;
	inverse_gammaAdjust.b	= 1.0/ gammaAdjust.b;
	
	color.r*=		applyLGG(liftAdjust.r, inverse_gammaAdjust.r, gainAdjust.r, color.r);
	color.g*=		applyLGG(liftAdjust.g, inverse_gammaAdjust.g, gainAdjust.g, color.g);
	color.b*=		applyLGG(liftAdjust.b, inverse_gammaAdjust.b, gainAdjust.b, color.b);

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";

static const std::string fshader = R"(

void main()
{
	vec4 color = texture( inputTexture, i_uv );

	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;
	
	vec4 lift = u_lift, gamma = u_gamma, gain = u_gain;
	lift.rgb *= (lift.a) * liftStrength;
	gamma.rgb *= gamma.a;
	gain.rgb *= gain.a;

	color.rgb += lift.rgb;
	color.rgb = pow(color.rgb, 1/gamma.rgb);	
	color.rgb *= gain.rgb;

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";