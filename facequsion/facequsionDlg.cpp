
// facequsionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "facequsion.h"
#include "facequsionDlg.h"
#include "fusion_pixels.h"
#include "fusion_laplacian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CfacequsionDlg �Ի���




CfacequsionDlg::CfacequsionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfacequsionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfacequsionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfacequsionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CfacequsionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CfacequsionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CfacequsionDlg ��Ϣ�������

BOOL CfacequsionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CfacequsionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CfacequsionDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CfacequsionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CfacequsionDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		Mat l8u = imread("cola1.bmp");
	Mat r8u = imread("cola2.bmp");

	imshow("left",l8u); 
	imshow("right",r8u);

	Mat_<Vec3f> l; l8u.convertTo(l,CV_32F,1.0/255.0);//Vec3f��ʾ������ͨ������ l[row][column][depth]
	Mat_<Vec3f> r; r8u.convertTo(r,CV_32F,1.0/255.0);
	/*****************    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;******************/
	/* Performs linear transformation on every source array element:
	dst(x,y,c) = scale*src(x,y,alpha)+beta.
	Arbitrary combination of input and output array depths are allowed
	(number of channels must be the same), thus the function can be used
	for type conversion */

	//create blend mask matrix m,�����ȫ1���Ұ���ȫ0
	Mat_<float> m(l.rows,l.cols,0.0);					// ��mȫ����ֵΪ0
	m(Range::all(),Range(0,m.cols/2)) = 1.0;	// ȡmȫ����&[0,m.cols/2]�У���ֵΪ1.0

	LaplacianBlending A(l,r,m, 2);
	Mat_<Vec3f> blend = A.blend();
//	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	imshow("blended",blend);

	waitKey(0);
	destroyAllWindows();
}

void CfacequsionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// load source image
	Mat src_img_1 = imread("cola1.bmp", IMREAD_GRAYSCALE);
	Mat src_img_2 = imread("cola2.bmp", IMREAD_GRAYSCALE);
	Mat greaterPixels = FusionGreaterPixel(src_img_1, src_img_2);
	Mat averagePixels = FusionAveragePixel(src_img_1, src_img_2);

}
