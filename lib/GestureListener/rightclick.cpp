/* Copyright (C) 2015 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 *
 * This file is part of eta-gestemas.
 *
 * eta-gestemas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eta-gestemas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eta-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QtDebug>
#include "rightclick.h"
#include "longpressgesturerecognizer.h"

void RightClick::setGestureRecognizer(LongPressGestureRecognizer *recognizer)
{
    GestureListener::setGestureRecognizer(recognizer);
}

void RightClick::onBegan()
{
    qDebug() << "RighClick onBegan";
    const char *modifiers[] = {""};
    injectButton(3, modifiers);
}
void RightClick::onRecognized()
{
    qDebug() << "RighClick onRecognized";
}
void RightClick::onChanged()
{
    qDebug() << "RighClick onChanged";
}
void RightClick::onCanceled()
{
    qDebug() << "RighClick onCanceled";
}
void RightClick::onEnded()
{
    qDebug() << "RighClick onEnded";
}
void RightClick::onFailed()
{
    qDebug() << "RighClick onFailed";
}
