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

#ifndef SINGLE_CORE_APPLICATION_H
#define SINGLE_CORE_APPLICATION_H

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>


class SingleApplicationImpl;

class QString;
class QStringList;
class QVariant;

class SingleCoreApplication : public QCoreApplication
{
    Q_OBJECT

public:
    /**
     * Sets the version number of the data serialization format.
     * This function should be called before the constructor.
     */
    static void setDataStreamVersion (QDataStream::Version v);

    SingleCoreApplication (const QString & key, int & argc, char ** argv);
    virtual ~SingleCoreApplication ();

    bool isRunning () const;

public slots:
    bool sendMessage   (const QString & message, int timeout = 500);
    bool sendArguments (int timeout = 500);
    bool sendObject    (const QVariant & object, int timeout = 500);

signals:
    void messageReceived   (const QString     & message  );
    void argumentsReceived (const QStringList & arguments);
    void objectReceived    (const QVariant    & object   );

private:
    friend class SingleAppicationImpl;
    SingleApplicationImpl * const d;

};

#endif // SINGLE_CORE_APPLICATION_H
