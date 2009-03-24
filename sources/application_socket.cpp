/********************************************************************************
** This file is part of SingleApplication, a single instance application library.
** Copyright (C) 2009 freayd <freayd@gmail.com>
**
** SingleApplication is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SingleApplication is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with SingleApplication. If not, see <http://www.gnu.org/licenses/>.
********************************************************************************/

#include "application_socket.h"

#include <QtCore/QByteArray>


int * ApplicationSocket::s_dataStreamVersion = 0;

void ApplicationSocket::setDataStreamVersion (QDataStream::Version v)
{
    if (s_dataStreamVersion)
        delete s_dataStreamVersion;
    s_dataStreamVersion = new int (v);
}

ApplicationSocket::ApplicationSocket (QObject * parent /* = 0 */)
    : QLocalSocket (parent), m_blockSize (0)
{
    connect (this, SIGNAL(readyRead ()), SLOT(readMessage ()));
    connect (this, SIGNAL(error (QLocalSocket::LocalSocketError)),
                   SLOT(displayError (QLocalSocket::LocalSocketError)));
}

bool ApplicationSocket::sendMessage (const QString & message, int timeout)
{
    QVariant variant (message);
    return sendVariant (variant, timeout);
}

bool ApplicationSocket::sendArguments (const QStringList & arguments, int timeout)
{
    QVariant variant (arguments);
    return sendVariant (variant, timeout);
}

bool ApplicationSocket::sendVariant (const QVariant & variant, int timeout)
{
    if (state () != ConnectedState)
        return false;

    QByteArray block;
    QDataStream out (&block, WriteOnly);
    if (s_dataStreamVersion)
        out.setVersion (* s_dataStreamVersion);
    out << (quint64) 0;
    out << variant;
    out.device ()->seek (0);
    out << (quint64) (block.size () - sizeof (quint64));

    return write (block) != -1 && waitForBytesWritten (timeout);
}

void ApplicationSocket::readMessage ()
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

    QVariant variant;
    in >> variant;
    m_blockSize = 0;

    if (variant.type () == QVariant::String)
        emit messageReceived (variant.toString ());
    else if (variant.type () == QVariant::StringList)
        emit argumentsReceived (variant.toStringList ());
    else
        emit variantReceived (variant);
}

void ApplicationSocket::displayError (QLocalSocket::LocalSocketError error)
{
    if (error != PeerClosedError)
        qWarning ("ApplicationSocket: %s", errorString ().toUtf8 ().constData ());
}
