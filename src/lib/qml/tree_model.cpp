/***********************************************************************
 *
 * Filename: tree_model.cpp
 *
 * Description: qml list model used by the outliner.
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

#include "tree_model.hpp"
#include "plugin.hpp"

// LEAF

Leaf::Leaf(const QList<QVariant> &data, Leaf *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

Leaf::~Leaf()
{
    qDeleteAll(m_childItems);
}

void Leaf::appendChild(Leaf *item)
{
    std::cout << "APPENDING CHILD\n";
    m_childItems.append(item);
}

Leaf *Leaf::child(int row)
{
    return m_childItems.value(row);
}

int Leaf::childCount() const
{
    return m_childItems.count();
}

int Leaf::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<Leaf*>(this));

    return 0;
}

int Leaf::columnCount() const
{
    return m_itemData.count();
}

QVariant Leaf::data(int column) const
{
    return m_itemData.value(column);
}

Leaf *Leaf::parentItem()
{
    return m_parentItem;
}

void Leaf::clear()
{
    std::for_each(m_childItems.begin(), m_childItems.end(), [](Leaf* leaf){
        delete leaf;
        leaf=0;
    });

    m_childItems.clear();
    std::cout << "childItems cleared and are now " << m_childItems.count() << std::endl;
 
}

bool Leaf::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

bool Leaf::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.removeAt(position);

    std::for_each (m_childItems.begin(), m_childItems.end(), [position,columns](Leaf* child){
        child->removeColumns(position, columns);
    });

    return true;
}

bool Leaf::removeNode(int uid)
{
    std::for_each(m_childItems.begin(), m_childItems.end(), [](Leaf* leaf){
            std::cout << "leaf data = "
            << leaf->data(0).toInt() << " "
            << leaf->data(1).toInt() << " "
            << leaf->data(2).toInt() << " "
            << leaf->data(3).toInt() << "\n"
            ;
            });
    return false;
}

// TREE MODEL
    TreeModel::TreeModel(QObject *parent)
: QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "name" << "visible" << "icon" << "uid" << "nid";

    rootItem = new Leaf(rootData);
    updateTree();
}

TreeModel::~TreeModel()
{
    std::cout << "delete tree model\n";
    delete rootItem;
    rootItem=0;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Leaf *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Leaf*>(parent.internalPointer());

    Leaf *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[VisibleRole] = "visible";
    roles[IconRole] = "icon";
    roles[UidRole] = "uid";
    roles[NidRole] = "nid";
    return roles;
}



QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Leaf *childItem = static_cast<Leaf*>(index.internalPointer());
    Leaf *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Leaf *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Leaf*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<Leaf*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();

}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Leaf *item = static_cast<Leaf*>(index.internalPointer());

    switch (role) {
        case NameRole:
            std::cout << "Name role enum value:" << NameRole << std::endl;
            return item->data(0);
        case VisibleRole:
            return item->data(1);
        case IconRole:
            return item->data(2);
        case UidRole:
            return item->data(3);
         case NidRole:
            return item->data(4);
        default:
            return QVariant();
    }
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

void TreeModel::updateTree(QModelIndex parent)
{
    //std::cout << "**** UPDATE OUTLINER TREE ****\n";
    Leaf* root;    
    if(parent.isValid())
        root=static_cast<Leaf*>(parent.internalPointer());
    else
        root=rootItem;

    emit layoutAboutToBeChanged();
    root->clear();
    loadChildren(0,root);
    emit layoutChanged();
}

void TreeModel::test()
{
    clearTree();
}

QModelIndex TreeModel::setCurrentNode(int uid)
{
    //std::cout << "SETTING CURRENT NODE TO:" << uid << std::endl;
    QModelIndex rindex = getNodeIndex(uid,index(0,0));
    if(!rindex.isValid())
        return QModelIndex();
    else
        return rindex;
}

QModelIndex TreeModel::getNodeIndex(int uid, QModelIndex ind)
{
    if(!ind.isValid())
        return QModelIndex();

    int childCount = static_cast<Leaf*>(ind.internalPointer())->childCount();
    int row = static_cast<Leaf*>(ind.internalPointer())->row();
    if(data(ind,260).toInt() == uid)
        return index(row,0,parent(ind));

    for(int i=0; i<childCount; i++){
        QModelIndex rindex = getNodeIndex(uid,index(i,0,ind));
        if(rindex.isValid())
            return rindex; 
    }

    return QModelIndex();    
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent) 
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    Leaf* parentItem = getLeaf(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

Leaf* TreeModel::getLeaf(const QModelIndex& index) const
{
    if (index.isValid()) {
        Leaf* item = static_cast<Leaf*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

void TreeModel::removeNode(int uid, const QModelIndex& parent)
{
    QList<Leaf*> children = getLeaf(parent)->childItems();

    std::for_each(children.begin(), children.end(), [this,&parent,uid](Leaf* leaf){
        if(uid == leaf->data(3).toInt()){
            removeRows(leaf->row(),1,parent);
        } else {
            removeNode(uid,index(leaf->row(),0,parent));
        }
    });
}

void TreeModel::clearTree()
{
    QModelIndex parent = index(0,0);
    QList<Leaf*> children = getLeaf(parent)->childItems();

    std::for_each(children.begin(), children.end(), [this,&parent](Leaf* leaf){
        removeRows(leaf->row(),1,parent);
    });
}

void TreeModel::loadChildren(const int uid, Leaf* parent)
{
    feather::status e;
    std::string name;
    QList<QVariant> data;
    // create data
    feather::qml::command::get_node_name(uid,name,e);
    data.append(name.c_str()); // name
    data.append(true); // visible
    int nid;
    std::string icon;
    nid = feather::qml::command::get_node_id(uid,e); 
    feather::qml::command::get_node_icon(nid,icon,e); 
    data.append(icon.c_str()); // icon 
    data.append(uid); // uid
    data.append(0); // nid

    parent->appendChild(new Leaf(data,parent)); 

    // recursive loop through each child node
    std::vector<unsigned int> children;
    int fid = 202; // we only want to get the nodes for child field
    feather::qml::command::get_node_connected_uids(uid,fid,children);
  
    std::cout << "UPDATING OUTLINER\nuid:" << uid << " is connected to the following nodes\n";
    for(auto cuid : children)
        std::cout << "\t" << cuid << std::endl;
 
    if(!children.size())
        return;

    for_each(children.begin(), children.end(), [this,&parent,&uid](int cuid){
        // only load the child if it's connected to the parents parent fid
        feather::field::FieldBase* sfield = feather::plugin::get_node_field_base(cuid,201);
        for(auto conn: sfield->connections){
            if(conn.puid == uid && conn.pfid == 202)
                loadChildren(cuid,parent->lastChild());
        }
    });
}

