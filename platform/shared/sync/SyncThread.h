#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/SyncEngine.h"
#include "common/ThreadQueue.h"

namespace rho {
namespace sync {

class CSyncThread : public common::CThreadQueue
{
public:
    enum ESyncCommands{ scNone = 0, scSyncAll, scSyncOne, scLogin, scSearchOne};

private:

    DEFINE_LOGCLASS;

public:
    static const unsigned int SYNC_WAIT_BEFOREKILL_SECONDS  = 3;

    class CSyncCommand : public CQueueCommand
    {
    public:
	    int m_nCmdCode;
	    int m_nCmdParam;
	    String m_strCmdParam;
   		boolean m_bShowStatus;

	    CSyncCommand(int nCode, int nParam, boolean bShowStatus)
	    {
		    m_nCmdCode = nCode;
		    m_nCmdParam = nParam;
            m_bShowStatus = bShowStatus;
	    }
	    CSyncCommand(int nCode, String strParam, boolean bShowStatus)
	    {
		    m_nCmdCode = nCode;
		    m_strCmdParam = strParam;
            m_bShowStatus = bShowStatus;
	    }
	    CSyncCommand(int nCode, String strParam, int nCmdParam, boolean bShowStatus)
	    {
		    m_nCmdCode = nCode;
		    m_strCmdParam = strParam;
            m_nCmdParam = nCmdParam;
            m_bShowStatus = bShowStatus;
	    }

	    CSyncCommand(int nCode, boolean bShowStatus)
	    {
		    m_nCmdCode = nCode;
		    m_nCmdParam = 0;
            m_bShowStatus = bShowStatus;
	    }

	    boolean equals(const CQueueCommand& cmd)
	    {
            const CSyncCommand& oSyncCmd = (const CSyncCommand&)cmd;
		    return m_nCmdCode == oSyncCmd.m_nCmdCode && m_nCmdParam == oSyncCmd.m_nCmdParam &&
			    m_strCmdParam == oSyncCmd.m_strCmdParam;
	    }

        String toString()
        {
            switch(m_nCmdCode)
            {
            case scSyncAll:
                return "SyncAll";
            case scSyncOne:
                return "SyncOne";
            case scLogin:
                return "Login";
            case scSearchOne:
                return "Search";
            }
            return "Unknown";
        }

    };

    class CSyncLoginCommand : public CSyncCommand
    {
    public:
	    String m_strName, m_strPassword;
        CSyncLoginCommand(String name, String password, String callback) : CSyncCommand(CSyncThread::scLogin,callback,false)
	    {
		    m_strName = name;
		    m_strPassword = password;
	    }
    };
    class CSyncSearchCommand : public CSyncCommand
    {
    public:
	    String m_strFrom;
        boolean m_bSyncChanges;
        rho::Vector<rho::String> m_arSources;

        CSyncSearchCommand(String from, String params, rho::Vector<rho::String>& arSources, boolean sync_changes, int nProgressStep) : CSyncCommand(CSyncThread::scSearchOne,params,nProgressStep, false)
	    {
		    m_strFrom = from;
            m_bSyncChanges = sync_changes;
            m_arSources = arSources;
	    }
    };

private:
    static CSyncThread* m_pInstance;

    CSyncEngine     m_oSyncEngine;
public:
    ~CSyncThread(void);

    static CSyncThread* Create(common::IRhoClassFactory* factory);
    static void Destroy();
    static CSyncThread* getInstance(){ return m_pInstance; }
    static CSyncEngine& getSyncEngine(){ return m_pInstance->m_oSyncEngine; }

	void setPollInterval(int nInterval);
private:
    CSyncThread(common::IRhoClassFactory* factory);

    virtual int getLastPollInterval();
    virtual void processCommand(CQueueCommand* pCmd);
    virtual boolean isSkipDuplicateCmd() { return true; }

    virtual void processCommands();

    void checkShowStatus(CSyncCommand& oSyncCmd);
};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_sync_create();
void rho_sync_destroy();

void rho_sync_doSyncAllSources(int show_status_popup);
void rho_sync_doSyncSource(unsigned long nSrcID,int show_status_popup);
void rho_sync_doSearch(unsigned long ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, const char* callback, const char* callback_params);
void rho_sync_doSyncSourceByUrl(const char* szSrcID);
void rho_sync_login(const char *login, const char *password, const char* callback);
int rho_sync_logged_in();
void rho_sync_logout();
void rho_sync_set_notification(int source_id, const char *url, char* params);
void rho_sync_clear_notification(int source_id);
void rho_sync_set_pollinterval(int nInterval);
void rho_sync_set_syncserver(char* syncserver);
void rho_sync_setobjectnotify_url(const char* szUrl);
void rho_sync_addobjectnotify(int nSrcID, const char* szObject);
void rho_sync_cleanobjectnotify();
int rho_sync_get_pagesize();
void rho_sync_set_pagesize(int nPageSize);
void rho_sync_set_bulk_notification(const char *url, char* params);
void rho_sync_clear_bulk_notification();

unsigned long rho_sync_get_attrs(const char* szPartition, int nSrcID);
unsigned long rho_sync_is_blob_attr(const char* szPartition, int source_id, const char* szAttrName);
int rho_sync_get_lastsync_objectcount(int nSrcID);

#ifdef __cplusplus
};
#endif //__cplusplus


