#pragma once

// These are important configuration parameters.
#define PLUGIN_ID				"FCRP"			// Provide a unique 4-character string that identifies this plugin.
#define PLUGIN_NAME				",Fade Crop"		// This can be any string. This is the name of the plugin as it appears in the Resolume effects list.
#define PLUGIN_TYPE				FF_EFFECT		// FF_EFFECT, FF_MIXER, FF_SOURCE

// These are mostly decorative paramters. You can leave this alone as they don't seem to do much within Resolume, but other FFGL programs might use them.
#define PLUGIN_VERSION_MAJOR	0				// 
#define PLUGIN_VERSION_MINOR	0				//
#define PLUGIN_DESCRIPTION		"Provides a soft crop effect."
#define PLUGIN_ABOUT			"Provides a soft crop effect."

#define LOG_TEXT_CREATED		"FFGL: Created " PLUGIN_NAME " effect"