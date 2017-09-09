
#pragma once

#include <iostream>

class cFileLayoutProvider
{
public:
	virtual ~cFileLayoutProvider() {}

	/** The path to the world data. World data should only be accessed by
	cWorld and classes storing per-world data. You know who you are. */
	virtual AString GetWorldDataPath(const AString & a_WorldName) = 0;

	/** The path to player data. */
	virtual AString GetPlayerPrefix(void) = 0;

	/** The path to the log files. */
	virtual AString GetLogsPrefix(void) = 0;

	/** Path to server configuration data. Files here should not be modified
	by the server, except to generate default values. These are files that
	primarily affect server operations instead of gameplay. For example,
	a file that specifies what network port the server should listen on
	should be in config. */
	virtual AString GetConfigPrefix(void) = 0;

	/** Path to server resources. Files here should not be modified by the
	server, except to generate default values. These files primarily affect
	gameplay, but not the operation of the server. For example, a crafting
	grid specification belongs here, because it affects gameplay without
	affecting server ops.
	Note that prefabs should go in their own folder, see GetPrefabPrefix. */
	virtual AString GetResourcePrefix(void) = 0;

	/** Path to plugin configuration. Files here should not be modified by
	plugins except to generate default values. Files which configure the
	operation of a plugin belong here, but Lua files and other executable
	files belong in the Plugin path (see GetPluginPrefix). */
	virtual AString GetPluginConfigPrefix(const AString & a_Plugin) = 0;

	/** Path to plugin Lua and executable files. Files here should not be
	modified by plugins, and this directory may be read-only or shared
	across multiple instances of Cuberite. */
	virtual AString GetPluginPrefix(const AString & a_Plugin) = 0;

	/** Path to plugin data files. Files here should only be modified by
	plugins. Plugins can assume that the files will be persistant from run
	to run, but must not fail if the files are absent. For example, a
	plugin which stores portal locations should assume that if the data
	files are gone, there are no portals. */
	virtual AString GetPluginDataPrefix(const AString & a_Plugin) = 0;

	/** Path to server-wide internal data. Files here should not be
	modified by the server operator, but rather by the server executable.
	Server-wide data should be stored here, per-world data should be stored
	in the world data path. For example, a database storing credentials for
	logged in users should be stored here. */
	virtual AString GetDataPrefix(void) = 0;

	/** Path to prefabs. These are files that represent structures used in
	world generation. The server should not modify these files. */
	virtual AString GetPrefabPrefix(void) = 0;
};

class cFileLayoutLegacy : public cFileLayoutProvider
{
public:
	virtual AString GetLogsPrefix(void) override { return "logs/"; }
	virtual AString GetPlayerPrefix(void) override { return "players/"; }
	virtual AString GetWorldDataPath(const AString & a_WorldName) override  { return "./" + a_WorldName; }
	virtual AString GetConfigPrefix(void) override     { return "./"; }
	virtual AString GetResourcePrefix(void) override   { return "./"; }
	virtual AString GetPluginConfigPrefix(const AString & a_Plugin) override { return "./"; }
	virtual AString GetPluginPrefix(const AString & a_Plugin) override     { return "Plugins/" + a_Plugin; }
	virtual AString GetPluginDataPrefix(const AString & a_Plugin) override { return "./"; }
	virtual AString GetDataPrefix(void) override       { return "./"; }
	virtual AString GetPrefabPrefix(void) override     { return "Prefabs/"; }
};

class cFileLayoutArmadillo : public cFileLayoutProvider
{
public:
	virtual AString GetLogsPrefix(void) override { return "Logs/"; }
	virtual AString GetPlayerPrefix(void) override { return "Players/"; }
	virtual AString GetWorldDataPath(const AString & a_WorldName) override  { return "Worlds/" + a_WorldName; }
	virtual AString GetConfigPrefix(void) override     { return "Config/"; }
	virtual AString GetResourcePrefix(void) override   { return "Resources/"; }
	virtual AString GetPluginConfigPrefix(const AString & a_Plugin) override { return "Config/"; }
	virtual AString GetPluginPrefix(const AString & a_Plugin) override     { return "Plugins/" + a_Plugin; }
	virtual AString GetPluginDataPrefix(const AString & a_Plugin) override { return "Data/"; }
	virtual AString GetDataPrefix(void) override       { return "Data/"; }
	virtual AString GetPrefabPrefix(void) override     { return "Prefabs/"; }
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
		if (s_Layout != nullptr)
		{
			// Don't reset it if it's already set
			return;
		}

		// Check for settings.ini in the working directory
		if (cFile::IsFile(FILE_IO_PREFIX "settings.ini"))
		{
			s_Layout = new cFileLayoutLegacy();
			return;
		}
		s_Layout = new cFileLayoutArmadillo();
	}

	static cFileLayoutProvider & Get()
	{
		// If it hasn't been set yet, we're probably in one of the Tools
		if (s_Layout == nullptr)
		{
			s_Layout = new cFileLayoutArmadillo();
		}
		return *s_Layout;
	}

private:
	static cFileLayoutProvider * s_Layout;
};
