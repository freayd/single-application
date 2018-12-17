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

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QtCore/QDataStream>
#include <QtGui/QApplication>


class SingleApplicationImpl;

class QString;
class QStringList;
class QVariant;

class SingleApplication : public QApplication
{
    Q_OBJECT

public:
    /**
     * Sets the version number of the data serialization format.
     * This function should be called before the constructor.
     */
    static void setDataStreamVersion (QDataStream::Version v);

    SingleApplication (const QString & key, int & argc, char ** argv);
    SingleApplication (const QString & key, int & argc, char ** argv, bool GUIenabled);
    SingleApplication (const QString & key, int & argc, char ** argv, Type type);
    virtual ~SingleApplication ();

    bool isRunning () const;

public Q_SLOTS:
    bool sendMessage   (const QString & message, int timeout = 500);
    bool sendArguments (int timeout = 500);
    bool sendObject    (const QVariant & object, int timeout = 500);

Q_SIGNALS:
    void messageReceived   (const QString     & message  );
    void argumentsReceived (const QStringList & arguments);
    void objectReceived    (const QVariant    & object   );

private:
    friend class SingleAppicationImpl;
    SingleApplicationImpl * const d;

};

#endif // SINGLE_APPLICATION_H
