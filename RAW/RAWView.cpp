
// RAWView.cpp : implementation of the CRAWView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RAW.h"
#endif

#include "RAWDoc.h"
#include "RAWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRAWView

IMPLEMENT_DYNCREATE(CRAWView, CView)

BEGIN_MESSAGE_MAP(CRAWView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRAWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PIXELPOINTPROCESSING_ADD, &CRAWView::OnPixelpointprocessingAdd)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_SUB, &CRAWView::OnPixelpointprocessingSub)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_INVERSE, &CRAWView::OnPixelpointprocessingInverse)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_BINARIZATION, &CRAWView::OnPixelpointprocessingBinarization)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_LOGTRANSFORM, &CRAWView::OnPixelpointprocessingLogtransform)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_GAMMACORRECTION, &CRAWView::OnPixelpointprocessingGammacorrection)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_POSTERIZATION, &CRAWView::OnPixelpointprocessingPosterization)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_BITPLANESLICING, &CRAWView::OnPixelpointprocessingBitplaneslicing)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_CONTRASTSTRETCHING, &CRAWView::OnPixelpointprocessingContraststretching)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_STRETCHING, &CRAWView::OnHistogramprocessingStretching)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_ENDINSEARCH, &CRAWView::OnHistogramprocessingEndinsearch)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_EQUALIZATION, &CRAWView::OnHistogramprocessingEqualization)
	ON_COMMAND(ID_HISTOGRAMPROCESSING_SPECIFICATION, &CRAWView::OnHistogramprocessingSpecification)
	ON_COMMAND(ID_REGIONPROCESSING_CONVOLUTION, &CRAWView::OnRegionprocessingConvolution)
	ON_COMMAND(ID_PIXELPOINTPROCESSING_DISSOLVE, &CRAWView::OnPixelpointprocessingDissolve)
END_MESSAGE_MAP()

// CRAWView construction/destruction

CRAWView::CRAWView() noexcept
{
	// TODO: add construction code here

}

CRAWView::~CRAWView()
{
}

BOOL CRAWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRAWView drawing

void CRAWView::ShowRAWImg(CDC& pDC, CRAWDoc* pDoc, bool isOut)
{
	int height = pDoc->m_inH;
	int width = pDoc->m_inW;
	int histSize = pDoc->m_inHistSz;

	int img_i = 5;
	int img_j = 5;
	int hist_i = 5;
	int hist_j = 5 + 5 + width;

	UCHAR* rawImg = pDoc->m_inImg;
	UCHAR* histImg = pDoc->m_inHistImg;

	// 출력 이미지인 경우 변수 변경
	if (isOut)
	{
		height = pDoc->m_outH;
		width = pDoc->m_outW;
		histSize = pDoc->m_outHistSz;

		img_i = 5 + 25 + height;
		img_j = 5;
		hist_i = 5 + 25 + height;
		hist_j = 5 + 5 + width;

		rawImg = pDoc->m_outImg;
		histImg = pDoc->m_outHistImg;
	}

	UCHAR R, G, B;
	// 이미지 화면에 출력
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			R = G = B = rawImg[i * width + j];
			pDC.SetPixel(j + img_j, i + img_i, RGB(R, G, B));
		}
	}
	// 히스토그램 화면 출력
	for (int i = 0; i < histSize + 20; i++) {
		for (int j = 0; j < histSize; j++) {
			R = G = B = histImg[i * histSize + j];
			pDC.SetPixel(j + hist_j, i + hist_i, RGB(R, G, B));
		}
	}
}

void CRAWView::OnDraw(CDC* pDC)
{
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	/*int i, dan = 7;
	CString str;
	for (i = 1; i <= 9; i++)
	{
		str.Format(_T("%d x %d = %d"), dan, i, dan * i);
		pDC->TextOut(20, 20 * i, str);
	}*/

	CRect viewRect;
	CDC memDC;
	CBitmap bit;

	GetClientRect(viewRect);

	int height = viewRect.Height();
	int width = viewRect.Width();

	memDC.CreateCompatibleDC(pDC);
	bit.CreateCompatibleBitmap(pDC, width, height);
	memDC.SelectObject(&bit);
	memDC.Rectangle(0, 0, width, height);

	if (pDoc->m_inSz != 0) {
		this->ShowRAWImg(memDC, pDoc, false);
	}
	if (pDoc->m_outSz != 0) {
		this->ShowRAWImg(memDC, pDoc, true);
	}

	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	bit.DeleteObject();
}


// CRAWView printing


void CRAWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRAWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRAWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRAWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRAWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRAWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRAWView diagnostics

#ifdef _DEBUG
void CRAWView::AssertValid() const
{
	CView::AssertValid();
}

void CRAWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRAWDoc* CRAWView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRAWDoc)));
	return (CRAWDoc*)m_pDocument;
}
#endif //_DEBUG

void CRAWView::OnPixelpointprocessingAdd()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingAdd();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingSub()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingSub();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingInverse()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingInverse();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingBinarization()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingBinarization();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingLogtransform()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingLogtransform();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingGammacorrection()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingGammacorrection();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingPosterization()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingPosterization();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingBitplaneslicing()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingBitplaneslicing();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingContraststretching()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingContraststretching();
	Invalidate(TRUE);
}


void CRAWView::OnHistogramprocessingStretching()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnHistogramprocessingStretching();
	Invalidate(TRUE);
}


void CRAWView::OnHistogramprocessingEndinsearch()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnHistogramprocessingEndinsearch();
	Invalidate(TRUE);
}


void CRAWView::OnHistogramprocessingEqualization()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnHistogramprocessingEqualization();
	Invalidate(TRUE);
}


void CRAWView::OnHistogramprocessingSpecification()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnHistogramprocessingSpecification();
	Invalidate(TRUE);
}


void CRAWView::OnRegionprocessingConvolution()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnRegionprocessingConvolution();
	Invalidate(TRUE);
}


void CRAWView::OnPixelpointprocessingDissolve()
{
	// TODO: Add your command handler code here
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->OnPixelpointprocessingDissolve();
	Invalidate(TRUE);
}
