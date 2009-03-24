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

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtGui/QApplication>


class ApplicationServer;
class ApplicationSocket;

class QSharedMemory;

class SingleApplication : public QApplication
{
    Q_OBJECT

public:
    /**
     * Sets the version number of the data serialization format.
     * This function should be called before the constructor.
     */
    static void setDataStreamVersion (QDataStream::Version v);

    SingleApplication (const QString & key, int & argc, char ** argv, Type type = GuiClient);

    bool isRunning () const;

public slots:
    bool sendMessage   (const QString     & message,   int timeout = 500);
    bool sendArguments (const QStringList & arguments, int timeout = 500);
    bool sendVariant   (const QVariant    & variant,   int timeout = 500);

signals:
    void messageReceived   (const QString     & message  );
    void argumentsReceived (const QStringList & arguments);
    void variantReceived   (const QVariant    & variant  );

private:
    void shareApplicationPid ();
    qint64 sharedPid ();

    const QString m_key;
    bool m_isRunning;
    QSharedMemory     * m_sharedMemory;
    ApplicationServer * m_server;
    ApplicationSocket * m_socket;

};

#endif // SINGLE_APPLICATION_H

