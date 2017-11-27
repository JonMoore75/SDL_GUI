# SDL_GUI
GUI for SDL2 without directly using OpenGL

Project status is in development and not yet feature complete.

This project is somewhat based on the structure of [NanoGUI](https://github.com/wjakob/nanogui) by Wenzel Jakob and heavily draws on ideas from that project but it not an exact copy.

This is a GUI aimed at SDL2 users who have a project that is exclusively written in SDL2, and who only wish to use the inbuilt texture and surface rendering and not directly call OpenGL. 

The Nano name has been dropped as this project does not use the OpenGL based NanoVG graphics library but instead uses the SDL2 rendering functions.

This GUI also handles full utf8 unicode characters (unlike NanoGUI), and the current examples demonstrates this with Japanese text in one of the edit boxes. 

To implement utf8 it uses the UTF8-CPP Project from http://utfcpp.sourceforge.net/ and uses free Japanese font from https://www.freejapanesefont.com/bokutachi-no-gothic-2-regular-download/.
