// PortFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PFirewall.h"
#include "PortFilter.h"
#include "afxdialogex.h"


// PortFilter �Ի���

IMPLEMENT_DYNAMIC(PortFilter, CDialogEx)

PortFilter::PortFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PortFilter, pParent)
{

}

PortFilter::~PortFilter()
{
}

void PortFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PortFilter, CDialogEx)
END_MESSAGE_MAP()


// PortFilter ��Ϣ�������
