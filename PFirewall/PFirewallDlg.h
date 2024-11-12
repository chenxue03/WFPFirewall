
// PFirewallDlg.h : ͷ�ļ�
//

#pragma once

#include "common.h"

#define SUBLAYER_NAME L"SUBLAYER_NAME"
#define SUBLAYER_DISPLAY_NAME L"SUBLAYER_DISPLAY_NAME"


// CPFirewallDlg �Ի���
class CPFirewallDlg : public CDialogEx
{
// ����
public:
	CPFirewallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PFIREWALL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonStartFilter();
	afx_msg void OnBnClickedButton8();
};
