//////////////////////////////////////////////////////////////////////////
//	Define.h

#ifndef DEFINE_H_HEADER
#define DEFINE_H_HEADER

#include "stdafx.h"
#include <Windows.h>

#pragma once

#define UM_DWBTN_MOUSEOVER		(WM_USER + 1001)
#define UM_DWBTN_MOUSEOUT		(WM_USER + 1002)
#define UM_DWBTN_CLICK			(WM_USER + 1003)

#define UPDATE_PATH				_T("autoupdate/autoupdate.xml.zip")

struct AniPoint
{
	int x;
	int y;
	int cx;
	int cy;
	int fx;
	int fy;
};

struct AniPointData
{
	int start;
	int size;
};

#endif