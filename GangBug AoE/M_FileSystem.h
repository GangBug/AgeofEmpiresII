#ifndef __M_FILESYSTEM_H__
#define __M_FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int closeSdlRwops(SDL_RWops *rw);

class M_FileSystem : public Module
{
public:

	M_FileSystem(bool startEnabled = true);

	// Destructor
	virtual ~M_FileSystem();

	// Called before render is available
	bool Awake(pugi::xml_node&)override;

	// Called before quitting
	bool CleanUp()override;

	// Utility functions
	bool AddPath(const char* pathOrZip, const char* mountPoint = nullptr);
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

	void DrawDebug()override;

private:

};

#endif // __M_FILESYSTEM_H__