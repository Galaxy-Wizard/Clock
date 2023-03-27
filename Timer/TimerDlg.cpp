// TimerDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Timer.h"
#include "TimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CTimerDlg

const UINT_PTR EventNumber = 2000;
const UINT EventTimeOneSecond = 1000;
const COLORREF RedColor = RGB(255, 0, 0);
const COLORREF GrayColor = RGB(127, 127, 127);

CTimerDlg::CTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMER_DIALOG, pParent), Stop(true), CurrentColor(GrayColor)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, CurrentTime);
}

BEGIN_MESSAGE_MAP(CTimerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTimerDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CTimerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений CTimerDlg

BOOL CTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	SetTimer(EventNumber, EventTimeOneSecond, nullptr);
	InitialEventTime = CTimeSpan();
	CurrentEventTime = InitialEventTime;

	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = true;

	SingleLock.Unlock();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTimerDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	int InitialTimeInSeconds = 0;

	InitialEventTime = CTimeSpan(InitialTimeInSeconds);

	CurrentEventTime = InitialEventTime;

	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = false;
	CurrentColor = GrayColor;

	SingleLock.Unlock();
}


void CTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (nIDEvent == EventNumber)
	{
		CSingleLock SingleLock(&StopCriticalSection);
		SingleLock.Lock();

		CTimeSpan Time_Difference = CurrentEventTime - InitialEventTime;

		CString CurrentTimeString;
		if (Stop)
		{
			CurrentTime.SetTextColor(CurrentColor);

			CurrentTimeString.Format(CString(L"Время остановлено. Дни: %d Часы: %d Минуты: %d Секунды: %d"), Time_Difference.GetDays(), Time_Difference.GetHours(), Time_Difference.GetMinutes(), Time_Difference.GetSeconds());
			CurrentTime.SetWindowTextW(CurrentTimeString, Little);
		}
		else
		{
			CurrentColor = GrayColor;

			CurrentTime.SetTextColor(CurrentColor);

			CurrentEventTime += CTimeSpan(EventTimeOneSecond / 1000);

			CString CurrentTimeString;

			CurrentTimeString.Format(CString(L"Дни: %d Часы: %d Минуты: %d Секунды: %d"), Time_Difference.GetDays(), Time_Difference.GetHours(), Time_Difference.GetMinutes(), Time_Difference.GetSeconds());

			CurrentTime.SetWindowTextW(CurrentTimeString, Medium);
		}

		SingleLock.Unlock();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTimerDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = true;
	CurrentColor = RedColor;

	SingleLock.Unlock();
}
