
#include "stdafx.h"
#include "TraceMsgLog.h"

#include "atltime.h"


CTraceMsgLog::CTraceMsgLog(void)
{
}

CTraceMsgLog::~CTraceMsgLog(void)
{
}

void CTraceMsgLog::TraceMessage(CString str)
{
	CTime theTime;
	theTime = CTime::GetCurrentTime();

	CString strTime;
	strTime = theTime.Format(_T("%#H:%#M:%#S"));

	CString traceMsg;
	traceMsg.Format(_T("[IN3_TRACE][%s] %s\r\n"), strTime, str);
	TRACE(traceMsg);
}

void CTraceMsgLog::TraceMessage(int level, CString str)
{
	CString traceMsg(_T(""));
	switch(level)
	{
	case MSGINFO:
		traceMsg.Append(_T("[INFO] "));
		break;

	case MSGERROR:
		traceMsg.Append(_T("[ERR] "));
		break;

	case JAVASCRIPT_ALERT:
		traceMsg.Append(_T("[JAVASCRIPT ALERT] "));
		break;

	default:
		traceMsg.Append(_T("[DEBUG] "));
		break;
	}
	//
	traceMsg.Append(str);
	//
	TraceMessage(traceMsg);
}

void CTraceMsgLog::TraceWithMemory(CString str)
{
	CString traceMsg(L"[IN3_TRACE]");
	//
	// @ note
	// memory °ü·Ã string ¸¸µéÀð.
	//
	traceMsg.Append(str);
	traceMsg.Append(str);
	traceMsg.Append(L"\r\n");
	TRACE(traceMsg);
}