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

#ifndef APPLICATION_SOCKET_H
#define APPLICATION_SOCKET_H

#include <QtCore/QDataStream>
#include <QtNetwork/QLocalSocket>


class QVariant;

class ApplicationSocket : public QLocalSocket
{
    Q_OBJECT

public:
    static void setDataStreamVersion (QDataStream::Version v);

    ApplicationSocket (QObject * parent = 0);

public slots:
    bool sendObject (const QVariant & object, int timeout);

signals:
    void objectReceived (const QVariant & object);

private slots:
    void readData ();
    void displayError (QLocalSocket::LocalSocketError error);

private:
    static int * s_dataStreamVersion;

    quint64 m_blockSize;

};

#endif // APPLICATION_SOCKET_H
