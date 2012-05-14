#pragma once

#include "IN3WebkitDefine.h"
#include "IN3BookInfo.h"

/**
  @class	CBookInfo
  @remarks	IN3BookInfoEx �� ��ӹ��� Ŭ�����̴�. ���������� ���Ǵ� �������� ���ԵǾ� �ִ�.
  @brief	å����
 */
class AFX_EXT_CLASS CBookInfo : public IN3BookInfoEx
{
public:
	// ������
	CBookInfo(void);

	// �Ҹ���
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
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
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
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
	const CString GetThumbnailPath() const;
#endif /*SUPPROT_THUMBNAIL_PATH*/


	// overloading
	CBookInfo& operator=(const IN3BookInfo &other); ///< assignment operator
	CBookInfo& operator=(const IN3BookInfoEx &other); ///< assignment operator
	CBookInfo& operator=(const CBookInfo &other); ///< assignment operator



	/**
	 * fixed layout Ÿ�� ����
	 * FIXED_PORTRAIT	: ��� ���η� ������ ������
	 * FIXED_LANDSCAPE	: ��� ���η� ������ ������
	 * FIXED_BOTH		: samsung spec ������ PORTRAIT �� LANDSCAPE ��θ� ���� �ϴ� ��츦 �ǹ��Ѵ�.
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
	long		m_fixedOption; ///< fixed Ÿ��(portrait/landscape/both)	

	CString		m_packageName; ///< package folder name	ex) "OEBPS"
	CString		m_opsFilePath; ///< opf ������ �����			ex) $(root)\OEBPS\content.opf
	CString		m_tocFilePath; ///< toc ������ �����			ex) $(root)\OEBPS\toc.xhtml or $(root)\OEBPS\toc.ncx
	CString		m_unzipRootPath; ///< unzip �� å�� ���� ���

	// fixed content's size;
	int			m_fixedWidth; ///< fixed content's width
	int			m_fixedHeight; ///< fixed content's height

#if SUPPORT_THUMBNAIL_PATH
	// thumbnail path �� �������� �����ϴ� ���, unzip �� epub ��� �ȿ� thumbnail ������ ����� �ش� ��θ� ��ȯ�Ѵ�.
	// �׷��� �������� ������ ��θ� �����Ͽ� ������ �ʿ䰡 �����Ƿ� �ش� �Լ��� �������ϵ��� ó����.
	CString		m_epubThumnailPath;			// ������� �ҷ����� ���ؼ� �н��� ������.
#endif /*SUPPROT_THUMBNAIL_PATH*/
};
