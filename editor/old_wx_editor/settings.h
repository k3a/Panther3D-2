#pragma once

#include <vector>

namespace P3D
{
	struct sEditorSetting
	{
		wchar* name;
		wchar* value;
	};

	/** Class for editor settings. You can save/restore setting identified by string name. */
	class Settings
	{
	public:
		/** Get string setting. Name (case sensitive) must be max. 31 chars long! */
		static const wchar* GetString(const wchar* name, const wchar* default=_W(""));
		/** Get float setting. Name (case sensitive) must be max. 31 chars long! */
		static float GetFloat(const wchar* name, float default=0.0f);
		/** Get int setting. Name (case sensitive) must be max. 31 chars long! */
		static int GetInt(const wchar* name, int default=0);
		/** Get long setting. Name (case sensitive) must be max. 31 chars long! */
		static long GetLong(const wchar* name, long default=0);
		/** Get bool setting. Name (case sensitive) must be max. 31 chars long! */
		static bool GetBool(const wchar* name, bool default=false);

		/** Set string setting. Name (case sensitive) must be max. 31 chars long! */
		static void Set(const wchar* name, const wchar* data);
		/** Set float setting. Name (case sensitive) must be max. 31 chars long! */
		static void Set(const wchar* name, float data);
		/** Set int setting. Name (case sensitive) must be max. 31 chars long! */
		static void Set(const wchar* name, int data);
		/** Set long setting. Name (case sensitive) must be max. 31 chars long! */
		static void Set(const wchar* name, long data);
		/** Set bool setting. Name (case sensitive) must be max. 31 chars long! */
		static void Set(const wchar* name, bool data);

		/** Load settings from file. */
		static bool Load(const wchar* filename);
		/** Save settings to file. */
		static bool Save(const wchar* filename);
	private:
		static std::vector<sEditorSetting> s_esettings;
	};
}