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

#include "application_server.h"

#include "application_socket.h"

#include <QtCore/QVariant>


ApplicationServer::ApplicationServer (QObject * parent /* = 0 */)
    : QLocalServer (parent)
{
}

void ApplicationServer::incomingConnection (quintptr socketDescriptor)
{
    ApplicationSocket * socket = new ApplicationSocket (this);
    connect (socket, SIGNAL(disconnected ()), socket, SLOT(deleteLater ()));

    connect (socket, SIGNAL(objectReceived (const QVariant &)),
             this,   SIGNAL(objectReceived (const QVariant &)));

    socket->setSocketDescriptor (socketDescriptor);
}
