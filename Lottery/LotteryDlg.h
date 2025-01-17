
// LotteryDlg.h : 头文件
//

#pragma once
#include "DUIPlotWnd.h"

// CLotteryDlg 对话框
class CLotteryDlg : public CDialog
{
// 构造
public:
	CLotteryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LOTTERY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	DuiLib::CDUIPlotWnd m_DUIManWnd;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
