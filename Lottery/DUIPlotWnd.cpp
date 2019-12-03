#include "StdAfx.h"
#include "DUIPlotWnd.h"
#include <locale.h>

namespace DuiLib
{
	CDUIPlotWnd::CDUIPlotWnd(void)
	{
		m_nTimerRand = 0;
		m_bStart = FALSE;

		
	}

	CDUIPlotWnd::~CDUIPlotWnd(void)
	{
	}

	LPCTSTR CDUIPlotWnd::GetWindowClassName() const 
	{ 
		return _T("CDUIPlotWnd"); 
	}

	LRESULT CDUIPlotWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintMgr.Init(m_hWnd);


		CDialogBuilder builder;
	
		CControlUI* pRoot = NULL;
		pRoot = builder.Create(_T("OLVideoPlotWnd.xml"), (UINT)0, 0, &m_PaintMgr);

		ASSERT(pRoot && "Failed to parse XML");
		m_PaintMgr.AttachDialog(pRoot);
		m_PaintMgr.AddNotifier(this);

		Init();
	
		return 0;
	}

	void CDUIPlotWnd::Init()
	{
		BOOL BRet = FALSE;
		m_OrgV.clear();
		m_OrgF.clear();
		m_OrgB.clear();
		
		//获取工作目录
		CString strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt;
		BRet = GetSysAppFilePath(NULL, strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt);
		if(FALSE == BRet)
		{
			return;
		}
		
		m_orgFile = strAppWorkPath + _T("LotteryOrg.txt");
		CString resultFile = strAppWorkPath + _T("result.txt");
		
		CString firstFile = strAppWorkPath + _T("first.txt");
		CString black = strAppWorkPath + _T("b.txt");
		CString strText = _T("");
		CString szLine = _T("");
		//打开文件
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
		setlocale( LC_CTYPE, "chs" );
		

		vector<CString>     alreadyVec;
		/*
		CStdioFile file_already;
		file_already.Open(resultFile,CFile::modeRead);
		
		
		DWORD dwAttr = ::GetFileAttributes(resultFile); 
		if (dwAttr != 0xffffffff)  
		{ 
			while( file_already.ReadString( szLine ) )
			{
				strText += szLine;
				//int num = _wtoi(szLine);
				szLine.TrimLeft();
				szLine.TrimRight();
				//去掉时间部分，只留姓名
				int nPos = szLine.Find(_T("\t"));
				CString strName = szLine.Left(nPos);
				alreadyVec.push_back(strName);
			}
		}  
		

		file_already.Close();
*/
		//
		CStdioFile file;
		file.Open(m_orgFile,CFile::modeRead);
		if(file == NULL)
		{
			AfxMessageBox(_T("找不到LotteryOrg.txt文件！"));
			return;
		}
		//逐行读取字符串
		while( file.ReadString( szLine ) )
		{
			strText += szLine;
			//int num = _wtoi(szLine);
			szLine.TrimLeft();
			szLine.TrimRight();

			//判断是否已存在于result.txt

			vector<CString>::iterator it;
			it = find(alreadyVec.begin(),alreadyVec.end(),szLine);
			if(it != alreadyVec.end()){
				//存在该值
				
			}
			else{
				m_OrgV.push_back(szLine);
			}
			
		}

		//关闭文件
		file.Close();
		setlocale( LC_CTYPE, old_locale ); 
		free( old_locale ); 
		
		
		//first
		
		DWORD dwAttr1 = ::GetFileAttributes(firstFile); 
		if (dwAttr1 != 0xffffffff)  
		{ 
			CStdioFile fileFist;
			fileFist.Open(firstFile,CFile::modeRead);
			old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE, "chs" );
			//逐行读取字符串
			while( fileFist.ReadString( szLine ) )
			{
				strText += szLine;
				//int num = _wtoi(szLine);
				szLine.TrimLeft();
				szLine.TrimRight();

				//判断是否已存在于result.txt

				vector<CString>::iterator it;
				it = find(alreadyVec.begin(),alreadyVec.end(),szLine);
				if(it != alreadyVec.end()){
					//存在该值

				}
				else{
					m_OrgF.push_back(szLine);
				}

			}

			//关闭文件
			fileFist.Close();
			setlocale( LC_CTYPE, old_locale ); 
			free( old_locale ); 
		}
	
	

		//first end

		//black
		
		DWORD dwAttr2 = ::GetFileAttributes(black); 
		if (dwAttr2 != 0xffffffff)  
		{ 
			CStdioFile blackFile;
			blackFile.Open(black,CFile::modeRead);
			old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE, "chs" );
			//逐行读取字符串
			while( blackFile.ReadString( szLine ) )
			{
				strText += szLine;
				//int num = _wtoi(szLine);
				szLine.TrimLeft();
				szLine.TrimRight();

				m_OrgB.push_back(szLine);
				

			}

			//关闭文件
			blackFile.Close();
			setlocale( LC_CTYPE, old_locale ); 
			free( old_locale ); 
		}

		for(vector<CString>::iterator it = m_OrgB.begin();it != m_OrgB.end();it++)
		{
			CString strValue = *it;
			for(vector<CString>::iterator it_a = m_OrgV.begin();it_a != m_OrgV.end();it_a++){
				CString strCurr = *it_a;
				if(strCurr == strValue){
					m_OrgV.erase(it_a);
					break;
				}
			}
			
		}
	
		//black end

		
		for(int i=0;i<(int)m_OrgV.size();i++){
			m_Old.insert(pair<CString,int>(m_OrgV[i],0));

			CString str = m_OrgV[i];
			str +=_T(".jpg");
			
		/*	CString strPath = strAppWorkPath + _T("photo\\");
			strPath += str;
			const DuiLib::TImageInfo *p = m_PaintMgr.AddImage(str,strPath,true);*/

			const DuiLib::TImageInfo *p = m_PaintMgr.AddImage(str);

			if(p == NULL){
				AfxMessageBox(str + _T("图片不存在"));
			}
		}
		
		


		
	}

	void CDUIPlotWnd::Notify(TNotifyUI &msg)
	{
		if( msg.sType == _T("click") ) //鼠标点击事件
		{
			if( _tcsicmp(msg.pSender->GetName(), _T("btnPlotClose")) == 0) 
			{	
				Close();
				return; 
			}
			else if(_tcsicmp(msg.pSender->GetName(), _T("btnPlotMin")) == 0)
			{
				::PostMessage(m_hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
				return; 
			}
			else if(_tcsicmp(msg.pSender->GetName(), _T("btnStart")) == 0)
			{
				Start();
			}
			else if(_tcsicmp(msg.pSender->GetName(), _T("btnStop")) == 0)
			{
				Stop();
			}
			
			else if(_tcsicmp(msg.pSender->GetName(), _T("btnCalc")) == 0)
			{
				CalcResult();
			}
		

		}
	}

	

	void CDUIPlotWnd::Start(int type)
	{
		m_bStart = TRUE;
		
		srand( (unsigned int)time(0) );
		m_nTimerRand = ::SetTimer(this->GetHWND(),TIMERID_RAND,TIMER_INTERVAL,NULL);

		CControlUI* pStart = m_PaintMgr.FindControl(_T("btnStart"));
		if(pStart != NULL)
		{
			pStart->SetVisible(false);
		}

		CControlUI* pStop = m_PaintMgr.FindControl(_T("btnStop"));
		if(pStop != NULL)
		{
			pStop->SetVisible(true);
		}
	}

	void CDUIPlotWnd::Stop(int type)
	{
		m_bStart = FALSE;

		CControlUI* pStop = m_PaintMgr.FindControl(_T("btnStop"));
		if(pStop != NULL)
		{
			pStop->SetVisible(false);
		}

		CControlUI* pStart = m_PaintMgr.FindControl(_T("btnStart"));
		if(pStart != NULL)
		{
			pStart->SetVisible(true);
		}

		//如果有优先队列
		if(m_OrgF.size() > 0 && type == 1){
			vector<CString>::iterator it_f = m_OrgF.begin();
			CString str_f = *it_f;
			
			CControlUI* pResult = m_PaintMgr.FindControl(_T("lblResult"));
			if(pResult != NULL)
			{
				CString strImg = str_f+_T(".jpg");
				pResult->SetBkImage(strImg);


				pResult->NeedUpdate();

				
				TRACE(str_f);
				TRACE(_T("\n"));
			}


			CControlUI* pBack = m_PaintMgr.FindControl(_T("lblText"));
			if(pBack != NULL)
			{
				pBack->SetText(str_f);
			}

			m_OrgF.erase(it_f);

		}

		
		//结束取优先队列
		//ReLottery();
		CString strDelete;
		CControlUI* pResult = m_PaintMgr.FindControl(_T("lblText"));
		if(pResult != NULL)
		{
			CString strResult = pResult->GetText();
			strDelete = strResult;
			TRACE(_T("******Result"));
			TRACE(strResult);
			TRACE(_T("\n"));
			
			CString strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt;
			BOOL BRet = GetSysAppFilePath(NULL, strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt);
			if(FALSE == BRet)
			{
				return;
			}


			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE, "chs" );

			CStdioFile  mFile;                             //定义写文件CStdio 的对象。

			CFileException    m_Except;
			mFile.Open(strAppWorkPath + _T("result.txt"),CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate,&m_Except);
			if(mFile != NULL)
			{
				mFile.SeekToEnd();

				CString strTime;
				CTime tm;
				tm = CTime::GetCurrentTime();
				strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S "));
				
				strResult += _T("\t");
				strResult += strTime;
				strResult += _T("\r\n");
				mFile.WriteString(strResult); 
				mFile.Close();
			}

			setlocale( LC_CTYPE, old_locale ); 
			free( old_locale ); 
			//删掉vector中选中数据
			//int nResult = _wtoi(strResult);
			/*
			for(vector<CString>::iterator it = m_OrgV.begin();it != m_OrgV.end();it++)
			{
				CString strValue = *it;
				//int nValue = _wtoi(strValue);
				if(strDelete == strValue)
				{
					m_OrgV.erase(it);
					break;
				}
			}
			*/
		}

		::KillTimer(this->GetHWND(),TIMERID_RAND);
	}

	void CDUIPlotWnd::ReLottery(){

		CControlUI* pResult = m_PaintMgr.FindControl(_T("lblText"));
		if(pResult != NULL)
		{
			CString strResult = pResult->GetText();
			BOOL bBlack = FALSE;
			for(vector<CString>::iterator it = m_OrgB.begin();it != m_OrgB.end();it++)
			{
				CString strValue = *it;
				//int nValue = _wtoi(strValue);
				if(strResult == strValue)
				{
					//在黑名单中，需要重新抽
					bBlack = TRUE;
					break;
				}
			}

			if(bBlack){
				int res = IsOK();
				while(res == -2){
					res = IsOK();
				}

				if(res == -1){
					AfxMessageBox(_T("请检查配置，没有可以选中的人员"));
				}else{
					CString nText = m_OrgV[res];
					CControlUI* pResult = m_PaintMgr.FindControl(_T("lblResult"));
					if(pResult != NULL)
					{
						CString strImg = nText+_T(".jpg");
						pResult->SetBkImage(strImg);


						pResult->NeedUpdate();

						
						TRACE(nText);
						TRACE(_T("\n"));
					}


					CControlUI* pBack = m_PaintMgr.FindControl(_T("lblText"));
					if(pBack != NULL)
					{
						pBack->SetText(nText);
					}

					map<CString ,int >::iterator it;
					it=m_Old.find(nText);
					it->second++;
				}
			}
		}
	}

	int CDUIPlotWnd::IsOK(){
		int nNum = GetRandomNum();
	
		CString str = m_OrgV[nNum];

		for(vector<CString>::iterator it = m_OrgB.begin();it != m_OrgB.end();it++)
		{
			CString strValue = *it;
			//int nValue = _wtoi(strValue);
			if(str == strValue)
			{
				//在黑名单中，需要重新抽
				return -2;
			}
		}

		return nNum;
	}

	int CDUIPlotWnd::GetRandomNum(){
		int nTotal = m_OrgV.size();
		if(nTotal == 0)
		{
			return -1;
		}

		int nRand = rand();
		int nNum = nRand%nTotal;

		CString str;
		str.Format(_T("%d,%d,%d,"),nNum,nRand,nTotal);
		TRACE(str + m_OrgV[nNum]);
		TRACE(_T("\n"));
		return nNum;
	}

	LRESULT CDUIPlotWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;

		switch( uMsg ) 
		{
		case WM_CREATE:        
			lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_NCHITTEST:     
			lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_CLOSE:
			lRes = OnClose(uMsg, wParam, lParam, bHandled);
			break;
		case WM_DESTROY:
			lRes = OnDestory(uMsg, wParam, lParam, bHandled);
			break;
		case WM_TIMER:
			lRes = OnTimer(uMsg,wParam,lParam,bHandled);
			break;
		case WM_KEYDOWN:
			lRes = OnKeyDowm(uMsg,wParam,lParam,bHandled);
			break;
		
		default:
			bHandled = FALSE;
		}

		if( bHandled ) 
		{
			return lRes;
		}

		if( m_PaintMgr.MessageHandler(uMsg, wParam, lParam, lRes) ) 
		{
			return lRes;
		}
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	LRESULT CDUIPlotWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CDUIPlotWnd::OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		PostQuitMessage(0L);
		return 0;
	}

	

	LRESULT CDUIPlotWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(wParam == m_nTimerRand)
		{
			int nNum = GetRandomNum();
			
			if(nNum == -1){
				bHandled = FALSE;
				return 0;
			}
			
			
			::KillTimer(this->GetHWND(),m_nTimerRand);
			m_nTimerRand = 0;

			CString nText = m_OrgV[nNum];
			CControlUI* pResult = m_PaintMgr.FindControl(_T("lblResult"));
			if(pResult != NULL)
			{
				CString strImg = nText+_T(".jpg");
				pResult->SetBkImage(strImg);

				
				pResult->NeedUpdate();
			}


			CControlUI* pBack = m_PaintMgr.FindControl(_T("lblText"));
			if(pBack != NULL)
			{
				pBack->SetText(nText);
			}

			 map<CString ,int >::iterator it;
			 it=m_Old.find(nText);
			 it->second++;
			
			m_nTimerRand = ::SetTimer(this->GetHWND(),TIMERID_RAND,TIMER_INTERVAL,NULL);
		}
		
		bHandled = FALSE;
		return 0;
	}

	LRESULT CDUIPlotWnd::OnKeyDowm(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(wParam == VK_SPACE)
		{
			if(m_bStart)
			{
				Stop(1);
			}
			else 
			{
				Start(1);
			}

			bHandled = TRUE;
		}else if(wParam == VK_RETURN){
			if(m_bStart)
			{
				Stop(0);
			}
			else 
			{
				Start(0);
			}

			bHandled = TRUE;
		}
		else if(wParam == VK_ESCAPE)
		{
			Close();
			return 0;
		}
		else 
		{
			bHandled = FALSE;
		}
		
		return 0;
	}

	LRESULT CDUIPlotWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		if( !::IsZoomed(*this) ) {
			RECT rcSizeBox = m_PaintMgr.GetSizeBox();
			if( pt.y < rcClient.top + rcSizeBox.top ) 
			{
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
				return HTTOP;
			}
			else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
			{
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if( pt.x < rcClient.left + rcSizeBox.left )
			{
				return HTLEFT;
			}
			if( pt.x > rcClient.right - rcSizeBox.right )
			{
				return HTRIGHT;
			}
		}

		RECT rcCaption = m_PaintMgr.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right 
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintMgr.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("EditUI")) != 0)
					return HTCAPTION;
		}

		return HTCLIENT;
	}

	void CDUIPlotWnd::SetPlotText(CString str)
	{
		CControlUI* pPlot = m_PaintMgr.FindControl(_T("txtInfo"));
		if(pPlot != NULL)
		{
			pPlot->SetText(str/*.GetBuffer()*/);
			//str.ReleaseBuffer();
		}
	}

	BOOL CDUIPlotWnd::GetSysAppFilePath(HMODULE hModule, CString &strAppWorkRoot, CString &strAppFilePath, CString &strAppFileName, CString &strAppName, CString &strAppExt)
	{
		TCHAR szModuleFileName[MAX_PATH]; // 全路径名
		TCHAR szDrive[_MAX_DRIVE]; // 盘符名称，比如说C盘啊，D盘啊
		TCHAR szDirect[_MAX_DIR]; // 目录
		TCHAR szAppName[_MAX_FNAME]; // 进程名字
		TCHAR szAppExt[_MAX_EXT]; //后缀，一般为exe或者是dll

		if (NULL == ::GetModuleFileName(hModule, szModuleFileName, MAX_PATH)) //获得当前进程的文件路径
		{
			return FALSE;
		}
		_tsplitpath_s(szModuleFileName, szDrive, szDirect, szAppName, szAppExt); //分割该路径，得到盘符，目录，文件名，后缀名
		strAppExt = szAppExt;
		strAppName = szAppName;
		strAppFileName = szModuleFileName;
		strAppFilePath = szDrive;
		strAppFilePath += szDirect;
		strAppWorkRoot = szDrive;

		strAppFilePath.Trim(_T("\t \\\r\n"));
		strAppFilePath += _T("\\");

		return TRUE;
	}

	

	void CDUIPlotWnd::CalcResult(){
		
		//获取工作目录
		CString strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt;
		BOOLEAN BRet = GetSysAppFilePath(NULL, strAppWorkRoot, strAppWorkPath, strAppFileName, strAppName, strAppExt);
		if(FALSE == BRet)
		{
			return;
		}

		CString calcFile = strAppWorkPath + _T("calc.csv");//获取工作目录
		
	
		CString strMsg = _T("");
		map<CString,int>::iterator it;
		for(it=m_Old.begin();it != m_Old.end();it++)
		{
			strMsg += it->first;
			
			
			CString str;
			str.Format(_T(",%d"),it->second);
			str += _T("\r");
			
			strMsg+=str;
		}


		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
		setlocale( LC_CTYPE, "chs" );

		CStdioFile file;
		file.Open(calcFile,CFile::modeCreate|CFile::modeWrite);
		if(file == NULL)
		{
			AfxMessageBox(_T("找不到calc.txt文件！"));
			return;
		}


		file.Seek(0,CFile::end);
		
		file.WriteString(strMsg); 
		

		//关闭文件
		file.Close();

		setlocale( LC_CTYPE, old_locale ); 
		free( old_locale ); 
	}
}
