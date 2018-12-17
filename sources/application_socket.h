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

    ApplicationSocket (QObject * qPointer, QObject * parent = 0);

    bool sendObject (const QVariant & object, int timeout);

private Q_SLOTS:
    void readData ();
    void displayError (QLocalSocket::LocalSocketError error);

private:
    static int * s_dataStreamVersion;

    QObject * const q;
    quint64 m_blockSize;

};

#endif // APPLICATION_SOCKET_H
