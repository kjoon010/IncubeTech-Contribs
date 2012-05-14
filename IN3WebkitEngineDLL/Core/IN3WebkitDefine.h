
#ifndef __IN3WebkitDefine_H__
#define __IN3WebkitDefine_H__

// IN3WebkitEngine 내에서 사용되는 상수

#define EPUB_VER_2					2
#define EPUB_VER_3					3
#define SUPPORT_EPUB_VERSION		EPUB_VER_3				// 지원하는 epub version
#define SUPPORT_THUMBNAIL_PATH		0						// thumbnail 추출 경로를 엔진에서 지원하는가에 대한 설정
															// 0 일 경우, 뷰어에서 임의 경로를 설정하여 사용한다.
#define SUPPORT_VIEW_POOL_MANAGER	0
#define	SUPPORT_DELETE_UNZIPDIR		0

	
// HEADER
#define ENGINE_SOURCE_KEY			L"SOURCE"
#define ENGINE_SOURCE_VALUE			L"#SEND_ENGINE#"

// CODE TYPE
#define CMD_IN3_TYPE				L"CMD_TYPE"
#define	CMD_TYPE_VIEWER				L"CMD_TYPE_VIEWER"		// 뷰어에서 엔진으로 보낼때는 무조건 이 커멘드를 사용.
#define CMD_TYPE_ENGINE				L"CMD_TYPE_ENGINE"		// 엔진에서는 VIEWER, ENGINE 커멘드 모두 사용 가능하며 
															// 이벤트 처리 대상에 따라 구분하여 사용.(VIEWER/ENGINE)

// COMMEND
#define CMD_IN3_CODE						L"CMD_CODE"
#define CMD_IN3_PAGE_INIT					L"IN3_CMD_PAGE_INIT"
#define CMD_IN3_GET_PAGE					L"IN3_CMD_GET_COUNT_PAGE_TOTAL"
#define CMD_IN3_OPEN_POPUP_WEB				L"IN3_CMD_OPEN_POPUP_WEB"
#define CMD_IN3_PAGE_MOVE					L"IN3_CMD_ACTION_PAGE_MOVE"
#define CMD_IN3_MOVE_TO_OBJECT				L"IN3_CMD_ACTION_MOVE_TO_OBJECT_ID"
#define CMD_IN3_FONT_SIZE_CHANGE			L"IN3_CMD_ACTION_CHANGE_FONT_SIZE"
#define CMD_IN3_FONT_SIZE_CHANGE_AND_PAGING	L"IN3_CMD_ACTION_CHANGE_FONT_SIZE_AND_REPAGING"
#define CMD_IN3_FIND_TEXT					L"IN3_CMD_ACTION_FIND_TEXT"

#define CMD_IN3_INIT_ANNOTATION_LIST		L"IN3_CMD_ACTION_INIT_ANNOTATION_LIST"		// init annotaiton list
#define CMD_IN3_MARK_ANNOTATION				L"IN3_CMD_ACTION_MARK_ANNOTATION"			// add annotation
#define CMD_IN3_DELETE_ANNOTATION			L"IN3_CMD_ACTION_DELETE_ANNOTATION"			// delete annotation
#define CMD_IN3_MODIFY_ANNOTATION			L"IN3_CMD_ACTION_MODIFY_ANNOTATION"			// modify annotation
#define CMD_IN3_QUERY_ANNOTATION_POS		L"IN3_CMD_ACTION_QUERY_SELECTION_POSITION"	// query annotation's position
#define CMD_IN3_EVENT_ANNOTATION			L"IN3_CMD_EVENT_ON_SELECT_ANNOTATION"		// event when annotation selected
#define CMD_IN3_EVENT_MEMO_ICON				L"IN3_CMD_EVENT_ON_SELECT_MEMO_ICON"		// event when memoicon selected

#define CMD_IN3_INIT_BOOKMARK_LIST			L"IN3_CMD_ACTION_INIT_BOOKMARK_LIST"		// init bookmark list
#define CMD_IN3_SET_BOOKMARK				L"IN3_CMD_ACTION_SET_BOOKMARK"				// add bookmark
#define CMD_IN3_CLEAR_BOOKMARK				L"IN3_CMD_ACTION_CLEAR_BOOKMARK"			// clear bookmark on current page
#define CMD_IN3_DELETE_BOOKMARK				L"IN3_CMD_ACTION_DELETE_BOOKMARKS"			// delete bookmark by id
#define CMD_IN3_QUERY_BOOKMARK_STATUS		L"IN3_CMD_ACTION_QUERY_BOOKMARK_STATUS"
#define CMD_IN3_EVENT_BOOKMARK_STATUS		L"IN3_CMD_EVENT_BOOKMARK_STATUS"			// receive event when page moved

#define CMD_IN3_FIND_TEXT_ALL				L"IN3_CMD_ACTION_FIND_TEXT_ALL"				// text search all
#define CMD_IN3_FIND_TEXT					L"IN3_CMD_ACTION_FIND_TEXT"					// text search
#define CMD_IN3_MOVE_TO_FIND_RESULT			L"IN3_CMD_ACTION_MOVE_TO_SEARCH_RESULT"		// move to search result

#define CMD_IN3_PAGE_LOAD					L"IN3_PAGE_LOAD"							// page load
#define CMD_IN3_COMPLETE_FILE_INIT			L"IN3_CMD_COMPLETE_FILE_INIT"				// page load complate
#define CMD_IN3_CANCEL_SELECTION			L"IN3_CMD_ACTION_CANCEL_SELECTION"			// cancel selection
#define CMD_IN3_CHANGE_COLORS				L"IN3_CMD_ACTION_CHANGE_COLORS"				// change colors


//
#define	MAX_EPUB				10

// view Rect
#define RECT_CHAPTER_HEGIHT		52		// 챕터 표시 영역
#define RECT_PAGEINFO_HEIGHT	58		// 페이지 번호 표시 영역
#define RECT_REFLOW_SIDE_GAP	40		// 컬럽 갭


// keris 
#define DEFAULT_CONTENT_HEIGHT	1024		//864
#define DEFAULT_CONTENT_WIDTH	768			//628


// User Message
#define WM_CLOSE_EPUB				(WM_USER + 1)		// epub close event
#define WM_LODEAD_EPUB				(WM_USER + 2)		// when epub file load complate ( when total page calculated )
#define WM_CONTEXTMENU_EPUB			(WM_USER + 3)		// show context menu
#define WM_ANNOTATIONMENU_EPUB		(WM_USER + 4)		// show annotation menu
#define WM_OPEN_WEB_POPUP			(WM_USER + 5)		// open web popup

#define WM_ANNOTATION_ADDED			(WM_USER + 6)		// add annotation callback event
#define WM_ANNOTATION_SELECTED		(WM_USER + 7)		// annotation selected event
#define WM_MEMO_ICON_SELECTED		(WM_USER + 8)		// memo icon selected callback event
#define WM_ANNOTATION_DELETED		(WM_USER + 9)		// deleted annotation event
#define WM_ANNOTATION_MODIFIED		(WM_USER + 10)		// modified anotation event
#define WM_ANNOTATION_POSITION		(WM_USER + 11)		//

#define WM_BOOKMARK_ADDED			(WM_USER + 12)		// bookmark added event
#define WM_BOOKMARK_DELETED			(WM_USER + 13)		// bookmark deleted event
#define WM_BOOKMARK_CLEARED			(WM_USER + 14)		// bookmark cleared event
#define WM_BOOKMARK_STATUS			(WM_USER + 15)		// bookmark status event

#define WM_FIND_TEXT_ALL			(WM_USER + 16)		// find all text event
#define WM_FIND_TEXT				(WM_USER + 17)		// find text (prev/next)
#define WM_FIXED_PAGE_SIZE			(WM_USER + 18)

#define WM_PAGE_MOVED				(WM_USER + 19)		// page moved event

//
// trace message for debug
#if _DEBUG
#include "TraceMsgLog.h"

#define TRACEMSG(a)			CTraceMsgLog::TraceMessage(a)
#define TRACEMSG_ERR(a)		CTraceMsgLog::TraceMessage(0, a)
#define TRACEMSG_INFO(a)	CTraceMsgLog::TraceMessage(1, a)
#define TRACEMSG_JAVA(a)	CTraceMsgLog::TraceMessage(3, a)

#else

#define TRACEMSG(a)
#define TRACEMSG_ERR(a)
#define TRACEMSG_INFO(a)
#define TRACEMSG_JAVA(a)

#endif /*!_DEBUG*/

#ifdef USE_WEBKIT_PRINT
#undef  USE_WEBKIT_PRINT
#endif


#endif /*__IN3WebkitDefine_H__*/