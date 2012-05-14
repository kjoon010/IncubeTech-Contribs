
#ifndef __IN3WebkitDefine_H__
#define __IN3WebkitDefine_H__


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



#endif /*__IN3WebkitDefine_H__*/