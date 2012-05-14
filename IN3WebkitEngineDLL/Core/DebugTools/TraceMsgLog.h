#pragma once

#include <atlstr.h>

/*
 * CTraceMsgLog
 * 출력창에 디버그 메시지를 뿌려주는 클래스
 */
class CTraceMsgLog
{
public:
	CTraceMsgLog(void);
	~CTraceMsgLog(void);

	// 출력 메시지 타입 정의
	enum MessageLebel { MSGERROR = 0, MSGINFO, MSGDEBUG, JAVASCRIPT_ALERT };

	// 디버그 메시지 출력 함수
	static void TraceMessage(CString str);
	static void TraceMessage(int level, CString str);

	static void TraceWithMemory(CString str); // 구현 예정 현재 미사용
};
