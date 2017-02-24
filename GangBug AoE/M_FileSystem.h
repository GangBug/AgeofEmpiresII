#ifndef __M_FILESYSTEM_H__
#define __M_FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int closeSdlRwops(SDL_RWops *rw);

class M_FileSystem : public Module
{
public:

	M_FileSystem();

	// Destructor
	virtual ~M_FileSystem();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* pathOrZip, const char* mountPoint = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	uint Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	uint Save(const char* file, const char* buffer, unsigned int size) const;

	void DrawDebug();

private:

};

#endif // __M_FILESYSTEM_H__