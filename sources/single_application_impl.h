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
