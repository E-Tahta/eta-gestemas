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

#include "twotouchpinchgesturerecognizer.h"
#include "touch.h"
#include "utilities.h"
#include <cmath>
#include <QtCore/QtDebug>

TwoTouchPinchGestureRecognizer::TwoTouchPinchGestureRecognizer()
     :GestureRecognizer(),
      m_scale(1.0f),
      m_touch1(nullptr),
      m_touch2(nullptr),
      m_distanceSquared(1.0f)
{
}

bool TwoTouchPinchGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    if (!GestureRecognizer::isEqual(other)) return false;

    return true;
}

void TwoTouchPinchGestureRecognizer::onTouchBegan(const Touch *touch)
{
    if (numTouches() > 2) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
        return;
    }

    if (numTouches() == 1) {
        m_touch1 = touch;
    } else { //numTouches() == 2
        m_touch2 = touch;
        float x1 = m_touch1->x();
        float y1 = m_touch1->y();
        float x2 = m_touch2->x();
        float y2 = m_touch2->y();
        m_distanceSquared =
            SQUARED_PYTHAGOREAN(y1, y2, x1, x2);
        // Just in case
        if (m_distanceSquared == 0.0f) {
            m_distanceSquared = 1.0f;
        }
    }
}

void TwoTouchPinchGestureRecognizer::onTouchMoved(const Touch *touch)
{
    float x1 = touch->startX();
    float y1 = touch->startY();
    float x2 = touch->x();
    float y2 = touch->y();
    if (numTouches() < 2) {
        if ((state() == State::Possible)
            && (SQUARED_PYTHAGOREAN(y1, y2, x1, x2) >=
                SQUARED(recognitionThreshold()))) {
            setState(State::Failed);
        }
        return;
    }

    float currentDistanceSquared = 0.0f;
    x1 = m_touch1->x();
    y1 = m_touch1->y();
    x2 = m_touch2->x();
    y2 = m_touch2->y();
    currentDistanceSquared = SQUARED_PYTHAGOREAN(y1, y2, x1, x2);

    m_scale = sqrtf(currentDistanceSquared / m_distanceSquared);
    m_distanceSquared = currentDistanceSquared;

    if (state() == State::Possible) {
        updateCentralPoint();
        setState(State::Began);
    } else if (state() == State::Began || state() == State::Changed) {
        updateCentralPoint();
        setState(State::Changed);
    }
}

void TwoTouchPinchGestureRecognizer::onTouchEnded(const Touch *touch)
{
    if (numTouches() == 0) {
        if (state() == State::Began || state() == State::Changed) {
            updateCentralPoint();
            setState(State::Ended);
        } else if (state() == State::Possible) {
            setState(State::Failed);
        }
    } else { // numTouches() == 1
        if (m_touch1 == touch) {
            m_touch1 = m_touch2;
        }
        m_touch2 = nullptr;
    }
}

void TwoTouchPinchGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    m_touch1 = m_touch2 = nullptr;
    m_scale = 1.0f;
    m_distanceSquared = 1.0f;
}
