
// facequsion.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfacequsionApp:
// �йش����ʵ�֣������ facequsion.cpp
//

class CfacequsionApp : public CWinAppEx
{
public:
	CfacequsionApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfacequsionApp theApp;