// Process_Monitor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PFirewall.h"
#include "Process_Monitor.h"
#include "afxdialogex.h"


// Process_Monitor �Ի���

IMPLEMENT_DYNAMIC(Process_Monitor, CDialogEx)

Process_Monitor::Process_Monitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(Process_Monitor, pParent)
{

}

Process_Monitor::~Process_Monitor()
{
}

void Process_Monitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Process_Monitor, CDialogEx)
END_MESSAGE_MAP()


// Process_Monitor ��Ϣ�������
