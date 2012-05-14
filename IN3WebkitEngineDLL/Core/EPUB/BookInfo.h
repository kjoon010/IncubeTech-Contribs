#pragma once

#include "IN3WebkitDefine.h"
#include "IN3BookInfo.h"

/**
  @class	CBookInfo
  @remarks	IN3BookInfoEx 를 상속받은 클래스이다. 엔진에서만 사용되는 정보들이 포함되어 있다.
  @brief	책정보
 */
class AFX_EXT_CLASS CBookInfo : public IN3BookInfoEx
{
public:
	// 생성자
	CBookInfo(void);

	// 소멸자
	~CBookInfo(void);

	// Setter
	void SetNavType(long type);
	void SetFixedOption(long opt);

	void SetPakcageName(CString packageName);
	void SetOPSFilePath(CString href);
	void SetTocFilePath(CString href);
	void SetUnzipRootPath(CString root);

	void SetFixedWidth(int width);
	void SetFixedHeight(int height);

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
	void SetThumbnailPath(CString path);
#endif /*SUPPROT_THUMBNAIL_PATH*/


	// getter
	const long	GetNavType() const;
	const int GetFixedOption() const;

	const CString GetPackageName() const;
	const CString GetOPSFilePath() const;
	const CString GetTocFilePath() const;
	const CString GetUnzipRootPath() const;

	const int GetFixedWidth() const;
	const int GetFixedHeight() const;

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
	const CString GetThumbnailPath() const;
#endif /*SUPPROT_THUMBNAIL_PATH*/


	// overloading
	CBookInfo& operator=(const IN3BookInfo &other); ///< assignment operator
	CBookInfo& operator=(const IN3BookInfoEx &other); ///< assignment operator
	CBookInfo& operator=(const CBookInfo &other); ///< assignment operator



	/**
	 * fixed layout 타입 정의
	 * FIXED_PORTRAIT	: 대게 세로로 길쭉한 컨텐츠
	 * FIXED_LANDSCAPE	: 대게 가로로 길쭉한 컨텐츠
	 * FIXED_BOTH		: samsung spec 에서는 PORTRAIT 와 LANDSCAPE 모두를 지원 하는 경우를 의미한다.
	 */
	typedef enum FixedOption { FIXED_PORTRAIT = 0, FIXED_LANDSCAPE, FIXED_BOTH };

protected:	
	/**
	 * NCX : Epub 2.0 spec
	 * NAV : Epub 3.0 sepc
	 */
	typedef enum NavType { NONE, NCX, NAV };

protected:
	long		m_nNavType; ///< ncx(EPub 2.0 spec)? nav(EPub 3.0 spec)
	long		m_fixedOption; ///< fixed 타입(portrait/landscape/both)	

	CString		m_packageName; ///< package folder name	ex) "OEBPS"
	CString		m_opsFilePath; ///< opf 파일의 상대경로			ex) $(root)\OEBPS\content.opf
	CString		m_tocFilePath; ///< toc 파일의 상대경로			ex) $(root)\OEBPS\toc.xhtml or $(root)\OEBPS\toc.ncx
	CString		m_unzipRootPath; ///< unzip 된 책의 절대 경로

	// fixed content's size;
	int			m_fixedWidth; ///< fixed content's width
	int			m_fixedHeight; ///< fixed content's height

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path 를 엔진에서 제공하는 경우, unzip 된 epub 경로 안에 thumbnail 폴더를 만들어 해당 경로를 반환한다.
	// 그러나 엔진에서 섬네일 경로를 지정하여 제공할 필요가 없으므로 해당 함수는 미지원하도록 처리함.
	CString		m_epubThumnailPath;			// 썸네일을 불러오기 위해서 패스를 저장함.
#endif /*SUPPROT_THUMBNAIL_PATH*/
};
