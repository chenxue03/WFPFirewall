#pragma once


// Process_Monitor �Ի���

class Process_Monitor : public CDialogEx
{
	DECLARE_DYNAMIC(Process_Monitor)

public:
	Process_Monitor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Process_Monitor();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = Process_Monitor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
