#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhoMutexLock.h"
#include "sqlite/sqlite3.h"

namespace rho{
namespace db{

class CDBAdapter;
class CDBResult
{
    CDBAdapter* m_pDB;
    sqlite3_stmt* m_dbStatement;
    boolean m_bReportNonUnique;
    int     m_nErrorCode;
public:
    CDBResult(sqlite3_stmt* st,CDBAdapter* pDB);
    CDBResult();
    ~CDBResult(void);

    void setStatement(sqlite3_stmt* st);
    sqlite3_stmt* getStatement(){ return m_dbStatement; }
    boolean getReportNonUnique(){ return m_bReportNonUnique; }
    void setReportNonUnique(boolean bSet){ m_bReportNonUnique = bSet; }
    boolean isNonUnique(){ return m_nErrorCode==SQLITE_CONSTRAINT; }
    boolean isError(){ return m_nErrorCode!=SQLITE_OK; }
    int     getErrorCode(){ return m_nErrorCode; }
    void    setErrorCode(int nError){ m_nErrorCode=nError; }

    virtual bool isEnd(){ return m_dbStatement == null; }
    void next()
    {
        if ( sqlite3_step(m_dbStatement) != SQLITE_ROW )
            setStatement(null);
    }

    virtual String getStringByIdx(int nCol)
    {
        char* res = (char *)sqlite3_column_text(m_dbStatement, nCol);
        return res ? res : String();
    }

    int getIntByIdx(int nCol)
    {
        return sqlite3_column_int(m_dbStatement, nCol);
    }

    uint64 getUInt64ByIdx(int nCol)
    {
        return sqlite3_column_int64(m_dbStatement, nCol);
    }

    int getColCount()
    {
        return sqlite3_data_count(m_dbStatement);
    }

    boolean isNullByIdx(int nCol)
    {
        return sqlite3_column_type(m_dbStatement,nCol) == SQLITE_NULL;
    }

    String getColName(int nCol)
    {
        return sqlite3_column_name(m_dbStatement,nCol);;
    }

};

typedef rho::common::CAutoPtr<rho::db::CDBResult> DBResultPtr;
#define DBResult(name, call)\
    rho::db::DBResultPtr p##name = call;\
    rho::db::CDBResult& name = *p##name;

}
}
