#pragma once

#include <atlstr.h>

/*
 * CTraceMsgLog
 * ���â�� ����� �޽����� �ѷ��ִ� Ŭ����
 */
class CTraceMsgLog
{
public:
	CTraceMsgLog(void);
	~CTraceMsgLog(void);

	// ��� �޽��� Ÿ�� ����
	enum MessageLebel { MSGERROR = 0, MSGINFO, MSGDEBUG, JAVASCRIPT_ALERT };

	// ����� �޽��� ��� �Լ�
	static void TraceMessage(CString str);
	static void TraceMessage(int level, CString str);

	static void TraceWithMemory(CString str); // ���� ���� ���� �̻��
};
