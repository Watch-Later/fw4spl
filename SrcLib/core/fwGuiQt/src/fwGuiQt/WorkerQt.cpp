/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGuiQt/WorkerQt.hpp"

#include "fwGuiQt/App.hpp"
#include "fwGuiQt/util/FuncSlot.hpp"

#include <fwCore/util/LazyInstantiator.hpp>

#include <fwGui/registry/worker.hpp>

#include <fwThread/Worker.hpp>
#include <fwThread/Timer.hpp>

#include <fwServices/registry/ActiveWorkers.hpp>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <QEvent>
#include <QDir>
#include <QPointer>
#include <QTimer>
#include <QStringList>
#include <QSharedPointer>
#include <QFont>


namespace fwGuiQt
{

class WorkerQtTask : public QEvent
{
public:
    WorkerQtTask( const ::fwThread::Worker::TaskType &handler )
        : QEvent( static_cast< QEvent::Type >(s_WORKER_QT_TASK_EVENT_TYPE) ),
        m_handler( handler )
    {
        SLM_ASSERT( "QApplication should be instantiated", qApp );
    }

    ~WorkerQtTask()
    {
        m_handler();
    }

    static const int s_WORKER_QT_TASK_EVENT_TYPE;

protected:

    ::fwThread::Worker::TaskType m_handler;
};

const int WorkerQtTask::s_WORKER_QT_TASK_EVENT_TYPE = QEvent::registerEventType();



/**
 * @brief Private implementation of fwThread::Worker using boost::asio.
 */
class WorkerQt : public ::fwThread::Worker
{
public:
    typedef ::boost::thread ThreadType;

    WorkerQt();

    void init( int &argc, char **argv );

    virtual ~WorkerQt();

    void stop();

    void post(TaskType handler);

    ::fwThread::Worker::FutureType getFuture();

    virtual ::fwThread::ThreadIdType getThreadId() const;

    virtual void processTasks();

    virtual void processTasks(PeriodType maxtime);

protected:

    QSharedPointer< QApplication > m_app;

    SPTR(::fwThread::Timer) createTimer();

    /// Copy constructor forbidden
    WorkerQt( const WorkerQt& );

    /// Copy operator forbidden
    WorkerQt& operator=( const WorkerQt& );

    ::fwThread::ThreadIdType m_threadId;

};



struct FWGUIQT_CLASS_API WorkerQtInstanciator
{

    FWGUIQT_API WorkerQtInstanciator(bool reg = true ) ;

    FWGUIQT_API SPTR(::fwThread::Worker) getWorker();

    SPTR(WorkerQt) m_qtWorker;

    FWGUIQT_API static int    s_argc;
    FWGUIQT_API static char **s_argv;
    FWGUIQT_API static bool   s_GUIenabled;
};

int WorkerQtInstanciator::s_argc = 0 ;
char** WorkerQtInstanciator::s_argv = NULL;
bool WorkerQtInstanciator::s_GUIenabled = true;

FWGUIQT_API WorkerQtInstanciator::WorkerQtInstanciator(bool reg) :
    m_qtWorker(::boost::make_shared< WorkerQt >())
{
    m_qtWorker->init( boost::ref(s_argc), s_argv);

    if(reg)
    {
        ::fwServices::registry::ActiveWorkers::getDefault()
            ->addWorker(::fwServices::registry::ActiveWorkers::s_DEFAULT_WORKER, m_qtWorker);
    }
}


SPTR(::fwThread::Worker) WorkerQtInstanciator::getWorker()
{
    return m_qtWorker;
}

::fwThread::Worker::sptr getQtWorker( int &argc, char **argv )
{

    WorkerQtInstanciator::s_argc = argc;
    WorkerQtInstanciator::s_argv = argv;

    typedef ::fwCore::util::LazyInstantiator< WorkerQtInstanciator, WorkerQtInstanciator > InstantiatorType;

    SPTR(WorkerQtInstanciator) instanciator = InstantiatorType::getInstance();

#ifdef _DEBUG
    {
        static ::boost::mutex mutex;
        ::boost::mutex::scoped_lock scoped_lock(mutex);
        static bool initialized = false;
        SLM_ASSERT("getQtWorker(argc, argv) shall be called only once", !initialized);
        initialized = true;
    }
#endif


    return instanciator->m_qtWorker;
}



//------------------------------------------------------------------------------


/**
 * @class TimerQt
 * @brief Private Timer implementation using Qt.
 *
 *
 * @date   2012.
 */
class TimerQt : public ::fwThread::Timer
{
public:
    /**
     * @brief Constructs a TimerQt from given io_service.
     */
    TimerQt();

    ~TimerQt();

    /// Starts or restarts the timer.
    void start();

    /// Stops the timer and cancel all pending operations.
    void stop();

    /// Sets time duration.
    void setDuration(TimeDurationType duration);

    /// Returns if the timer mode is 'one shot'.
    bool isOneShot() const
    {
        ::fwCore::mt::ScopedLock lock(m_mutex);
        return m_timerQt->isSingleShot();
    }

    /// Sets timer mode.
    void setOneShot(bool oneShot)
    {
        ::fwCore::mt::ScopedLock lock(m_mutex);
        m_timerQt->setSingleShot(oneShot);
    }

    /// Returns true if the timer is currently running.
    bool isRunning() const
    {
        ::fwCore::mt::ScopedLock lock(m_mutex);
        return m_timerQt->isActive();
    }

protected Q_SLOTS:

    void call();

protected:

    /// Copy constructor forbidden.
    TimerQt( const TimerQt& );

    /// Copy operator forbidden.
    TimerQt& operator=( const TimerQt& );

    void updatedFunction();

    QPointer< QTimer > m_timerQt;

    QPointer< ::fwGuiQt::util::FuncSlot > m_qtFunc;
};

//------------------------------------------------------------------------------


// ---------- WorkerQt private implementation ----------

WorkerQt::WorkerQt() :
    m_app(NULL),
    m_threadId( ::fwThread::getCurrentThreadId() )
{
}

void WorkerQt::init( int &argc, char **argv )
{
#ifdef __MACOSX__
    if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 )
    {
        // https://bugreports.qt-project.org/browse/QTBUG-32789 #fix for Mac OS X 10.9
        QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
    }
#endif

    m_app = QSharedPointer< QApplication > ( new ::fwGuiQt::App( argc, argv, WorkerQtInstanciator::s_GUIenabled ) );

    OSLM_TRACE("Init Qt" << ::fwThread::getCurrentThreadId() <<" Start");

    QDir pluginDir("./qtplugins");
    if (pluginDir.exists())
    {
        QCoreApplication::setLibraryPaths(QStringList(pluginDir.absolutePath()));
    }

    OSLM_TRACE("Init Qt" << ::fwThread::getCurrentThreadId() <<" Finish");

}

WorkerQt::~WorkerQt()
{
    this->stop();
}


::fwThread::Worker::FutureType WorkerQt::getFuture()
{
    if (! m_future.valid() )
    {
        SLM_ASSERT("WorkerQt loop shall be created and ran from main thread ",
            ! m_future.valid() && ::fwThread::getCurrentThreadId() == this->getThreadId() );

        ::boost::packaged_task< ExitReturnType > task( ::boost::bind(&QApplication::exec) );

        ::boost::future< ExitReturnType > ufuture = task.get_future();

        m_future = ::boost::move(ufuture);

        task();
    }

    return m_future;
}


::fwThread::ThreadIdType WorkerQt::getThreadId() const
{
    return m_threadId;
}


void WorkerQt::stop()
{
    this->postTask<void>(&QApplication::quit).wait();
}

SPTR(::fwThread::Timer) WorkerQt::createTimer()
{
    return ::boost::make_shared< TimerQt >();
}

void WorkerQt::post(TaskType handler)
{
    QApplication::postEvent( qApp, new WorkerQtTask(handler) );
}

void WorkerQt::processTasks()
{
    QApplication::sendPostedEvents(0, ::fwGuiQt::WorkerQtTask::s_WORKER_QT_TASK_EVENT_TYPE);
}

void WorkerQt::processTasks(PeriodType maxtime)
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, int(maxtime));
}

// ---------- Timer private implementation ----------

TimerQt::TimerQt() :
    m_timerQt( new QTimer(qApp) )
{
    m_qtFunc = new ::fwGuiQt::util::FuncSlot();
    QObject::connect(m_timerQt, SIGNAL(timeout()), m_qtFunc, SLOT(trigger()));
}

TimerQt::~TimerQt()
{
    QObject::disconnect(m_timerQt, SIGNAL(timeout()), m_qtFunc, SLOT(trigger()));
    delete m_qtFunc;
    m_timerQt->stop();
    delete m_timerQt;
}

void TimerQt::setDuration(TimeDurationType duration)
{
    ::fwCore::mt::ScopedLock lock(m_mutex);
    m_timerQt->setInterval( static_cast<int>(
                                    ::boost::chrono::duration_cast< ::boost::chrono::milliseconds >(duration).count())
                            );
}

void TimerQt::start()
{
    ::fwCore::mt::ScopedLock lock(m_mutex);
    m_timerQt->start();
}

void TimerQt::stop()
{
    ::fwCore::mt::ScopedLock lock(m_mutex);
    m_timerQt->stop();
}


void TimerQt::call()
{
    m_function();
}


void TimerQt::updatedFunction()
{
    m_qtFunc->setFunction(m_function);
}


} //namespace fwGuiQt

