#include "stdafx.h"
#include "MonitorGraphUnit.h"


inline LONG GetRectHeight(RECT rect) {
	return rect.bottom - rect.top;
}

inline LONG GetRectWidth(RECT rect) {
	return rect.right - rect.left;
}

inline void AddVector(double *x, double *y, double ax = 0, double ay = 0) {
	(*x) += ax;
	(*y) += ay;
	double length = sqrt((*x) * (*x) + (*y) * (*y));
	(*x) /= length;
	(*y) /= length;
}

COLORREF AddColor(COLORREF a, COLORREF b) { 
	return RGB(
		max(0, min(GetRValue(a) + GetRValue(b), 255)),
		max(0, min(GetGValue(a) + GetGValue(b), 255)),
		max(0, min(GetBValue(a) + GetBValue(b), 255))
	);
}

CMonitorGraphUnit::stHWNDtoTHIS CMonitorGraphUnit::_hWndToThis;
int CMonitorGraphUnit::_childCnt = -1;

CMonitorGraphUnit::CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, WCHAR* title, COLORREF backColor , TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight, int iColumnNum)
{
	WCHAR szWindowClass[] = L"childWindow";
	if (_childCnt == -1) {
		memset(&_hWndToThis, 0, sizeof(_hWndToThis));
		_childCnt = 0;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(this);
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = NULL;
		RegisterClassExW(&wcex);
	}

	_hWnd = CreateWindowW(szWindowClass, title, WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_BORDER,
		iPosX, iPosY, iWidth, iHeight, hWndParent, NULL, hInstance, NULL);

	_childCnt++;

	_pos.x = iPosX;
	_pos.y = iPosY;
	_size.cx = iWidth;
	_size.cy = iHeight;
	_hWndParent = hWndParent;
	_hInstance = hInstance;
	_enType = enType;
	_backColor = backColor;
	PutThis();

	_columnArray = new stColumnInfo[iColumnNum];
	memset(_columnArray, NULL, sizeof(stColumnInfo) * iColumnNum);
	_iDataColumnNum = iColumnNum;

	_graphUIPen = CreatePen(PS_SOLID, 1, AddColor(RGB(100, 100, 100), backColor));

	GetClientRect(_hWnd, &_graphRect);
	_titleRect = _graphRect;
	_titleRect.bottom = 30;
	_graphRect.top = 30;

	if (enType == LINE_MULTI || enType == PIE) {
		_multiDataInfoRect = _graphRect;
		_graphRect.right -= 100;
		_multiDataInfoRect.left = _graphRect.right;
	}
	
	else if (enType == LINE_SINGLE || enType == BAR_SINGLE_HORZ)
		SetDataColumnInfo(1, 1);
	else {
		_multiDataInfoRect = _graphRect;
		_graphRect.bottom -= 20;
		_multiDataInfoRect.top = _graphRect.bottom;
	}
	SetMultiLineColor();

	_backBrush = CreateSolidBrush(backColor);
	_titleBrush = CreateSolidBrush(AddColor(backColor, RGB(25, 25, 25)));
	_barBrush = CreateSolidBrush(RGB(50, 200, 50));

	wcscpy(_srzTitle, L" ");
	wcscat(_srzTitle, title);

	_titleFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
	_graphUIFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
	_barGraphDataFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{
	delete[] _columnArray;
}

LRESULT CALLBACK CMonitorGraphUnit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	CMonitorGraphUnit* temp;
	HBRUSH Brush;

	switch (message) {
	case WM_PAINT: 
		{
			hdc = BeginPaint(hWnd, &ps);
			temp = GetThis(hWnd);
			temp->DrawGraphTitle(hdc);

			switch (temp->_enType) {
			case LINE_SINGLE:
				temp->DrawGraphInterface(hdc);
				temp->DrawLineSingleGraph(hdc);
				break;
			case BAR_SINGLE_HORZ:
				temp->DrawGraphInterface(hdc);
				temp->DrawBarSingleGraph(hdc);
				break;
			case LINE_MULTI:
				temp->DrawGraphInterface(hdc);
				temp->DrawLineMultiGraph(hdc);
				break;
			case BAR_COLUMN_HORZ:
				temp->DrawGraphInterface(hdc);
				temp->DrawBarMultiGraph(hdc);
				break;
			case PIE:
				temp->DrawGraphBackground(hdc);
				temp->DrawPieGraph(hdc);
				temp->DrawMultiLineMemberList(hdc);
				break;
			}
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		temp = GetThis(hWnd);
		temp->ReleaseAllObject();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CMonitorGraphUnit::SetMultiLineColor()
{
	_multiLineGraphColor[enWHITE] =		RGB(250, 250, 250);
	_multiLineGraphColor[enRED] =		RGB(250, 50, 50);
	_multiLineGraphColor[enBLUE] =		RGB(150, 150, 250);
	_multiLineGraphColor[enGREEN] =		RGB(50, 250, 50);
	_multiLineGraphColor[enMAGENTA] =	RGB(250, 50, 250);
	_multiLineGraphColor[enCYAN] =		RGB(50, 250, 250);
	_multiLineGraphColor[enYELLOW] =	RGB(250, 250, 50);
	_multiLineGraphColor[enBROWN] =		RGB(0x9a, 0x40, 0x38);
	_multiLineGraphColor[enGRAY] =		RGB(125, 125, 125);
	_multiLineGraphColor[enORANGE] =	RGB(250, 125, 50);

	for (int i = 0;i < enCOLOR_NUM;i++) {
		_multiLineBrush[i] = CreateSolidBrush(_multiLineGraphColor[i]);
		_multiLinePen[i] = CreatePen(PS_SOLID,2, _multiLineGraphColor[i]);
	}
}

void CMonitorGraphUnit::DrawGraphTitle(HDC hdc)
{
	FillRect(hdc, &_titleRect, _titleBrush);

	HFONT oldFont = (HFONT)SelectObject(hdc, _titleFont);
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, AddColor(_backColor ,RGB(200,200,200)));
	DrawText(hdc, _srzTitle, -1, &_titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	SelectObject(hdc, oldFont);
	SetBkMode(hdc, oldMode);
	SetTextColor(hdc, oldColor);
}

void CMonitorGraphUnit::DrawGraphBackground(HDC hdc) {
	FillRect(hdc, &_graphRect, _backBrush);
}

void CMonitorGraphUnit::DrawGraphInterface(HDC hdc)
{
	DrawGraphBackground(hdc);
	LONG width = GetRectWidth(_graphRect);
	LONG height = GetRectHeight(_graphRect);
	height /= 5;
	WCHAR buffer[256];

	HFONT oldFont = (HFONT)SelectObject(hdc, _graphUIFont);
	HPEN oldPen = (HPEN)SelectObject(hdc, _graphUIPen);
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, AddColor(_backColor, RGB(125, 125, 125)));
	for (int i = 0;i <= 4;i++) {
		MoveToEx(hdc, 0, 30 + i * height, NULL);
		LineTo(hdc, width, 30 + i * height);
		_itow(100 - (i * 20), buffer, 10);
		TextOut(hdc, 3, 30 + i * height, buffer, wcslen(buffer));
	}
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldFont);
	SetBkMode(hdc, oldMode);
	SetTextColor(hdc, oldColor);
}

void CMonitorGraphUnit::DrawLineSingleGraph(HDC hdc, int arrayIndex)
{
	LONG width = GetRectWidth(_graphRect);
	LONG height = GetRectHeight(_graphRect);

	int data;
	_columnArray[arrayIndex]._DataArray->peek(0, &data);

	HPEN graphPen = _multiLinePen[arrayIndex];
	HPEN oldPen = (HPEN)SelectObject(hdc, graphPen);
	MoveToEx(hdc, 0, (height - 30) * 0.01 * (100 - data) + 30, NULL);

	for (int i = 1;i < _columnArray[arrayIndex]._DataArray->getCount();i++) {
		_columnArray[arrayIndex]._DataArray->peek(i, &data);
		LineTo(hdc, width * ((double)i / (dfMAXDATA - 2)), height* 0.01 * (100 - data) + 30);
	}

	SelectObject(hdc,oldPen);
}

void CMonitorGraphUnit::DrawBarSingleGraph(HDC hdc, int arrayIndex)
{
	int data;
	LONG width = GetRectWidth(_graphRect) / _iDataColumnNum;
	LONG height = GetRectHeight(_graphRect);

	_columnArray[arrayIndex]._DataArray->peek(_columnArray[arrayIndex]._DataArray->getCount() - 1, &data);

	if (data < 0) return;

	RECT bar;
	bar.bottom = _graphRect.bottom;
	bar.top = _graphRect.top + height * 0.01 * (100 - data);

	bar.right = _graphRect.right - width * (_iDataColumnNum - arrayIndex - 1) - width / 5;
	bar.left = _graphRect.left + width * arrayIndex + width / 5;

	FillRect(hdc, &bar, _barBrush);

	UINT oldTextAlign = SetTextAlign(hdc, TA_CENTER);
	COLORREF oldColor = SetTextColor(hdc, RGB(255,255,255));
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	HPEN oldPen = (HPEN)SelectObject(hdc, _barGraphDataFont);

	WCHAR buffer[256];
	_itow(data, buffer, 10);

	TextOut(hdc, _graphRect.left + width * (arrayIndex + 0.5), min(_graphRect.bottom - 25, bar.top + 5), buffer, wcslen(buffer));

	SetTextAlign(hdc, oldTextAlign);
	SetTextColor(hdc, oldColor);
	SetBkMode(hdc, oldMode);
	SelectObject(hdc, oldPen);
}

void CMonitorGraphUnit::DrawLineMultiGraph(HDC hdc)
{
	for (int i = 0;i < _iDataColumnNum;i++) {
		if (_columnArray[i]._DataArray == NULL) continue;
		this->DrawLineSingleGraph(hdc, i);
	}

	this->DrawMultiLineMemberList(hdc);
}

void CMonitorGraphUnit::DrawMultiLineMemberList(HDC hdc)
{
	FillRect(hdc, &_multiDataInfoRect, _backBrush);
	LONG graphWidth = GetRectWidth(_graphRect);
	LONG height = GetRectHeight(_multiDataInfoRect);
	LONG width = GetRectWidth(_multiDataInfoRect);

	HFONT oldFont = (HFONT)SelectObject(hdc, _graphUIFont);
	HPEN oldPen = (HPEN)SelectObject(hdc, _graphUIPen);
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, AddColor(_backColor, RGB(125, 125, 125)));
	
	MoveToEx(hdc, graphWidth, 30, NULL);
	LineTo(hdc, graphWidth, height + 30);

	RECT tempRect;
	for (int i = 0;i < _iDataColumnNum;i++) {
		tempRect.top = _multiDataInfoRect.top + (i * 20 + 5);
		tempRect.bottom = tempRect.top + 15;
		tempRect.left = _multiDataInfoRect.left + 5;
		tempRect.right = tempRect.left + 20;
		FillRect(hdc, &tempRect, _multiLineBrush[i]);

		TextOut(hdc, tempRect.right + 3, tempRect.top, _columnArray[i].szName, wcslen(_columnArray[i].szName));
	}

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldFont);
	SetBkMode(hdc, oldMode);
	SetTextColor(hdc, oldColor);
}

void CMonitorGraphUnit::DrawBarMultiGraph(HDC hdc)
{
	for (int i = 0;i < _iDataColumnNum;i++) {
		if (_columnArray[i]._DataArray == NULL) continue;
		this->DrawBarSingleGraph(hdc, i);
	}

	this->DrawMultiBarMemberList(hdc);
}

void CMonitorGraphUnit::DrawMultiBarMemberList(HDC hdc)
{
	FillRect(hdc, &_multiDataInfoRect, _backBrush);
	LONG graphHeight = GetRectHeight(_graphRect);
	LONG height = GetRectHeight(_multiDataInfoRect);
	LONG width = GetRectWidth(_multiDataInfoRect);

	HFONT oldFont = (HFONT)SelectObject(hdc, _graphUIFont);
	HPEN oldPen = (HPEN)SelectObject(hdc, _graphUIPen);
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, AddColor(_backColor, RGB(125, 125, 125)));
	UINT oldTextAlign = SetTextAlign(hdc, TA_LEFT | TA_CENTER);

	MoveToEx(hdc, 0, graphHeight + 30, NULL);
	LineTo(hdc, width, graphHeight + 30);

	for (int i = 0;i < _iDataColumnNum;i++) {
		TextOut(hdc, (width / _iDataColumnNum) * (i + 0.5), 30 + graphHeight + 2, _columnArray[i].szName, wcslen(_columnArray[i].szName));
	}

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldFont);
	SetBkMode(hdc, oldMode);
	SetTextColor(hdc, oldColor);
	SetTextAlign(hdc, oldTextAlign);
}

void CMonitorGraphUnit::DrawPieGraph(HDC hdc)
{
	LONG height = GetRectHeight(_graphRect);
	LONG width = GetRectWidth(_graphRect);
	LONG minGraph = min(height, width);

	int data = 50, totalData = 0;
	for (int i = 0;i < _iDataColumnNum;i++) {
		_columnArray[i]._DataArray->peek(_columnArray[i]._DataArray->getCount() - 1, &data);
		totalData += data;
	}
	int iDiameter = minGraph * 0.9;

	LONG graphCenterX = minGraph / 2;
	LONG graphCenterY = minGraph / 2 + 30;

	int iNX = (int)(cos(dfPI * 2) * 100);
	int iNY = (int)(sin(dfPI * 2) * 100);
	double dfAccuRadian = 0;
	for (int i = 0; i < _iDataColumnNum; i++) {
		_columnArray[i]._DataArray->peek(_columnArray[i]._DataArray->getCount() - 1, &data);
		if (data <= 0) continue;
		dfAccuRadian += dfPI * 2 * ((double)data / (double)totalData);

		int x = (int)(cos(dfAccuRadian) * 100);
		int y = (int)(sin(dfAccuRadian) * 100);

		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, _multiLineBrush[i]);
		Pie(hdc, graphCenterX - iDiameter / 2, graphCenterY - iDiameter / 2, 
			graphCenterX + iDiameter / 2, graphCenterY + iDiameter / 2,
			graphCenterX + x, graphCenterY + y,
			graphCenterX + iNX, graphCenterY + iNY
		);

		iNX = x; iNY = y;
		SelectObject(hdc, oldBrush);
	}
	dfAccuRadian = 0;
	HFONT oldFont = (HFONT)SelectObject(hdc, _barGraphDataFont);
	UINT oldTextAlign = SetTextAlign(hdc, TA_LEFT | TA_CENTER);
	int oldMode = SetBkMode(hdc, TRANSPARENT);
	for (int i = 0; i < _iDataColumnNum; i++) {
		_columnArray[i]._DataArray->peek(_columnArray[i]._DataArray->getCount() - 1, &data);
		dfAccuRadian += dfPI * 2 * ((double)data / (double)totalData);

		int x = (int)(cos(dfAccuRadian) * 100);
		int y = (int)(sin(dfAccuRadian) * 100);
		double tx = x, ty = y;

		AddVector(&tx, &ty, iNX, iNY);

		tx *= (iDiameter / 3);
		ty *= (iDiameter / 3);

		WCHAR buffer[256];
		_itow((double)data / totalData * 100, buffer, 10);
		COLORREF oldColor = SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, graphCenterX + tx, graphCenterY + ty + 1, buffer, wcslen(buffer));
		TextOut(hdc, graphCenterX + tx, graphCenterY + ty - 1, buffer, wcslen(buffer));
		TextOut(hdc, graphCenterX + tx + 1, graphCenterY + ty, buffer, wcslen(buffer));
		TextOut(hdc, graphCenterX + tx - 1, graphCenterY + ty, buffer, wcslen(buffer));
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, graphCenterX + tx, graphCenterY + ty, buffer, wcslen(buffer));
		SetTextColor(hdc, oldColor);

		iNX = x; iNY = y;
	}
	SelectObject(hdc, oldFont);
	SetBkMode(hdc, oldMode);
	SetTextAlign(hdc, oldTextAlign);
}

void CMonitorGraphUnit::ReleaseAllObject()
{
	DeleteObject(_titleFont);
	DeleteObject(_graphUIPen);
	DeleteObject(_backBrush);
	DeleteObject(_titleBrush);
	DeleteObject(_graphUIFont);
	DeleteObject(_barBrush);
	DeleteObject(_barGraphDataFont);

	for (int i = 0; i < _iDataColumnNum;i++) {
		if (_columnArray[i].szName == NULL) continue;
		delete _columnArray[i]._DataArray;
	}
	delete[] _columnArray;
	_columnArray = NULL;

	for (int i = 0;i < enCOLOR_NUM;i++) {
		DeleteObject(_multiLineBrush[i]);
		DeleteObject(_multiLinePen[i]);
	}
}

BOOL CMonitorGraphUnit::InsertData(int iData, ULONG64 u64ServerID, int iType)
{
	for (int i = 0;i < _iDataColumnNum;i++) {
		if (_columnArray[i].u64ServerID != u64ServerID) continue;
		if (_columnArray[i].iType != iType) continue;

		int temp;
		if (_columnArray[i]._DataArray->getCount() == dfMAXDATA - 1)
			_columnArray[i]._DataArray->dequeue(&temp);
		_columnArray[i]._DataArray->enqueue(iData);

		return TRUE;
	}
	return FALSE;
}

BOOL CMonitorGraphUnit::SetDataColumnInfo(ULONG64 u64ServerID, int iType, WCHAR * szName)
{
	for (int i = 0;i < _iDataColumnNum;i++) {
		if (_columnArray[i].u64ServerID != 0) continue;
		wcscpy(_columnArray[i].szName, szName);
		_columnArray[i].iType = iType;
		_columnArray[i].u64ServerID = u64ServerID;
		_columnArray[i]._DataArray = new my_queue<int>(dfMAXDATA);
		return TRUE;
	}
	return FALSE;
}

BOOL CMonitorGraphUnit::PutThis(void)
{
	for (int i = 0;i < dfMAXCHILD;i++) {
		if (_hWndToThis.pThis[i] != NULL) continue;
		_hWndToThis.hWnd[i] = _hWnd;
		_hWndToThis.pThis[i] = this;
		return TRUE;
	}
	return FALSE;
}

BOOL CMonitorGraphUnit::RemoveThis(HWND hWnd)
{
	for (int i = 0;i < dfMAXCHILD; i++) {
		if (_hWndToThis.hWnd[i] != hWnd) continue;
		_hWndToThis.hWnd[i] = NULL;
		if (_hWndToThis.pThis[i]) {
			delete _hWndToThis.pThis[i];
			_hWndToThis.pThis[i] = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

CMonitorGraphUnit * CMonitorGraphUnit::GetThis(HWND hWnd)
{
	for (int i = 0;i < dfMAXCHILD; i++) {
		if (_hWndToThis.hWnd[i] != hWnd) continue;
		return _hWndToThis.pThis[i];
	}
	return NULL;
}
