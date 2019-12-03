#pragma once

/*
 功能：剧情详情页面，当详情页面的剧情介绍多余三行文字时，显示【查看更多】按钮。点击【查看更多】弹出该窗口。
 对应xml：OLVideoPlotWnd.xml
 该窗口展现详情信息，不能完全显示时，展示滚动条。
*/

#define  TIMERID_RAND 1
#define  TIMER_INTERVAL 20

#define  TIMER_TEST 2
#define  TIMER_INTERVAL_TEST  1000
namespace DuiLib
{

	class CDUIPlotWnd: public CWindowWnd, public INotifyUI
	{
	public:
		CDUIPlotWnd(void);
		~CDUIPlotWnd(void);

		LPCTSTR GetWindowClassName() const ;
		void Notify(TNotifyUI &msg);
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		void SetPlotText(CString str); //设置详情信息

		LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKeyDowm(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		void Init();
		BOOL GetSysAppFilePath(HMODULE hModule, CString &strAppWorkRoot, CString &strAppFilePath, CString &strAppFileName, CString &strAppName, CString &strAppExt);
		void Start(int type=0);
		void Stop(int type=0);
		void ReLottery();
		int IsOK();
		int GetRandomNum();
		
		void CalcResult();
	private:
		CPaintManagerUI m_PaintMgr;
		vector<CString>     m_OrgV;
		vector<CString>     m_OrgF;
		vector<CString>     m_OrgB;
		map<CString,int>		m_Old;
		UINT            m_nTimerRand;

		BOOL           m_bStart;
		CString   m_orgFile;
	

		//int  m_calcResult[200];

	};

}
