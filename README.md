
> December 30, 2024 - Precompiled Windows Binaries [here](https://github.com/dgp-wk/ffgl-resolume7/releases).
>
>*Note: This is for Resolume 7. These plugins will probably break in other FFGL-aware software.*
This is one *fork* of the Resolume *fork* of the FFGL repository with a few additional plugins: 

- Image Effects Shaders 
	- Image Effects (Groups all the bottom filters into one filter)
	- Color Bias (Alternative to the Add Subtract (color) filter)
	- Color Temperature
	- Exposure (An alternative to the built-in Exposure effect, but offers additional effect power)
	- Gamma
	- Levels (Lets you tweak Highs/Mids/Lows)

*These effects are largely clipped from https://www.shadertoy.com/view/NstSWf . These effects are intended to provide a bit more color control.*

- Misc. Effects
	- Wrapper (Emulates the texture wrap function -- stretch the edges of an image across the composition.)
	- Example (A more pared down example than what's already been included.)

> I'm also adding a guide so that friends of mine who've never built one of these plugins can try to do it themselves. Good luck.

## HOW TO (Windows)
Read [this](https://github.com/resolume/ffgl/wiki) (maybe). I'm going to assume that you've never worked with Visual Studio, C++, glsl, or have done any software development in the past. This HOW TO will guide you all the way up to making your first effect filter with some copy-paste code.
1. Install Visual Studio. You probably want the community edition (which is free).
	- https://visualstudio.microsoft.com/
	- While installing Visual Studio, make sure to **check whatever box** offers to install the **Windows SDK**.
2. Get a copy of this repository.
	- (Simple) Download [this zip](https://github.com/dgp-wk/ffgl-resolume7/archive/refs/heads/master.zip) and extract it somewhere where you can find it.
	- (Less Simple) Check out this repository from github
		- Open Visual Studio
		- On the right hand side, click "Clone a Repository"
		- Copy and paste https://github.com/dgp-wk/ffgl-resolume7.git into the Repository location dialog.
3. Open the project.
	- You'll want to open the project using the `FFGLPlugins.sln` file. It'll be located in the `build\windows\` folder inside of the project directory. When in doubt, just search for the filename.
		- If you cloned the project, Visual Studio will just open it. However, you'll still need to search out the solution file on subsequent openings.
	- Once the project loads up, give it a few minutes to scan and analyze.
4. Build the project.
	- Look at the middle-top of the Visual Studio window. You should see a group of drop-down boxes that say something like `Local Machine` `Debug` and `x64`. Make sure they match.
	- Look for "Build" at the top of the Visual Studio. Click on it and select "Build Solution"
		- Or just press F7.
	- Go grab a coffee. This is going to compile all of the plugins in the project. It can take about 5 minutes.
	- If everything compiles without throwing an error, do a happy dance. 
5. Tell Resolume where to find your plugins.
	- Open Resolume.
	- Hit the `Arena` menu, then open `Preferences`.
	- Click on `Video`.
	- Press the + button. A file dialogue will open up. Navagate to where your project files are located. Go **into** the `binaries\x64\Debug` folder, then hit `Select Folder`.
	- Close the Preferences window. Resolume should have already started loading in the effects you built earlier.
6. Create a New Effect.
	- Create a new project meta file. (This part sucks)
		- Go into your file explorer and enter the `build\windows\` directory. This is the same place where the `FFGLPlugins.sln` is sitting.
		- Copy `Example_Effect.vcxproj`, then rename it to a name of your choosing (how about `Test_Effect.vcxproj`?).
		- Open your new `.vcxproj` Using notepad (++, or a text editor of your choosing).
		- The your find function to search for `<ProjectGuid>`. It'll find something like: `<ProjectGuid>{FC65EF1A-9029-40B9-B703-F1A8F563D458}</ProjectGuid>`
			- Delete it. The whole line.
		- Search and replace `Example_Effect` with a name for your effect (how about `Test_Effect`?).
		- Save and close.
	- Copy the Example_Effect files and load them into Visual Studio. (This part is a bit better)
		- Back out to the `\source\plugins` folder. 
		- Find the Example_Effects folder and copy it. *Rename it with a name for your effect, the same name you've been using all along (how about `Test_Effect`?).*
		- Go back to Visual Studio. 
		- On the left hand side, on the Solution Explorer, right-click on the Solution (it should be the first item in the list).
		- Hover `Add` then select `Existing Project...`
		- Navigate your way back to the `build\windows\` and select your `.vcxproj` file.
		- Assuming everything went well, it should load in your new Effect project and all the files you made a copy of.
			- You'll probably also notice that there are a bunch of files in the project. Those are external dependencies which need to be in there for compilation, but because we didn't copy over the project's `.filters` file, they're not sorted neatly. Make a new folder within the Visual Studio project (how about `lib`?) and drag all of those unfamiliar files in there.
	- Start messing around.
		- Assuming everything above worked out, I want you to open up the _config.h file within your new Effect project.
			- This is a convenience file that lets us define some stuff without messing up the main body of the effect code.
			- Change the `PLUGIN_ID` and the `PLUGIN_NAME` to something that you would recognize. How about `TST1` and `,Test Effect`?
		- Open up ``_shader.h``
			- This is a program that will run on the GPU. It is taking a color -- `InColor` -- from the Resolume GUI and splatting it right on the screen. We're gonna mess with it a bit.
			- 	Replace `fragColor = color;` with `fragColor = vec4( i_uv[0] * color.r, i_uv[1]  *color.g, i_uv[0] * i_uv[1] * color.b, 1);`
		- Build the project. Go to Resolume and pull your effect into the Composition. Find its widget in the inspector and move the colors around. What do you see? Is it a big funky gradient? If so, do a happy happy dance.

### Notes on Development, Debugging, and Building

 - Visual studio will fail a build if you have a plugin currently *slotted* in Resolume. That is, if you have a layer hidden somewhere with your Effect applied to it, it just won't build. You'll have to close all instances if your effect in Resolume before you can build a new version of it.
 - So long as you built with the Debug configuration, you'll be able to drop breakpoints and watches and step through the code. You'll need to attach to the Arena process.
 - The Resolume FFGL repository is 50/50 on example quality. The instructions in the Wiki will point you into building a project that will build, but will fail to do anything in Resolume. The AddSubtract project does work, however, but it lacks some of the newer syntactic sugar.
 - Similarly, the `ffglqs` namespace will offer you some pretty Plugin classes with all that syntactic sugar, but YMMV.
 - Some people have been building additional frameworks and plugin-generators, including a Rust-based framework which seems pretty slick, at least at a distance. https://github.com/edeetee/ffgl-rs

## HOW TO (Mac)
- lol. lmao even
- But seriously, you would need a Mac running the architecture you intend to build for (Sillicon != Intel, which is important if you're still using trashcans to run displays) and XCode. I've never built for Mac, but there is a walkthrough later in this README.
- Do not expect any of the projects to get effortlessly loaded into XCode. You may need to go file wrangling because of the way Visual Studio handles file structures, and because of the way the the Resolume developers decided to arrange the project.





## Old README Begins Here


This is the Resolume fork of the FFGL repository. It is up to date and has Visual Studio and Xcode projects to compile 64 bit plugins that can be loaded by Resolume 7.0.3 and up.  

**Note for macOS developers:** *Resolume 7.11.0 has added native ARM support. This means that on Apple Sillicon it will run as a native ARM process. Native ARM processes cannot load x86_64 based plugins. To enable your plugin to be loaded you should build it as universal build. If your Xcode is up-to-date enough you can choose to build for "Any Mac (Apple Silicon, Intel)" instead of "My Mac" in the top left corner. Please read the [apple developer documentation](https://developer.apple.com/documentation/apple-silicon/building-a-universal-macos-binary) for more information about universal builds.*

The master branch is used for continued development. It will contain the latest features, fixes and bugs. Plugins compiled with the master branch will work in Resolume 7.3.1 and up.
If you do not want to be affected by the latest bugs you can use one of the stable releases. eg FFGL 2.2, which is the most recent released version of the sdk. Plugin development for Resolume 7.0.0/7.0.1/7.0.2 is no longer supported by this repository. These versions are very old and there are many newer versions that users can update to.

You can find some help to get started with FFGL plugin development on the [wiki](https://github.com/resolume/ffgl/wiki).

Also more examples are available on this [repo](https://github.com/flyingrub/ffgl/tree/more/).

## Master branch changes since FFGL 2.2
- Replaced glload by glew, enabling OpenGL 4.6 extensions to be used inside plugins. Plugins may need to add deps/glew.props to their project's property pages for them to link to the binary.
- Implemented parameter display names. Parameter names are used as identification during serialization, display names can be used to override the name that is shown in the ui. The display name can also be changed dynamically by raising a display name changed event. (Requires Resolume 7.4.0 and up)
- Implemented value change events. Plugins can change their own parameter values and make the host pick up the change. See the new Events example on how to do this. (Requires Resolume 7.4.0 and up)
- Implemented dynamic option elements. Plugins can add/remove/rename option elements on the fly. (Requires Resolume 7.4.1 and up)

*You can suggest a change by creating an issue. In the issue describe the problem that has to be solved and if you want, a suggestion on how it could be solved.*

## Quickstart

Below are the first steps you need to create and test an FFGL plugin for Resolume. This assumes you have experience with git and C++ development.

### Mac

- Go to `<repo>/build/osx`, open `FFGLPlugins.xcodeproj`
- Create a compilation target for your plugin:
	- Select the Xcode project (top of the tree)
	- Duplicate a target and rename it
	- Remove the old plugin-specific files under Build Phases > Compile Sources (e.g. if you duplicated Gradients, remove `FFGLGradients.cpp`)
	- Duplicating a target in Xcode creates and assigns a new `xx copy-Info.plist` file, but we don't want that. Go to Build Settings > Packaging > Info.plist and change the file name to `FFGLPlugin-Info.plist`.  
	- Find the reference to the newly created `xx copy-Info.plist` file in the Xcode Project Navigator (probably all the way down the panel) and remove it there. When asked, choose Move to Trash.
- In Finder, duplicate a plugin folder and rename the files. Choose a corresponding plugin type, e.g. copy `AddSubtract` if you want to build an Effect plugin or `Gradients` if you want to build a Source plugin.
- Drag the new folder into the Xcode project. You will be asked to which target you want to add them, add them to your new target.
- Go to the target's Build Phases again and make sure there are no resources under the Copy Bundle Resources phase.
- Replace the class names to match your new plugin name and rename the elements in the PluginInfo struct
- Fix up the Build scheme:
	- When duplicating a target, a Build Scheme was also created. Next to the play and stop buttons, click the schemes dropdown and select Manage Schemes. 
	- Rename the scheme that was auto-created (e.g. "Gradient copy")
	- Select it in the scheme drop down.
- Press play (Cmd+B) to compile.
- Copy the resulting `.bundle` file from `<repo>/binaries/debug` to `~/Documents/Resolume/Extra Effects` and start Arena to test it.

### Windows 

This assumes you use Visual Studio 2017

- Go to `<repo>/build/windows`, duplicate a `.vcxproj` and the corresponding `.vcxproj.filters` file, and rename them.
- Open `FFGLPlugins.sln`. Then right-click the Solution in the solution explorer (top of the tree), and choose Add > Existing Project and select the new file.
- Remove the original `.cpp` and `.h` source files from the newly added project, i.e. if you duplicated `Gradient.vcxproj`, remove `FFGLGradients.h` and `FFGLGradients.cpp`
- In Explorer, go to `<repo>/source/`, duplicate a plugin folder and rename the files. Choose a corresponding plugin type, i.e. copy `AddSubtract` if you want to build an Effect plugin or `Gradients` if you want to build a Source plugin.
- Add the new source files to the project by dragging them into Visual Studio, onto your new project.
- If you want to start the build with Visual Studio's Build command (F5), right-click the project and select Set as Startup Project. Altenatively, you can right-click the project and select Build.
- After building, find the resulting `.dll` file in `\binaries\x64\Debug`. Copy it to `<user folder>/Documents/Resolume/Extra Effects`