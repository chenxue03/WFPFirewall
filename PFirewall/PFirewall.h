
// PFirewall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPFirewallApp: 
// �йش����ʵ�֣������ PFirewall.cpp
//

class CPFirewallApp : public CWinApp
{
public:
	CPFirewallApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPFirewallApp theApp;