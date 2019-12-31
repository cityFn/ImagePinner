/*****
C_Date	: 2018-06-13
L_Date	: 2018-06-13
Author	: cityFn
Desc		: For loading Image from the path
*****/

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include "SDL_envHeader.h"

class ImageLoader
{
	public:
		ImageLoader();
		virtual ~ImageLoader();

		const char* getPath();
		void setPath(const char*path);

		void RenderImage(FS nSwitch);
	protected:

	private:
		FS imageState;
		FS pathState;
		string m_strPath;
};

#endif // IMAGELOADER_H
