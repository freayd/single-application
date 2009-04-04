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

#include "single_application.h"

#include "application_socket.h"
#include "single_application_impl.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


void SingleApplication::setDataStreamVersion (QDataStream::Version v)
{
    ApplicationSocket::setDataStreamVersion (v);
}

SingleApplication::SingleApplication (const QString & key, int & argc, char ** argv)
    : QApplication (argc, argv), d (new SingleApplicationImpl (key, this))
{
}

SingleApplication::SingleApplication (const QString & key, int & argc, char ** argv, bool GUIenabled)
    : QApplication (argc, argv, GUIenabled), d (new SingleApplicationImpl (key, this))
{
}

SingleApplication::SingleApplication (const QString & key, int & argc, char ** argv, Type type)
    : QApplication (argc, argv, type), d (new SingleApplicationImpl (key, this))
{
}

bool SingleApplication::isRunning () const
{
    return d->isRunning ();
}

bool SingleApplication::sendMessage (const QString & message, int timeout /* = 500 */)
{
    QVariant object (message);
    return d->sendObject (object, timeout);
}

bool SingleApplication::sendArguments (int timeout /* = 500 */)
{
    QVariant object (QCoreApplication::arguments ());
    return d->sendObject (object, timeout);
}

bool SingleApplication::sendObject (const QVariant & object, int timeout /* = 500 */)
{
    return d-sendObject (object, timeout);
}

void SingleApplication::processObject (const QVariant & object)
{
    if (object.type () == QVariant::String)
        emit messageReceived (object.toString ());
    else if (object.type () == QVariant::StringList)
        emit argumentsReceived (object.toStringList ());
    else
        emit objectReceived (object);
}
