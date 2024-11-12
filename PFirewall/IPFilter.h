#pragma once

#include <vector>
#include <utility>
#include "common.h"

// Э������
#define IP_PROTOCOL 0
#define ICMP_PROTOCOL 1
#define TCP_PROTOCOL 2
#define UDP_PROTOCOL 3
// ����
#define IN 1
#define OUT 0
// ����
#define PERMIT 1
#define BLOCK 0
#define FILTER_DISPLAY_NAME L"filter display name PFirewall"

// IPFilter �Ի���

// ������Ϣ
typedef struct _FILTER_INFO {
	UINT32 source_ip;//ԴIP
	UINT32 dest_ip;//Ŀ��IP
	UINT16 source_port;//Դ�˿�
	UINT16 dest_port;//Ŀ��IP
	UINT16 protocol_type;//Э������
	UINT16 direction;//���ݰ��ķ���
	UINT16 action;//���˶���
	CString str_source_ip;
	CString str_dest_ip;
	CString str_source_port;
	CString str_dest_port;
	CString str_protocol_type;
	CString str_direction;
	CString str_action;
}FILTER_INFO;

class IPFilter : public CDialogEx
{
	DECLARE_DYNAMIC(IPFilter)

public:
	IPFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~IPFilter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void getFilterInfo(FILTER_INFO &);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
