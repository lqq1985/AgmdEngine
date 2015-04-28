#ifndef _THREAD_H_
#define _THREAD_H_

#include <windows.h>
#include <Winbase.h>
#include <assert.h>


/**
**************************************************************************************************************************************************************
* Mutex classes using win32 critical section
**************************************************************************************************************************************************************/
class Mutex
{
public:
	Mutex	();
	~Mutex	();

	void Lock() ;
	void Unlock() ;

private:
	CRITICAL_SECTION m_CriticalSection;
};

/**
**************************************************************************************************************************************************************
* Mutex constructor
**************************************************************************************************************************************************************/
Mutex::Mutex()
{
	if(!InitializeCriticalSectionAndSpinCount(&m_CriticalSection,10))
	{
		assert(0);
	}
}

Mutex::~Mutex()
{
	::DeleteCriticalSection(&m_CriticalSection);
}
/**
**************************************************************************************************************************************************************
* Mutex lock
**************************************************************************************************************************************************************/
void Mutex::Lock() 
{
	::EnterCriticalSection(&m_CriticalSection);
}


/**
**************************************************************************************************************************************************************
* Mutex unlock
**************************************************************************************************************************************************************/
void Mutex::Unlock() 
{
	::LeaveCriticalSection(&m_CriticalSection);
}

/**
 **************************************************************************************************************************************************************
 * Thread class
 **************************************************************************************************************************************************************/
class CThread  
{
public:
                                    CThread                     ();
    virtual                        ~CThread                     ();

			bool                    Init                        ();
            void                    Release                     ();

			virtual unsigned int	Run							() = NULL;

			unsigned int			GetThreadId					() const {return m_dwThreadID; }
private:
    void*			m_hThread;
    unsigned int	m_dwThreadID;
};

/**************************************************************************************************************************************************************/

static unsigned int WINAPI FnThread(CThread* pThis)
{
    return pThis->Run();
}
/**
 **************************************************************************************************************************************************************
 * Thread constructor
 **************************************************************************************************************************************************************/
CThread::CThread()
{
    m_dwThreadID    = 0;
    m_hThread       = NULL;
}

/**
 **************************************************************************************************************************************************************
 * Thread destructor
 **************************************************************************************************************************************************************/
CThread::~CThread() 
{
    Release(); 
}

/**
 **************************************************************************************************************************************************************
 * Thread initialization
 **************************************************************************************************************************************************************
 * @param pointer to a thread base
 **************************************************************************************************************************************************************/
bool CThread::Init()
{
    Release();

    m_hThread = ::CreateThread
    (
        0,
        0,
        (LPTHREAD_START_ROUTINE)FnThread,
        this,
        0,
        (LPDWORD)&m_dwThreadID 
    );

    return m_hThread != 0;
}

/**
 **************************************************************************************************************************************************************
 * Release thread
 **************************************************************************************************************************************************************/
void CThread::Release()
{
    if (m_hThread)
    {
        ::WaitForSingleObject(m_hThread, 0);
        ::CloseHandle(m_hThread);

        m_hThread = 0;
    }
}

#endif