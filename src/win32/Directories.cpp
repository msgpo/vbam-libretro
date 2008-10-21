// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team
// Copyright (C) 2008 VBA-M development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "stdafx.h"
#include "vba.h"
#include "Directories.h"
#include "Reg.h"
#include "WinResUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Directories dialog

static int CALLBACK browseCallbackProc(HWND hWnd, UINT msg,
                                       LPARAM l, LPARAM data)
{
  char *buffer = (char *)data;
  switch(msg) {
  case BFFM_INITIALIZED:
    if(buffer[0])
      SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)buffer);
    break;
  default:
    break;
  }
  return 0;
}

Directories::Directories(CWnd* pParent /*=NULL*/)
  : CDialog(Directories::IDD, pParent)
{
}


void Directories::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAVE_PATH, m_savePath);
	DDX_Control(pDX, IDC_ROM_PATH, m_romPath);
	DDX_Control(pDX, IDC_GBCROM_PATH, m_gbcromPath);
	DDX_Control(pDX, IDC_GBROM_PATH, m_gbromPath);
	DDX_Control(pDX, IDC_CAPTURE_PATH, m_capturePath);
	DDX_Control(pDX, IDC_BATTERY_PATH, m_batteryPath);
}


BEGIN_MESSAGE_MAP(Directories, CDialog)
  ON_BN_CLICKED(IDC_BATTERY_DIR, OnBatteryDir)
  ON_BN_CLICKED(IDC_CAPTURE_DIR, OnCaptureDir)
  ON_BN_CLICKED(IDC_GBROM_DIR, OnGbromDir)
  ON_BN_CLICKED(IDC_ROM_DIR, OnRomDir)
  ON_BN_CLICKED(IDC_SAVE_DIR, OnSaveDir)
  ON_BN_CLICKED(IDC_GBCROM_DIR, OnGbcromDir)
END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
// Directories message handlers

BOOL Directories::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString p;

	p = regQueryStringValue("romdir", NULL);
	if(!p.IsEmpty())
		GetDlgItem(IDC_ROM_PATH)->SetWindowText(p);

	p = regQueryStringValue("gbcromdir", NULL);
	if(!p.IsEmpty())
		GetDlgItem(IDC_GBCROM_PATH)->SetWindowText(p);

	p = regQueryStringValue("gbromdir", NULL);
	if(!p.IsEmpty())
		GetDlgItem(IDC_GBROM_PATH)->SetWindowText(p);

	p = regQueryStringValue("batteryDir", DEFAULT_BATTERY_DIR);
	if(!p.IsEmpty())
		GetDlgItem(IDC_BATTERY_PATH)->SetWindowText( p);

	p = regQueryStringValue("saveDir", DEFAULT_SAVESTATES_DIR);
	if(!p.IsEmpty())
		GetDlgItem(IDC_SAVE_PATH)->SetWindowText(p);

	p = regQueryStringValue("captureDir", NULL);
	if(!p.IsEmpty())
		GetDlgItem(IDC_CAPTURE_PATH)->SetWindowText(p);

	return TRUE;
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Directories::OnBatteryDir()
{
  m_batteryPath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_BATTERY_DIR));
  if(!p.IsEmpty())
    m_batteryPath.SetWindowText(p);
}

void Directories::OnCaptureDir()
{
  m_capturePath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_CAPTURE_DIR));
  if(!p.IsEmpty())
    m_capturePath.SetWindowText(p);
}

void Directories::OnGbromDir()
{
  m_gbromPath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_ROM_DIR));
  if(!p.IsEmpty())
    m_gbromPath.SetWindowText(p);
}

void Directories::OnGbcromDir()
{
  m_gbcromPath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_ROM_DIR));
  if(!p.IsEmpty())
    m_gbcromPath.SetWindowText(p);
}

void Directories::OnRomDir()
{
  m_romPath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_ROM_DIR));
  if(!p.IsEmpty())
    m_romPath.SetWindowText(p);
}

void Directories::OnSaveDir()
{
  m_savePath.GetWindowText(initialFolderDir);
  CString p = browseForDir(winResLoadString(IDS_SELECT_SAVE_DIR));
  if(!p.IsEmpty())
    m_savePath.SetWindowText(p);
}

void Directories::OnCancel()
{
  EndDialog(FALSE);
}

void Directories::OnOK()
{
	CDialog::OnOK();

	CString buffer;

	m_romPath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "romdir", buffer );
	treatRelativePath( buffer );

	m_gbcromPath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "gbcromdir", buffer );
	treatRelativePath( buffer );

	m_gbromPath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "gbromdir", buffer );
	treatRelativePath( buffer );

	m_batteryPath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "batteryDir", buffer );
	treatRelativePath( buffer );

	m_savePath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "saveDir", buffer );
	treatRelativePath( buffer );

	m_capturePath.GetWindowText(buffer);
	if( !buffer.IsEmpty() )
		regSetStringValue( "captureDir", buffer );
	treatRelativePath( buffer );

	EndDialog(TRUE);
}

CString Directories::browseForDir(CString title)
{
  static char buffer[1024];
  LPMALLOC pMalloc;
  LPITEMIDLIST pidl;

  CString res;

  if(SUCCEEDED(SHGetMalloc(&pMalloc))) {
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = m_hWnd;
    bi.lpszTitle = title;
    bi.pidlRoot = 0;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    bi.lpfn = browseCallbackProc;
    bi.lParam = (LPARAM)(LPCTSTR)initialFolderDir;

    pidl = SHBrowseForFolder(&bi);

    if(pidl) {
      if(SHGetPathFromIDList(pidl, buffer)) {
        res = buffer;
      }
      pMalloc->Free(pidl);
      pMalloc->Release();
    }
  }
  return res;
}
