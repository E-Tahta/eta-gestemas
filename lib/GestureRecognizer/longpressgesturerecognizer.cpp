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

#include "longpressgesturerecognizer.h"
#include "touch.h"
#include "utilities.h"
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QTimer>

LongPressGestureRecognizer::LongPressGestureRecognizer()
     :GestureRecognizer(),
     m_numTouchesRequired(1),
     m_minPressDuration(900),
     m_numTouchesRequiredReached(false),
     m_timer(nullptr)
{
    m_timer = new QTimer(this);
    Q_CHECK_PTR(m_timer);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this,
            &LongPressGestureRecognizer::onTimeout);

}

bool LongPressGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    const LongPressGestureRecognizer *p =
        static_cast<const LongPressGestureRecognizer*>(&other);

    if (m_numTouchesRequired != p->m_numTouchesRequired) return false;
    if (m_minPressDuration != p->m_minPressDuration) return false;
    if (m_maxAllowableDrift != p->m_maxAllowableDrift) return false;
    if (m_allowSimultaneousRecognition !=
        p->m_allowSimultaneousRecognition) return false;

    return true;
}

void LongPressGestureRecognizer::onTouchBegan(const Touch *touch)
{
    // qDebug() << "LongPress onTouchBegan";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (numTouches() > numTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
    }
    if (numTouches() == numTouchesRequired()) {
        m_numTouchesRequiredReached = true;
        m_timer->start(minPressDuration());
    }

}

void LongPressGestureRecognizer::onTouchMoved(const Touch *prev,
                                              const Touch *current)
{
    // const Touch *touch = current;
    // qDebug() << "LongPress onTouchMoved";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    const Touch *t = current;
    float driftSquared = SQUARED_PYTHAGOREAN(t->y(), t->startY(),
                                            t->x(), t->startX());
    if (state() == State::Possible
        && maxAllowableDrift() > 0
        && driftSquared > SQUARED(maxAllowableDrift())) {
        setState(State::Failed);
    } else if (state() == State::Began || state() == State::Changed) {
        updateCentralPoint();
        setState(State::Changed);
    }
}

void LongPressGestureRecognizer::onTouchEnded(const Touch *prev,
                                              const Touch *current)
{
    // const Touch *touch = current;
    // qDebug() << "LongPress onTouchEnded";
    // qDebug() << "touchId:" << touch->touchId()
    //          << "x:" << touch->x()
    //          << "y:" << touch->y()
    //          << "resolutionX:" << touch->resolutionX()
    //          << "resolutionY:" << touch->resolutionY()
    //          << "timeStamp:" <<  touch->timeStamp();

    if (m_numTouchesRequiredReached) {
        if (state() == State::Began || state() == State::Changed) {
            updateCentralPoint();
            setState(State::Ended);
            return;
        }
    }
    setState(State::Failed);
}

void LongPressGestureRecognizer::reset()
{
    GestureRecognizer::reset();
    m_numTouchesRequiredReached = false;
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void LongPressGestureRecognizer::onTimeout()
{
    if (state() == State::Possible) {
        updateCentralPoint();
        setState(State::Began);
    }
}
