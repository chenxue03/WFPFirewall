#pragma once


// ProcessMonitor �Ի���

class ProcessMonitor : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessMonitor)

public:
	ProcessMonitor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ProcessMonitor();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ProcessMonitor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedBtngetfolder();
	void OnBnClickedBtnstart();
	void OnBnClickedBtnstop();
	void OnBnClickedBtnaddcallouts();
	void OnBnClickedBtndelcallouts();
};
