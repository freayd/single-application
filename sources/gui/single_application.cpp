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

SingleApplication::~SingleApplication ()
{
    delete d;
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
