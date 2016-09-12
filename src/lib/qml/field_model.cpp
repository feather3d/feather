/***********************************************************************
 *
 * Filename: field_model.cpp
 *
 * Description: qml list model for node fields.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
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

#include "field_model.hpp"

std::vector<FieldName*> FieldModel::m_fieldnames;

FieldModel::FieldModel(QObject* parent) : QAbstractListModel(parent)
{
}

FieldModel::~FieldModel()
{
    for(auto f : m_fields)
        delete f;
    m_fields.clear();
}                                                                        

QHash<int, QByteArray> FieldModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(UidRole, QByteArray("uid"));
    roles.insert(NidRole, QByteArray("nid"));
    roles.insert(FidRole, QByteArray("fid"));
    roles.insert(TypeRole, QByteArray("type"));
    roles.insert(LockedRole, QByteArray("locked"));
    roles.insert(ConnectedRole, QByteArray("connected"));
    roles.insert(KeyedRole, QByteArray("keyed"));
    return roles;
}

int FieldModel::rowCount(const QModelIndex& parent) const
{
    return m_fields.size();
}

QVariant FieldModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant(); // Return Null variant if index is invalid
    if (index.row() > (m_fields.size()-1) )
        return QVariant();
    FieldInfo *dobj = m_fields.at(index.row());
    switch (role) {
        case Qt::DisplayRole: // The default display role now displays the first name as well
        case NameRole:
            return QVariant::fromValue(dobj->name);
        case UidRole:
            return QVariant::fromValue(dobj->uid);
        case NidRole:
            return QVariant::fromValue(dobj->nid);
        case FidRole:
            return QVariant::fromValue(dobj->fid);
        case TypeRole:
            return QVariant::fromValue(dobj->type);
        case LockedRole:
            return QVariant::fromValue(dobj->locked);
        case ConnectedRole:
            return QVariant::fromValue(dobj->connected);
        case KeyedRole:
            return QVariant::fromValue(dobj->keyed);
        default:
            return QVariant();
    }
}

void FieldModel::clear()
{
    m_fields.clear();
}

void FieldModel::addField(int uid, int nid, int fid, int type, bool locked, bool connected, bool keyed)
{
    m_fields.append(new FieldInfo(getFieldName(uid,nid),uid,nid,fid,type,locked,connected,keyed));
}

void FieldModel::addFields(int uid, int nid, int conn)
{
    std::vector<feather::field::FieldBase*> fids;
    //std::cout << "addFields " << nid << std::endl;
    //feather::qml::command::get_fid_list(uid,nid,feather::field::connection::In,fids);
    feather::qml::command::get_fid_list(uid,nid,static_cast<feather::field::connection::Type>(conn),fids);
    m_fields.clear();
    for(uint i=0; i < fids.size(); i++) {
        //std::cout << "adding field - uid:" << uid << " nid:" << nid << " fid:" << fids.at(i)->id << " type:" << fids.at(i)->type << std::endl;
        // we only want to add fields that can be edited from the field editor
        // TODO - add a function to add the keyed value, this will be determined by looking to see
        // if the connected field is a KeyTrack
        m_fields.append(new FieldInfo(getFieldName(nid,fids.at(i)->id),uid,nid,fids.at(i)->id,fids.at(i)->type,fids.at(i)->locked,fids.at(i)->connected(),0));
    }
    emit layoutChanged();
}

void FieldModel::addFieldName(QString name, int nid, int fid)
{
    //std::cout << "adding field name\n";
    m_fieldnames.push_back(new FieldName{name,nid,fid});
}

QString FieldModel::getFieldName(int nid, int fid){
    //std::cout << "getFieldName\n";
    for(auto fn : m_fieldnames){
        //std::cout << "looking for " << nid << " " << fid << std::endl;
        //std::cout << "\tin: " << fn->nid << " " << fn->fid << " " << fn->name.toStdString().c_str() << std::endl;
        if((fn->nid==nid) && (fn->fid==fid) || (fn->nid==0) && (fn->fid==fid))
            return fn->name;
    }
    return "ERROR";
}
