/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once
#include "interface.h"

namespace P3D
{

	///Type of console message
	enum eMsgType
	{
		/// Information
		MSG_INFO=0,
		/// Debug message
		MSG_DBG,
		/// Error
		MSG_ERR,
		/// Error in messagebox, but allowing to continue
		MSG_ERR_BOX,
		/// Fatal error, cannot continue
		MSG_ERR_FATAL,
		/// Informational message box
		MSG_BOX
	};

	///Console interface. Console is implemented in engine.
	///Every module can use instance (singleton) created by engine.
	class IConsole : public IBaseInterface
	{
	public:
		/// Add message to the console.
		virtual void AddMsg(eMsgType type, const wchar* text, ...)=0;
		/// Add extended message to the console.
		virtual void AddMsgEx(eMsgType type, const wchar* scopeName, const wchar* text, ...)=0;
		/// Get current number of lines in the console.
		virtual unsigned int GetLineCount()const=0;
		/// Get specific console line string.
		virtual const wchar* GetLine(unsigned int lineNum)=0;
		/// Get whole console string.
		virtual const wchar* GetString()const=0;
	};

};
