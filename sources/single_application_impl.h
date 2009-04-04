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

#ifndef SINGLE_APPLICATION_IMPL_H
#define SINGLE_APPLICATION_IMPL_H

#include <QtCore/QString>


class ApplicationServer;
class ApplicationSocket;

class QObject;
class QSharedMemory;
class QVariant;

class SingleApplicationImpl
{
public:
    SingleApplicationImpl (const QString & key, QObject * parent);

    inline bool isRunning () const { return m_isRunning; }

    bool sendObject (const QVariant & object, int timeout = 500);
    void processObject (const QVariant & object);

private:
    void shareApplicationPid ();
    qint64 sharedPid ();

    QObject * const q;

    const QString m_key;
    bool m_isRunning;
    QSharedMemory     * m_sharedMemory;
    ApplicationServer * m_server;
    ApplicationSocket * m_socket;

};

#endif // SINGLE_APPLICATION_IMPL_H
