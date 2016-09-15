/***********************************************************************
 *
 * Filename: qml_status.hpp
 *
 * Description: qml status wrapper.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef QML_STATUS_HPP
#define QML_STATUS_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "status.hpp"

using namespace feather;

// STATUS
class Status : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged)
 
    public:
        Status(QObject* parent=0);
        ~Status();

        // state
        int state() { return m_state; }

        void setState(int& s) {
            if(m_state!= s) {
                m_state=s;
                stateChanged();
            }
        }

        // message
        QString msg() { return m_msg; }

        void setMsg(QString& m) {
            if(m_msg!= m) {
                m_msg=m;
                msgChanged();
            }
        }

        enum State {
            Failed = FAILED,
            Passed = PASSED,
            Warning = WARNING 
        };

    signals:
        void stateChanged();
        void msgChanged();

    private:
        int m_state;
        QString m_msg;
};

#endif
