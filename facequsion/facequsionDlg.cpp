
// facequsionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "facequsion.h"
#include "facequsionDlg.h"
#include "fusion_pixels.h"
#include "fusion_laplacian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CfacequsionDlg 对话框




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


// CfacequsionDlg 消息处理程序

BOOL CfacequsionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfacequsionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfacequsionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CfacequsionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
		Mat l8u = imread("cola1.bmp");
	Mat r8u = imread("cola2.bmp");

	imshow("left",l8u); 
	imshow("right",r8u);

	Mat_<Vec3f> l; l8u.convertTo(l,CV_32F,1.0/255.0);//Vec3f表示有三个通道，即 l[row][column][depth]
	Mat_<Vec3f> r; r8u.convertTo(r,CV_32F,1.0/255.0);
	/*****************    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;******************/
	/* Performs linear transformation on every source array element:
	dst(x,y,c) = scale*src(x,y,alpha)+beta.
	Arbitrary combination of input and output array depths are allowed
	(number of channels must be the same), thus the function can be used
	for type conversion */

	//create blend mask matrix m,左半脸全1，右半脸全0
	Mat_<float> m(l.rows,l.cols,0.0);					// 将m全部赋值为0
	m(Range::all(),Range(0,m.cols/2)) = 1.0;	// 取m全部行&[0,m.cols/2]列，赋值为1.0

	LaplacianBlending A(l,r,m, 2);
	Mat_<Vec3f> blend = A.blend();
//	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	imshow("blended",blend);

	waitKey(0);
	destroyAllWindows();
}

void CfacequsionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// load source image
	Mat src_img_1 = imread("cola1.bmp", IMREAD_GRAYSCALE);
	Mat src_img_2 = imread("cola2.bmp", IMREAD_GRAYSCALE);
	Mat greaterPixels = FusionGreaterPixel(src_img_1, src_img_2);
	Mat averagePixels = FusionAveragePixel(src_img_1, src_img_2);

}
