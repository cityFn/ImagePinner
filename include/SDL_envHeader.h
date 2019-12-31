#ifndef SDL_ENVHEADER
#define SDL_ENVHEADER

#include <cstdio>
#include <string>
#include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_envHeader.h>

using namespace std;

enum FRV // function return value
{
	Success = 0,
	Fail,
	ClassLackParameter,
	InnerClassReleased,
	RelatedClassReleased,
	Noresource
};

enum FS // Flag state
{
	Unload = 0,
	Loaded
};

#endif // SDL_ENVHEADER
