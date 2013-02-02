/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kalda
*/
#pragma once


namespace P3D
{

	class ISoundListener
	{
	public:
		virtual void SetPosition(Vec3 position) = 0;
		virtual void SetLookAt(Vec3 position) = 0;
		virtual void SetLookVector(Vec3 v) = 0;
		virtual void SetVelocity(Vec3 velocity) = 0;
		virtual inline Vec3 GetPosition() = 0;
		virtual inline Vec3 GetLookVector() = 0;
		virtual inline Vec3 GetVelocity() = 0;
	};

}