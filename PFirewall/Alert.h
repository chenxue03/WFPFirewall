#pragma once

#define SUBLAYER_DISPLAY_NAME L"ALter Sublayer Display Name"
#define SUBLAYER_NAME L"ALter Sublayer Name"
#define FILTER_NAME L"ALter Filter Name"

// Alert �Ի���

class Alert : public CDialogEx
{
	DECLARE_DYNAMIC(Alert)

public:
	void InstallProvider(const GUID * providerKey, PCWSTR providerName, const GUID * subLayerKey, PCWSTR subLayerName);
	void BlockPortScanning(HANDLE engine, PCWSTR filterName, const GUID * providerKey, const GUID * subLayerKey);
	Alert(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Alert();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Alert };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
};
