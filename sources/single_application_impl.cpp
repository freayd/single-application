/********************************************************************************
** Copyright (c) 2009-2018 Freayd
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
********************************************************************************/

#include "single_application_impl.h"

#include "application_server.h"
#include "application_socket.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QSharedMemory>
#include <QtCore/QVariant>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#ifdef Q_OS_UNIX
#include <signal.h>
#include <errno.h>
#endif


SingleApplicationImpl::SingleApplicationImpl (const QString & key, QObject * parent)
    : q (parent), m_key (key), m_server (0), m_socket (0)
{
    // Determine if the application is already running.
    m_sharedMemory = new QSharedMemory (m_key + "sharedpid", parent);
    if (m_sharedMemory->create (sizeof (qint64)))
    {
        m_isRunning = false;

        m_sharedMemory->lock ();
        shareApplicationPid ();
        m_sharedMemory->unlock ();
    }
    else if (m_sharedMemory->error () == QSharedMemory::AlreadyExists)
    {
        m_isRunning = true;

        if (m_sharedMemory->attach ())
        {
            m_sharedMemory->lock ();
            const qint64 pid = sharedPid ();

            // On Unix, check if the PID is already running
            // because shared memory segment survives crashes.
#ifdef Q_OS_UNIX
            if (kill (pid, 0) == -1 && errno == ESRCH)
            {
                m_isRunning = false;
                shareApplicationPid ();
            }
#endif

            m_sharedMemory->unlock ();
        }
        else
            qWarning ("%s: %s", q->metaObject ()->className (),
                                qPrintable (m_sharedMemory->errorString ()));

        if (m_isRunning)
        {
            m_sharedMemory->detach ();
            m_sharedMemory->deleteLater ();
            m_sharedMemory = 0;
        }
    }
    else
    {
        m_isRunning = false;
        qWarning ("%s: %s", q->metaObject ()->className (),
                            qPrintable (m_sharedMemory->errorString ()));
        m_sharedMemory->deleteLater ();
        m_sharedMemory = 0;
    }

    const QString socketName (m_key + "singleapplication");
    if (m_isRunning)
    {
        m_socket = new ApplicationSocket (parent);
        m_socket->connectToServer (socketName);
        if (! m_socket->waitForConnected (3000))
        {
            qWarning ("%s: %s", q->metaObject ()->className (),
                                qPrintable (m_socket->errorString ()));
            m_socket->deleteLater ();
            m_socket = 0;
        }
    }
    else
    {
        // On Windows, check if another server does not already exists
        // because two local servers can listen to the same pipe.
#ifdef Q_OS_WIN32
        {
        QLocalSocket socket;
        socket.connectToServer (socketName);
        bool exists = socket.waitForConnected (3000);
        socket.disconnectFromServer ();
        if (exists)
            // FIXME Server can't be removed on Windows according to the documentation.
            QLocalServer::removeServer (socketName);
        }
#endif

        m_server = new ApplicationServer (parent);
        if (! m_server->listen (socketName) &&
                m_server->serverError () == QAbstractSocket::AddressInUseError)
        {
            // We trust in shared memory and so remove the existing server.
            QLocalServer::removeServer (socketName);
            m_server->listen (socketName);
        }

        if (! m_server->isListening ())
            qWarning ("%s: %s", q->metaObject ()->className (),
                                qPrintable (m_server->errorString ()));
    }
}

bool SingleApplicationImpl::sendObject (const QVariant & object, int timeout /* = 500 */)
{
    return m_socket && m_socket->sendObject (object, timeout);
}

void SingleApplicationImpl::shareApplicationPid ()
{
    const qint64 pid = QCoreApplication::applicationPid ();
    memcpy (m_sharedMemory->data (), & pid, sizeof (qint64));
}

qint64 SingleApplicationImpl::sharedPid ()
{
    return * (qint64 *) m_sharedMemory->constData ();
}
