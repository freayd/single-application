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

#include "application_socket.h"

#include <QtCore/QByteArray>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


int * ApplicationSocket::s_dataStreamVersion = 0;

void ApplicationSocket::setDataStreamVersion (QDataStream::Version v)
{
    if (s_dataStreamVersion)
        delete s_dataStreamVersion;
    s_dataStreamVersion = new int (v);
}

ApplicationSocket::ApplicationSocket (QObject * qPointer, QObject * parent /* = 0 */)
    : QLocalSocket (parent), q (qPointer), m_blockSize (0)
{
    connect (this, SIGNAL(readyRead ()), SLOT(readData ()));
    connect (this, SIGNAL(error (QLocalSocket::LocalSocketError)),
                   SLOT(displayError (QLocalSocket::LocalSocketError)));
}

bool ApplicationSocket::sendObject (const QVariant & object, int timeout)
{
    if (state () != ConnectedState)
        return false;

    QByteArray block;
    QDataStream out (&block, WriteOnly);
    if (s_dataStreamVersion)
        out.setVersion (* s_dataStreamVersion);
    out << (quint64) 0;
    out << object;
    out.device ()->seek (0);
    out << (quint64) (block.size () - sizeof (quint64));

    return write (block) != -1 && waitForBytesWritten (timeout);
}

void ApplicationSocket::readData ()
{
    if (state () != ConnectedState)
        return;

    QDataStream in (this);
    if (s_dataStreamVersion)
        in.setVersion (* s_dataStreamVersion);

    if (m_blockSize == 0)
    {
        if (bytesAvailable () < (int) sizeof (quint64))
            return;
        in >> m_blockSize;
    }

    if (in.atEnd () || bytesAvailable () < (qint64) m_blockSize)
        return;

    QVariant object;
    in >> object;
    m_blockSize = 0;

    const char * signal;
    QGenericArgument argument;
    if (object.type () == QVariant::String)
    {
        signal = "messageReceived(QString)";
        argument = Q_ARG(QString, object.toString ());
    }
    else if (object.type () == QVariant::StringList)
    {
        signal = "argumentsReceived(QStringList)";
        argument = Q_ARG(QStringList, object.toStringList ());
    }
    else
    {
        signal = "objectReceived(QVariant)";
        argument = Q_ARG(QVariant, object);
    }

    const QMetaObject * qMetaObject = q->metaObject ();
    if (! qMetaObject->method (qMetaObject->indexOfSignal (signal)).invoke (q, argument))
        qWarning ("%s: Failed to emit signal %s.", q->metaObject ()->className (),
                                                   signal);
}

void ApplicationSocket::displayError (QLocalSocket::LocalSocketError error)
{
    if (error != PeerClosedError)
        qWarning ("%s: %s", q->metaObject ()->className (),
                            qPrintable (errorString ()));
}
