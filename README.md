Algerian Flag and XAMPP Logo Graphics Project
So here’s what this project does: it’s a computer graphics app built with OpenGL and GLUT in C++. The whole thing runs smoothly and pulls off some cool tricks:
- The Algerian flag waves, like it’s caught in the wind.
- The XAMPP logo pops up, created with Bézier curves and a bit of tessellation magic.
- You get interactive controls. Zoom, animate, reset—just hit a key and things change on the fly.
What’s going on behind the scenes? You’re looking at OpenGL rendering, polygon tessellation, Bézier curve generation, and 2D transformations.

->Features
Algerian Flag:
- Waving animation, so it actually moves
- Crescent moon and star drawn in detail
- A proper flagpole—it isn’t floating in mid-air
- Smooth color transitions to make it look sharp

XAMPP Logo:
- Curves rendered in that classic SVG style
- Shapes approximated with Bézier curves
- Tessellated polygons, including ones with holes
- Everything scales cleanly when you zoom in or out

User Interaction:
- Zoom in/out on the XAMPP logo
- Turn the waving flag animation on or off
- Reset the animation whenever you want
- All updates happen in real time—no lag

Tech Stack
- C++
- OpenGL
- GLUT (FreeGLUT)
- GLU

Before you get started(run the project), make sure your computer has these  Software:
 C++ Compiler (G++)  Runs and builds C++ code 
 OpenGL  Handles graphics 
 GLUT / FreeGLUT  Lets you make windows and handle input

If you don’t have these yet, here’s what to do:
Windows folks:
- Option 1: Code::Blocks (This is easiest)
  1. Download Code::Blocks with MinGW already bundled.
  2. Make sure you check the MinGW box during install.
  3. FreeGLUT usually comes along, but you can add it if needed.
  4. after that on code block you click on create project and then click glut project give it name and finish it by click next option to reach finish.
  5. after that you open main.cpp and begin write project if you need this project directly delete on main.cpp and copy this project and paste on that.

Linux users (Ubuntu or Debian):
Open up your terminal and punch in:
sudo apt update
sudo apt install g++ freeglut3-dev
This pulls in the G++ compiler, plus both OpenGL and FreeGLUT libraries.
To check if G++ got installed, run:
g++ --version
If you see the version number, you’re good to go.
