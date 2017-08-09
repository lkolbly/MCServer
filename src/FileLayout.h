
#pragma once

#include <iostream>

class cFileLayoutProvider
{
public:
	virtual ~cFileLayoutProvider() {};

	virtual AString GetWorldDataPrefix(void) = 0;
	virtual AString GetConfigPrefix(void) = 0;
	virtual AString GetResourcePrefix(void) = 0;
	virtual AString GetPluginPrefix(void) = 0;
	virtual AString GetPluginDataPrefix(void) = 0;
	virtual AString GetDataPrefix(void) = 0;
};

class cFileLayoutLegacy : public cFileLayoutProvider
{
public:
	virtual AString GetWorldDataPrefix(void) override  { return "./"; }
	virtual AString GetConfigPrefix(void) override     { return "./"; }
	virtual AString GetResourcePrefix(void) override   { return "./"; }
	virtual AString GetPluginPrefix(void) override     { return "Plugins/"; }
	virtual AString GetPluginDataPrefix(void) override { return "./"; }
	virtual AString GetDataPrefix(void) override       { return "./"; }
};

class cFileLayoutArmadillo : public cFileLayoutProvider
{
public:
	virtual AString GetWorldDataPrefix(void) override  { return "Worlds/"; }
	virtual AString GetConfigPrefix(void) override     { return "Config/"; }
	virtual AString GetResourcePrefix(void) override   { return "Resources/"; }
	virtual AString GetPluginPrefix(void) override     { return "Plugins/"; }
	virtual AString GetPluginDataPrefix(void) override { return "Data/"; }
	virtual AString GetDataPrefix(void) override       { return "Data/"; }
};

class cFileLayout
{
public:

	cFileLayout() {}

	~cFileLayout()
	{
		if (s_Layout != nullptr)
		{
			delete s_Layout;
		}
	}

	static void Autodetect(void)
	{
		// Check for settings.ini in the working directory
		if (cFile::IsFile(FILE_IO_PREFIX "settings.ini"))
		{
			std::cout << "Detected legacy file layout" << std::endl;
			s_Layout = new cFileLayoutLegacy();
			return;
		}
		std::cout << "Using new Armadillo file layout" << std::endl;
		s_Layout = new cFileLayoutArmadillo();
	}

	static cFileLayoutProvider & Get() { return *s_Layout; };

private:
	static cFileLayoutProvider * s_Layout;
};
