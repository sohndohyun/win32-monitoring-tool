#ifndef _c_monitor_graph_unit_h__
#define _c_monitor_graph_unit_h__

#include "my_queue.h"

#define dfMAXCHILD 100
#define dfMAXDATA 20
#define dfPI 3.1416

class CMonitorGraphUnit
{
public:

	enum TYPE
	{
		BAR_SINGLE_HORZ,
		BAR_COLUMN_HORZ,
		LINE_SINGLE,
		LINE_MULTI,
		PIE
	};

	enum COLOR {
		enWHITE,
		enRED,
		enBLUE,
		enGREEN,
		enMAGENTA,
		enCYAN,
		enYELLOW,
		enBROWN,
		enGRAY,
		enORANGE,
		enCOLOR_NUM
	};

	typedef struct ST_HWNDtoTHIS {
		HWND hWnd[dfMAXDATA];
		CMonitorGraphUnit* pThis[dfMAXDATA];
	}stHWNDtoTHIS;

	typedef struct ST_COLUMN_INFO
	{
		ULONG64 u64ServerID;
		int iType;
		WCHAR szName[256];

		my_queue<int> *_DataArray;

	} stColumnInfo;
private:
	int _iDataColumnNum;
	stColumnInfo *_columnArray;

	COLORREF _multiLineGraphColor[enCOLOR_NUM];
public:

	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, WCHAR* title, COLORREF backColor, TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight, int iColumnNum = 1);
	~CMonitorGraphUnit();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void SetMultiLineColor();
	// PAINT:
	void DrawGraphTitle(HDC hdc);
	void DrawGraphInterface(HDC hdc);
	void DrawGraphBackground(HDC hdc);

	void DrawLineSingleGraph(HDC hdc, int arrayIndex = 0);
	void DrawBarSingleGraph(HDC hdc, int arrayIndex = 0);
	void DrawLineMultiGraph(HDC hdc);
	void DrawMultiLineMemberList(HDC hdc);
	void DrawBarMultiGraph(HDC hdc);
	void DrawMultiBarMemberList(HDC hdc);
	void DrawPieGraph(HDC hdc);

	void ReleaseAllObject();

	BOOL InsertData(int iData, ULONG64 u64ServerID = 1, int iType = 1);
	BOOL SetDataColumnInfo(ULONG64 u64ServerID, int iType, WCHAR *szName = L"DEFAULT");

	HWND GetHWND() { return _hWnd; }
protected:
	BOOL PutThis(void);
	static BOOL RemoveThis(HWND hWnd);
	static CMonitorGraphUnit *GetThis(HWND hWnd);
	
private:

	//------------------------------------------------------
	HWND _hWndParent;
	HWND _hWnd;
	HINSTANCE _hInstance;
	//------------------------------------------------------
	POINT _pos;
	SIZE _size;
	TYPE _enType;
	COLORREF _backColor;

	HPEN _graphUIPen;

	HBRUSH _backBrush;
	HBRUSH _titleBrush;
	HBRUSH _barBrush;

	HBRUSH _multiLineBrush[enCOLOR_NUM];
	HPEN _multiLinePen[enCOLOR_NUM];

	HFONT _titleFont;
	HFONT _graphUIFont;
	HFONT _barGraphDataFont;
	
	RECT _graphRect;
	RECT _multiDataInfoRect;
	RECT _titleRect;

	WCHAR _srzTitle[256];

	// static 맴버 함수의 프로시저에서 This 포인터를 찾기 위한
	// HWND + Class Ptr 의 테이블
	static stHWNDtoTHIS _hWndToThis;
	static int _childCnt;
};

#endif