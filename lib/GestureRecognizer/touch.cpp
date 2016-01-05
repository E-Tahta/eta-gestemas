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

#include "touch.h"
#include <QtCore/QtGlobal>

Touch::Touch()
    :m_touchId(0UL),m_targetId(0UL),
     m_startX(0.0f), m_startY(0.0f),
     m_x(0.0f),m_y(0.0f),
     m_resolutionX(0.0f),m_resolutionY(0.0f),
     m_device(nullptr),
     m_startTime(0ULL),m_timeStamp(0ULL),m_ownershipState(Deferred)
{
}

Touch::Touch(uint32_t touchId, uint32_t targetId,
             float startX, float startY,
             float x, float y,
             float resolutionX, float resolutionY,
             void* device, uint64_t startTime, uint64_t timeStamp)
    :m_touchId(touchId),m_targetId(targetId),
     m_startX(startX), m_startY(startY),
     m_x(x),m_y(y),
     m_resolutionX(resolutionX),m_resolutionY(resolutionY),
     m_device(device),
     m_startTime(startTime),m_timeStamp(timeStamp),m_ownershipState(Deferred)
{
}

void Touch::setOwnershipState(OwnershipState ownershipState)
{
    Q_ASSERT(m_ownershipState == Deferred);
    m_ownershipState = ownershipState;
}