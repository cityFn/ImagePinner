/*****
TODO : implementation
*****/

#include "ImageLoader.h"

ImageLoader::ImageLoader() :
	imageState(FS::Unload),
	pathState(FS::Unload)
{
	//ctor
}

ImageLoader::~ImageLoader()
{
	//dtor
}

const char* ImageLoader::getPath()
{
	return m_strPath.c_str();
}

void ImageLoader::setPath(const char* path)
{
	if (NULL != path)
	{
		m_strPath.clear();
		m_strPath.assign(path);
	}
}

void ImageLoader::RenderImage(FS nSwitch)
{

}
