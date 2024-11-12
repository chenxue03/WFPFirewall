
// PFirewallDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PFirewall.h"
#include "PFirewallDlg.h"
#include "afxdialogex.h"
#include "string.h"
#include "IPFilter.h"
#include "ProcessMonitor.h"
#include "Alert.h"
#include "IPFilter.h"
#include <sstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EXIT_ON_ERROR(fnName) \
   if (result != ERROR_SUCCESS) \
   { \
     strError.Format(L"%s failed (%08x)", fnName,result);\
	 MessageBox(strError,_T("ERROR"),MB_OK);\
     FwpmEngineClose0(engine);\
     EndDialog(0);\
     return;\
   }


// ȫ��������Ϣ
extern std::vector<std::pair<UINT64, FILTER_INFO>> filter_info_v;
HANDLE shareEngine = NULL;
GUID subLayerGUID = {0};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPFirewallDlg �Ի���



CPFirewallDlg::CPFirewallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PFIREWALL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPFirewallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPFirewallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPFirewallDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPFirewallDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPFirewallDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPFirewallDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CPFirewallDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON2, &CPFirewallDlg::OnBnClickedButtonStartFilter)
	ON_BN_CLICKED(IDC_BUTTON8, &CPFirewallDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CPFirewallDlg ��Ϣ�������

BOOL CPFirewallDlg::OnInitDialog()
{
	//openEngine();
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������	
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT1);
	CRect rect;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	//m_programLangList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	//m_programLangList.SetExtendedStyle(m_programLangList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	CListCtrl* pList;
	pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->InsertColumn(0, _T("���"), LVCFMT_CENTER, 40, 0);
	pList->InsertColumn(1, _T("ԴIP"), LVCFMT_CENTER, 100, 0);
	pList->InsertColumn(2, _T("Ŀ��IP"), LVCFMT_CENTER, 100, 0);
	pList->InsertColumn(3, _T("Э������"), LVCFMT_CENTER, 80, 0);
	pList->InsertColumn(4, _T("Դ�˿�"), LVCFMT_CENTER, 70, 0);
	pList->InsertColumn(5, _T("Ŀ�Ķ˿�"), LVCFMT_CENTER, 80, 0);
	pList->InsertColumn(6, _T("�� or ��"), LVCFMT_CENTER, 80, 0);
	pList->InsertColumn(7, _T("��������"), LVCFMT_CENTER, 80, 0);

	//pList->InsertItem(0, _T("0.0.0.0"));
	//pList->SetItemText(0, 1, _T("1.1.1.1"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPFirewallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPFirewallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPFirewallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPFirewallDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IPFilter  Dlg;
	Dlg.DoModal();
	/*CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString str;
	pBoxOne->GetWindowText(str);//��ȡedit��ֵ
	MessageBox(str, _T("�������н��"), MB_OK);
	str.ReleaseBuffer();*/
}

void CPFirewallDlg::OnBnClickedButton2()
{
	MessageBox(L"dddd", _T("d"), MB_OK);
}


void CPFirewallDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ѩ2ʵ��
}


void CPFirewallDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���̼�� process monitor
	ProcessMonitor  Dlg;
	Dlg.DoModal();
}


void CPFirewallDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���籨��
	Alert Dlg;
	Dlg.DoModal();
}



void CPFirewallDlg::OnBnClickedButton6()
{
	DWORD result;
	HANDLE engineHandle = shareEngine;
	for (auto it = filter_info_v.begin(); it != filter_info_v.end(); ++it) {
		result = FwpmFilterDeleteById0(engineHandle, (*it).first);
		if (result != ERROR_SUCCESS)
		{
			CString str;
			str.Format(L"FwpmSubLayerDeleteByKey0 failed (%08x)", result);
			MessageBox(str, _T("���˹���ɾ��ʧ��"), MB_OK);
			FwpmEngineClose0(engineHandle);
			break;
		}
	}
CLEANUP:
	result = FwpmSubLayerDeleteByKey0(engineHandle, &subLayerGUID);
	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmSubLayerDeleteByKey0 failed (%08x)", result);
		MessageBox(str, _T("���˲�ɾ��ʧ��"), MB_OK);
		FwpmEngineClose0(engineHandle);
		return;
	}
	result = FwpmEngineClose0(engineHandle);
	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmEngineClose0 failed (%08x)", result);
		MessageBox(str, _T("��������ر�ʧ��"), MB_OK);
		FwpmEngineClose0(engineHandle);
		return;
	}
	MessageBox(L"�Ѿ��ɹ�ͣ�����й��˹���", _T("��������رճɹ�"), MB_OK);
	filter_info_v.clear();
	filter_info_v.resize(0);
	CButton *pBtnStart, *pBtnStop;
	pBtnStart = (CButton*)GetDlgItem(IDC_BUTTON2);
	pBtnStop = (CButton*)GetDlgItem(IDC_BUTTON6);
	pBtnStart->EnableWindow(TRUE);
	pBtnStop->EnableWindow(FALSE);
}

void CPFirewallDlg::OnBnClickedButtonStartFilter()
{
	HANDLE engineHandle = NULL;
	FWPM_SUBLAYER0 fwpFilterSubLayer;
	RPC_STATUS rpcStatus = RPC_S_OK;
	DWORD  result = ERROR_SUCCESS;
	DWORD fwpTxStatus = ERROR_SUCCESS;

	printf("initial the sublayer...\n");
	memset(&fwpFilterSubLayer, 0, sizeof(fwpFilterSubLayer));
	rpcStatus = UuidCreate(&fwpFilterSubLayer.subLayerKey);
	if (RPC_S_OK != rpcStatus)
	{
		CString str;
		str.Format(L"UuidCreate failed (%08x).\n", result);
		MessageBox(str, _T("���ɹ��˲�subLayerKeyʧ��"), MB_OK);
		return;
	}
	fwpFilterSubLayer.displayData.name = L"MyFilterSublayer";
	fwpFilterSubLayer.displayData.description = L"My filter sublayer";
	fwpFilterSubLayer.flags = 0;
	fwpFilterSubLayer.weight = 0x100;

	printf("Opening filter engine.\n");
	result = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &engineHandle);
	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmEngineOpen0 failed (%08x).\n", result);
		MessageBox(str, _T("���������ʧ��"), MB_OK);
		return;
	}
	shareEngine = engineHandle;
	printf("Adding filter sublayer.\n");
	result = FwpmSubLayerAdd0(engineHandle, &fwpFilterSubLayer, NULL);

	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmSubLayerAdd0 failed (%08x)", result);
		MessageBox(str, _T("���˲����ʧ��"), MB_OK);
		FwpmEngineClose0(engineHandle);
		return;
	}
	subLayerGUID = fwpFilterSubLayer.subLayerKey;
	for (auto it = filter_info_v.begin(); it!=filter_info_v.end(); ++it) {
		FILTER_INFO info = (*it).second;
		FWPM_FILTER0      fwpFilter;
		FWPM_FILTER_CONDITION0 fwpConditions[4] = { 0 };
		CString str;
		str.Format(L"source_ip %d ,dest_ip %d,s_p %d,d_p %d,dire %d,action %d, type %d", info.source_ip, info.dest_ip, info.source_port, info.dest_port, info.direction, info.action, info.protocol_type);
		//MessageBox(str, _T("����"), MB_OK);
		int count = 0;
		switch (info.protocol_type)
		{
		case IP_PROTOCOL:
			RtlZeroMemory(&fwpFilter, sizeof(FWPM_FILTER0));
			count = 0;
			fwpFilter.subLayerKey = fwpFilterSubLayer.subLayerKey;
			if (info.direction == IN) {
				fwpFilter.layerKey = FWPM_LAYER_INBOUND_IPPACKET_V4;
			}
			else	fwpFilter.layerKey = FWPM_LAYER_OUTBOUND_IPPACKET_V4;
			if(info.action == BLOCK)	fwpFilter.action.type = FWP_ACTION_BLOCK;
			else	fwpFilter.action.type = FWP_ACTION_PERMIT;
			fwpFilter.weight.type = FWP_EMPTY;
			fwpFilter.displayData.name = FILTER_DISPLAY_NAME;
			if (info.dest_ip) {
				if(info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.dest_ip;
				++count;
				//MessageBox(L"1", _T("����"), MB_OK);
			}
			if (info.source_ip) {
				if(info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.source_ip;
				++count;
				//MessageBox(L"2", _T("����"), MB_OK);
			}
			fwpFilter.numFilterConditions = count;
			fwpFilter.filterCondition = fwpConditions;
			break;
		case ICMP_PROTOCOL:
			count = 0;
			RtlZeroMemory(&fwpFilter, sizeof(FWPM_FILTER0));
			fwpFilter.subLayerKey = fwpFilterSubLayer.subLayerKey;
			if (info.direction == IN)	fwpFilter.layerKey = FWPM_LAYER_INBOUND_ICMP_ERROR_V4;
			else	fwpFilter.layerKey = FWPM_LAYER_OUTBOUND_ICMP_ERROR_V4;
			if (info.action == BLOCK)	fwpFilter.action.type = FWP_ACTION_BLOCK;
			else	fwpFilter.action.type = FWP_ACTION_PERMIT;
			fwpFilter.weight.type = FWP_EMPTY;
			fwpFilter.displayData.name = FILTER_DISPLAY_NAME;
			if (info.dest_ip) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.dest_ip;
				++count;
			}
			if (info.source_ip) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.source_ip;
				++count;
			}
			fwpFilter.numFilterConditions = count;
			fwpFilter.filterCondition = fwpConditions;
			break;
		case TCP_PROTOCOL:
			count = 0;
			RtlZeroMemory(&fwpFilter, sizeof(FWPM_FILTER0));
			fwpFilter.subLayerKey = fwpFilterSubLayer.subLayerKey;
			if (info.direction == IN)	fwpFilter.layerKey = FWPM_LAYER_INBOUND_TRANSPORT_V4;
			else	fwpFilter.layerKey = FWPM_LAYER_OUTBOUND_TRANSPORT_V4;
			if (info.action == BLOCK)	fwpFilter.action.type = FWP_ACTION_BLOCK;
			else	fwpFilter.action.type = FWP_ACTION_PERMIT;
			fwpFilter.weight.type = FWP_EMPTY;
			fwpFilter.displayData.name = FILTER_DISPLAY_NAME;
			if (info.dest_ip) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.dest_ip;
				++count;
			}
			if (info.source_ip) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.source_ip;
				++count;
			}
			if (info.dest_port) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT16;
				fwpConditions[count].conditionValue.uint16 = info.dest_port;
				++count;
			}
			if (info.source_port) {
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT16;
				fwpConditions[count].conditionValue.uint32 = info.source_port;
				++count;
			}
			fwpFilter.numFilterConditions = count;
			fwpFilter.filterCondition = fwpConditions;
			break;
		case UDP_PROTOCOL:
			count = 0;
			RtlZeroMemory(&fwpFilter, sizeof(FWPM_FILTER0));
			fwpFilter.subLayerKey = fwpFilterSubLayer.subLayerKey;
			fwpFilter.layerKey = FWPM_LAYER_DATAGRAM_DATA_V4;;
			if (info.action == BLOCK)	fwpFilter.action.type = FWP_ACTION_BLOCK;
			else	fwpFilter.action.type = FWP_ACTION_PERMIT;
			fwpFilter.weight.type = FWP_EMPTY;
			fwpFilter.displayData.name = FILTER_DISPLAY_NAME;
			if (info.dest_ip) {
				//fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.dest_ip;
				++count;
			}
			if (info.source_ip) {
				//fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT32;
				fwpConditions[count].conditionValue.uint32 = info.source_ip;
				++count;
			}
			if (info.dest_port) {
				//fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT16;
				fwpConditions[count].conditionValue.uint16 = info.dest_port;
				++count;
			}
			if (info.source_port) {
				//fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
				if (info.direction == IN)	fwpConditions[count].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
				else fwpConditions[count].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
				fwpConditions[count].matchType = FWP_MATCH_EQUAL;
				fwpConditions[count].conditionValue.type = FWP_UINT16;
				fwpConditions[count].conditionValue.uint32 = info.source_port;
				++count;
			}
			fwpFilter.numFilterConditions = count;
			fwpFilter.filterCondition = fwpConditions;
			break;
		default:
			break;
		}
		fwpTxStatus = FwpmTransactionBegin0(engineHandle, NULL);
		if (fwpTxStatus != ERROR_SUCCESS)
		{
			CString str;
			str.Format(L"FwpmTransactionBegin0 failed (%08x)", result);
			MessageBox(str, _T("��������潻��ʧ��"), MB_OK);
			FwpmEngineClose0(engineHandle);
			return;
		}

		result = FwpmFilterAdd0(engineHandle, &fwpFilter, NULL, &(*it).first);
		if (result != ERROR_SUCCESS)
		{
			CString str;
			str.Format(L"FwpmFilterAdd0 failed (%08x)", result);
			MessageBox(str, _T("���˹������ʧ��"), MB_OK);
			FwpmEngineClose0(engineHandle);
			return;
		}

		result = FwpmTransactionCommit0(engineHandle);
		if (result != ERROR_SUCCESS)
		{
			CString str;
			str.Format(L"FwpmTransactionCommit0 failed (%08x)", result);
			MessageBox(str, _T("�����ύʧ��"), MB_OK);
			FwpmTransactionAbort0(engineHandle);
			FwpmEngineClose0(engineHandle);
			EndDialog(0);
			return;
		}
	}
	MessageBox(L"�ɹ��������й��˹���", _T("SUCCESS"), MB_OK);
	CButton *pBtnStart,*pBtnStop;
	pBtnStart = (CButton*)GetDlgItem(IDC_BUTTON2);
	pBtnStop = (CButton*)GetDlgItem(IDC_BUTTON6);
	pBtnStart->EnableWindow(FALSE);
	pBtnStop->EnableWindow(TRUE);
	return;
	for (auto it = filter_info_v.begin(); it!=filter_info_v.end(); ++it) {
		result = FwpmFilterDeleteById0(engineHandle, (*it).first);
		if (result != ERROR_SUCCESS)
		{
			CString str;
			str.Format(L"FwpmSubLayerDeleteByKey0 failed (%08x)", result);
			MessageBox(str, _T("���˹���ɾ��ʧ��"), MB_OK);
			FwpmEngineClose0(engineHandle);
			return;
		}
	}
CLEANUP:
	result = FwpmSubLayerDeleteByKey0(engineHandle, &fwpFilterSubLayer.subLayerKey);
	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmSubLayerDeleteByKey0 failed (%08x)", result);
		MessageBox(str, _T("���˲�ɾ��ʧ��"), MB_OK);
		FwpmEngineClose0(engineHandle);
		return;
	}
	
	result = FwpmEngineClose0(engineHandle);
	if (result != ERROR_SUCCESS)
	{
		CString str;
		str.Format(L"FwpmEngineClose0 failed (%08x)", result);
		MessageBox(str, _T("��������ر�ʧ��"), MB_OK);
	}
}

//��ʾȫ�����˹���
void CPFirewallDlg::OnBnClickedButton8()
{
	CListCtrl* pList;
	pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	
	pList->DeleteAllItems();
	for (int i = 0;i<filter_info_v.size();++i) {
		FILTER_INFO info = filter_info_v[i].second;
		
		/*
		CString str;
		str.Format(L"%s", info.str_dest_port);
		MessageBox(str, _T("xxxx", MB_OK));*/
		std::stringstream stream;
		std::string strTemp;
		stream << i+1;
		strTemp = stream.str();
		pList->InsertItem(i, (CString)strTemp.c_str());
		//pList->SetItemText(i, 0, (CString)strTemp.c_str());
		pList->SetItemText(i, 1, info.str_source_ip);
		pList->SetItemText(i, 2, info.str_dest_ip);
		pList->SetItemText(i, 3, info.str_protocol_type);
		pList->SetItemText(i, 4, info.str_source_port);
		pList->SetItemText(i, 5, info.str_dest_port);
		pList->SetItemText(i, 6, info.str_direction);
		pList->SetItemText(i, 7, info.str_action);
	}
}
