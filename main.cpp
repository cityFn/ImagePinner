// file headers.
#include <cstdio>
#include <string>
#include <fstream>
#include <direct.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

// definitions
#define wDefault SDL_WINDOWPOS_CENTERED
#define nDefault -1

// internal global variant.
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
double CERTAIN_WINDOW_WIDTH = 450;

// main resources
SDL_Window* gDefaultWindow = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* gTexture = NULL;

// configuration variables
int cfgWidth = -1;
string cfgPath;

// initialization of SDL basics
bool Init();

// release of SDL basics
void Close();

// load in media
bool LoadInMedia();

// load out media(if possible)
void LoadOutMedia();

// read configuration file
bool ReadConfig();

SDL_Texture* LoadTexture(string path);

bool Init()
{
	bool bSuccess = true;

	// initialize
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL error : %s\n", SDL_GetError());
		bSuccess = false;
	}
	else
	{
		// set render filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Linear filtering for texture not enabled...\n");
		}

		// create window
		gDefaultWindow = SDL_CreateWindow(
											"Picture Of Today!",
											wDefault,
											wDefault,
											SCREEN_WIDTH,
											SCREEN_HEIGHT,
											SDL_WINDOW_SHOWN |
											SDL_WINDOW_ALWAYS_ON_TOP);
		if (NULL == gDefaultWindow)
		{
			printf("SDL window error : %s\n", SDL_GetError());
			bSuccess = false;
		}
		else
		{
			// create renderer of the view
			gRender = SDL_CreateRenderer(gDefaultWindow, -1, SDL_RENDERER_SOFTWARE);
			if (NULL == gRender)
			{
				printf("Renderer could not be called. SDL error : %s\n", SDL_GetError());
				bSuccess = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRender, 0xFF, 0xFF, 0xFF, 0xFF); // r g b a 0x00 256

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					bSuccess = false;
				}
			}
		}
	}

	return bSuccess;
}

void Close()
{
	SDL_DestroyRenderer(gRender);
	SDL_DestroyWindow(gDefaultWindow);
	gRender = NULL;
	gDefaultWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool LoadInMedia()
{
	bool bSuccess = true;

	char szBuffer[250];
	getcwd(szBuffer, 250);
	string sPath(szBuffer);
	if(!sPath.empty())
	{
		size_t startPos = 0;
		while((startPos = sPath.find("\\", startPos)) != string::npos)
		{
			sPath.replace(startPos, 1, "/");
			startPos++;
		}
	}

	if (!cfgPath.empty())
	{
		sPath.append("/resource/");
		sPath.append(cfgPath);
	}
	else
	{
		sPath.append("/resource/default.jpg");
	}

	gTexture = LoadTexture(sPath);

	if (NULL == gTexture)
	{
		printf("SDL image load failure : %s\n", SDL_GetError());
		bSuccess = false;
	}
	else
	{
		printf("SDL image load in success...\n");
	}

		// readjust certain window width from file
		if ((cfgWidth  > -1) && (cfgWidth  < 1920))
		{
			CERTAIN_WINDOW_WIDTH = cfgWidth;
		}

	// read the width and height of image
	SDL_QueryTexture(gTexture, NULL, NULL, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// make sure the width and height is in range
	if(SCREEN_WIDTH > CERTAIN_WINDOW_WIDTH)
	{
		double dRatio = static_cast<double>(SCREEN_WIDTH) / CERTAIN_WINDOW_WIDTH;
		SCREEN_HEIGHT = static_cast<int>(static_cast<double>(SCREEN_HEIGHT) / dRatio);
		SCREEN_WIDTH = CERTAIN_WINDOW_WIDTH;
	}

	return bSuccess;
}

void LoadOutMedia()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
}

bool ReadConfig()
{
	bool bSucces = false;

	char szBuffer[250];
	getcwd(szBuffer, 250);
	string sPath(szBuffer);
	if(!sPath.empty())
	{
		size_t startPos = 0;
		while((startPos = sPath.find("\\", startPos)) != string::npos)
		{
			sPath.replace(startPos, 1, "/");
			startPos++;
		}
	}

	sPath.append("/config.txt");

	ifstream configFile(sPath.c_str());
	if (configFile.is_open() && configFile.good())
	{
		string strBuffer;
		while (getline(configFile, strBuffer))
		{
			size_t pos = strBuffer.find("maxWidth", 0);
			if (pos != string::npos)
			{
				size_t formatPos = strBuffer.find("=", pos);
				++formatPos;
				if (formatPos != string::npos)
				{
					cfgWidth = atoi(strBuffer.substr(formatPos).c_str());
	printf("%s\n", strBuffer.substr(formatPos).c_str());
				}
			}
			pos = strBuffer.find("file", 0);
			if (pos != string::npos)
			{
				size_t formatPos = strBuffer.find("=", pos);
				++formatPos;
				if (formatPos != string::npos)
				{
					cfgPath = strBuffer.substr(formatPos);
	printf("%s\n", strBuffer.substr(formatPos).c_str());
				}
			}
		}
		bSucces = true;
	}

	return bSucces;
}

SDL_Texture* LoadTexture(string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(NULL == loadedSurface)
	{
		printf("Unable to load image\npath : %s\nerror: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRender, loadedSurface);
		if(NULL == newTexture)
		{
			printf("Unable to create texture\npath : %s\nerror: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}

	return newTexture;
}

int main(int argc, char* argv[])
{
	if (!ReadConfig())
	{
		printf("error reading configuration file...\n");

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"Configuration file error",
								"File is missing or can`t be opened. Please check the \"config.txt\".",
								NULL);
	}
	if (!Init())
	{
		printf("Initialize failure...\n");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"Program error",
								"(~ =5 =)~",
								NULL);
	}
	else
	{
		if (!LoadInMedia())
		{
			printf("Load in media failure...\n");

			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
									"Missing media",
									"Please check essential files.",
									NULL);
		}
		else
		{
			SDL_RenderClear(gRender);

			SDL_SetRenderDrawColor(gRender, 0xff, 0xff, 0xff, 0xff);
			SDL_Rect ViewPortRect;
			ViewPortRect.x = 0;
			ViewPortRect.y = 0;
			ViewPortRect.w = SCREEN_WIDTH;
			ViewPortRect.h = SCREEN_HEIGHT;

			// reset window size
			SDL_SetWindowSize(gDefaultWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

			// exit flag of main loop
			bool bExit = false;

			SDL_Event mainLoop;

			SDL_RenderSetViewport(gRender, &ViewPortRect);
			SDL_RenderCopy(gRender, gTexture, NULL, NULL);
			SDL_RenderPresent(gRender);
			while (!bExit)
			{
				while (0 != SDL_PollEvent(&mainLoop))
				{
					if (mainLoop.type == SDL_QUIT)
					{
						bExit = true;
					}
				}
				SDL_RenderPresent(gRender);
			}
		}
	}

	LoadOutMedia();
	Close();

	return 0;
}
