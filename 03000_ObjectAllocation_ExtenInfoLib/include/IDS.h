#pragma once

#include "map"
enum e_InitMode {
	e_InitMode_RealTime,
	e_InitMode_RightAway
};
/*****************************************************************************************/
class CIDS
{
public:
	CIDS() {}
	//*[7/6/2017 ConanSteve]*:  初始化字符串文件
	bool Init(TCHAR* const sFilePath);
	bool Init(CString const &sFilePath);
	//*[7/6/2017 ConanSteve]*:  恢复默认字符串
	void ResetDefault();
	//*[7/6/2017 ConanSteve]*:  翻译字符串
	CString Translate(TCHAR* const tchar);
	void SetAppName(CString sModuleName);
	~CIDS() {}
	
private:
	std::map<CString, CString> m_mapIDS;
	CString m_sLangFilePath;
	CString m_sModuleName{ TEXT("StringTable") };
	bool bPathFileExist;
	bool bDefault;

};


/*----------------------------------------------------------------------------
vars.h
注：在包含vars.h头文件声明和初始化全局变量前面使用 #define VAR_DECLS 1语句。
只是包含它，而不定义VAR_DECLS以引用变量。
----------------------------------------------------------------------------*/
#ifndef VAR_DEFS          // 保证文件只被包含一次  
#define VAR_DEFS 1  
/*----------------------------------------------
设置变量声明宏
----------------------------------------------*/
#ifndef VAR_DECLS  
# define _DECL extern  
# define _INIT(x)  
#else  
# define _DECL  
# define _INIT(x)  = x  
#endif  
/*----------------------------------------------
声明变量如下：
_DECL [标准变量声明] _INIT(x);
x是初始化变量的值。如果变量没有初始化的值，可以如下声明
_DECL [标准变量声明];
----------------------------------------------*/
_DECL CIDS theIDS;
#endif  
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/




#define tr(quote) theIDS.Translate(TEXT(quote))
#define InitLanguage(quote)  theIDS.Init(quote)
#define ResetStringTable theIDS.ResetDefault()
#define SetModuleName(quote)  theIDS.SetAppName(quote)



