/****************************************************************************
 * Copyright (c) 2005-2009 Kohei Yoshida
 * 
 * This code is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3 only,
 * as published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License version 3 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with this work.  If not, see
 * <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef _SCSOLVER_UI_RESMGR_HXX_
#define _SCSOLVER_UI_RESMGR_HXX_

#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/resource/XStringResourceManager.hpp>
#include <com/sun/star/lang/Locale.hpp>
#include "rtl/ustring.hxx"

#include <vector>

namespace com { namespace sun { namespace star { 
    namespace ucb {
        class XSimpleFileAccess;
    }
}}}

namespace scsolver {

class CalcInterface;

class StringResMgr
{
public:
    struct Entry
    {
        ::rtl::OUString Name;
        ::rtl::OUString Value;
    };

    StringResMgr(CalcInterface* pCalc);
    ~StringResMgr();

    /** 
     * Get current system locale. 
     * 
     * @return const ::rtl::OUString current system locale, or empty string if
     *         it is unknown.
     */
    const ::rtl::OUString getSystemLocaleString() const;

    const ::com::sun::star::lang::Locale getSystemLocale() const;

    /** 
     * Get a localized string from a numeric resource ID.  Internally this 
     * method maps the numeric resource ID with an associated string resource 
     * name. 
     * 
     * @param resid resource ID
     * 
     * @return ::rtl::OUString
     */
    const ::rtl::OUString getLocaleStr(int resid);

    const ::rtl::OUString getLocaleStr(const ::rtl::OUString& resName);

private:
    struct PropertiesFile
    {
        ::rtl::OUString FilePath;
        ::com::sun::star::lang::Locale Locale;
    };

    void init();

    ::rtl::OUString getResNameByID(int resid);

    /** 
     * Load locale strings from embedded translation file based on current
     * system locale.
     */
    void loadStrings();

    void loadStrings(const PropertiesFile& propFile);

    void getPropertiesFiles(::std::vector<PropertiesFile>& files);

    void parsePropertiesStream(const ::com::sun::star::uno::Sequence<sal_Int8>& bytes,
                               ::std::vector<Entry>& rEntries);

    ::com::sun::star::uno::Reference< ::com::sun::star::ucb::XSimpleFileAccess >
        getSimpleFileAccess();

private:
    ::com::sun::star::uno::Reference< ::com::sun::star::resource::XStringResourceManager >
        mxStrResMgr;
    ::com::sun::star::uno::Reference< ::com::sun::star::ucb::XSimpleFileAccess >
        mxFileAccess;
    ::com::sun::star::lang::Locale maDefaultLocale;
    ::com::sun::star::lang::Locale maCurrentLocale;
    CalcInterface* mpCalc;
    ::rtl::OUString msBaseTransDirPath;
    ::std::vector< ::rtl::OUString > mResNameMapper;
    bool mbStringLoaded:1;
};

// ---------------------------------------------------------------------------

class PropStreamParser
{
public:

    PropStreamParser(const ::com::sun::star::uno::Sequence<sal_Int8>& bytes);
    ~PropStreamParser();

    /** 
     * Parse a string line like this:
     * 
     * 23.DialogName.WidgetName.Label=This is text label
     * 
     * The first numerical segment must be ignored.  A comment begins with a
     * '#' and all characters after it will be ignored until it reaches the
     * linefeed.
     */
    void parse();
    void getEntries(::std::vector<StringResMgr::Entry>& rEntries) const;

private:
    PropStreamParser();

    void advanceToLinefeed(sal_Int32& i) const;
    void purgeBuffer(::rtl::OUString& rValue, ::std::vector<sal_Char>& rBuf) const;
    void pushEntry(const ::rtl::OUString& name, const ::rtl::OUString& value);

private:
    const ::com::sun::star::uno::Sequence<sal_Int8>& mrBytes;
    ::std::vector<StringResMgr::Entry> mEntries;
};










}

#endif
