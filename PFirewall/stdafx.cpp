
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// PFirewall.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

typedef struct Rule {
	CString sourceIP;
	CString destinIP;
	int type;
	int sourcePort;
	int destinPort;
	bool inout;
	bool operation;
}Rule;






