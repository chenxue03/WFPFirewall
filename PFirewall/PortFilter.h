#pragma once


// PortFilter �Ի���

class PortFilter : public CDialogEx
{
	DECLARE_DYNAMIC(PortFilter)

public:
	PortFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PortFilter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PortFilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
