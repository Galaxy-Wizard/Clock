#include "pch.h"
#include "CStaticExtended.h"


void CStaticExtended::SetTextColor(COLORREF ParameterCurrentColor)
{
	CurrentColor = ParameterCurrentColor;
}

void CStaticExtended::OnPaint()
{
	//CStatic::OnPaint();

	CDC* pdc = GetDC();

	if (pdc != nullptr)
	{
		RECT Rectangle;
		GetClientRect(&Rectangle);
		pdc->FillRect(&Rectangle, nullptr);

		CString WindowText = TextString;

		pdc->SetTextColor(CurrentColor);

		CFont CurrentFont;

		BOOL Result_1 = FALSE;

		if (Size == Big)
		{
			Result_1 = CurrentFont.CreateFontW(24, 16, 16, 16, FW_NORMAL, TRUE, TRUE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, nullptr);
		}
		else
		{
			if (Size == Medium)
			{
				Result_1 = CurrentFont.CreateFontW(22, 12, 16, 16, FW_NORMAL, TRUE, TRUE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, nullptr);
			}
			else
			{

				if (Size == Little)
				{
					Result_1 = CurrentFont.CreateFontW(16, 12, 16, 16, FW_NORMAL, TRUE, TRUE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, nullptr);
				}
			}
		}

		if (Result_1 == TRUE)
		{
			CRect Rectangle;

			GetClientRect(Rectangle);

			HGDIOBJ LastFont = pdc->SelectObject(CurrentFont);

			BOOL Result_2 = pdc->TextOutW(22, Rectangle.Height() / 2, WindowText);

			pdc->SelectObject(LastFont);

			CurrentFont.DeleteObject();
		}


		ReleaseDC(pdc);
		pdc = nullptr;
	}
}

void CStaticExtended::SetWindowTextW(LPCTSTR lpszString, int size)
{
	TextString = lpszString;
	Size = size;

	//CStatic::SetWindowTextW(TextString);

	OnPaint();
}