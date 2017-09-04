#include "Resize.h"

void Resize_Add(Resize_Table2 &d, HWND _hwnd, DWORD dlgItem, DWORD flags)
{
	d.a[d.count].hwnd = GetDlgItem(_hwnd, dlgItem);
	// get parent window size
	RECT rect;
	GetClientRect(_hwnd, &rect);
	d.a[d.count].xy[0].siz_parnt = rect.right;
	d.a[d.count].xy[1].siz_parnt = rect.bottom;
	// get child window size
	GetClientRect(d.a[d.count].hwnd, &rect);
	d.a[d.count].xy[0].siz_child = rect.right;
	d.a[d.count].xy[1].siz_child = rect.bottom;
	// get child window position
	MapWindowPoints(d.a[d.count].hwnd, _hwnd, (LPPOINT)&rect, 1);
	d.a[d.count].xy[0].pos_child = rect.left;
	d.a[d.count].xy[1].pos_child = rect.top;
	// setup scale data
	char *tmp = (char*)&flags;
	d.a[d.count].xy[0].siz_scale = tmp[0];
	d.a[d.count].xy[0].pos_scale = tmp[1];
	d.a[d.count].xy[1].siz_scale = tmp[2];
	d.a[d.count].xy[1].pos_scale = tmp[3];
	d.count++;
}

void Resize_WmSize(Resize_Table2 &d, HWND _hwnd)
{
	RECT parent;
	GetClientRect(_hwnd, &parent);
	for(int i = 0; i < d.count; i++){
		struct{
			short siz_child;
			short pos_child;
		}tempData[2];
		for(int j = 1; j > -1; j--){
			tempData[j].siz_child = d.a[i].xy[j].siz_child;
			tempData[j].pos_child = d.a[i].xy[j].pos_child;
			short diff = j ? parent.bottom : parent.right;
			diff -= d.a[i].xy[j].siz_parnt;
			if( d.a[i].xy[j].siz_scale & 0xf0){
				char siz_scale = d.a[i].xy[j].siz_scale;
				float ratio = (float)(siz_scale >> 4) / (float)(siz_scale & 15);
				tempData[j].siz_child += diff * ratio;
			}
			if( d.a[i].xy[j].pos_scale & 0xf0 ){
				char pos_scale = d.a[i].xy[j].pos_scale;
				float ratio = (float)(pos_scale >> 4) / (float)(pos_scale & 15);
				tempData[j].pos_child += diff * ratio;
			}
		}
		MoveWindow(d.a[i].hwnd, tempData[0].pos_child, tempData[1].pos_child, 
		tempData[0].siz_child, tempData[1].siz_child, false);
	}
	InvalidateRect(_hwnd, NULL, true);
}

void Resize_WmErase(Resize_Table2 &d, HWND _hwnd, HDC hDC)
{
	for(int i = 0; i < d.count; i++){
		RECT rect;
		GetClientRect(d.a[i].hwnd, &rect);
		MapWindowPoints(d.a[i].hwnd, _hwnd, (LPPOINT)&rect, 2);
		ExcludeClipRect( hDC, rect.left, rect.top, rect.right, rect.bottom );
	}
}
