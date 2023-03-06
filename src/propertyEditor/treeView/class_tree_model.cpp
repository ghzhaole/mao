//
// Created by leo on 2023/3/2.
//

#include "class_tree_model.h"

#include <QApplication>
#include <QItemEditorFactory>
#include <QMetaProperty>

#include "class_tree_item.h"
#include "class_tree_item_common.h"

namespace mao::propertyEditor {
classTreeModel::classTreeModel(QObject *parent) : QAbstractItemModel(parent) {
  root_ = new classTreeItem("root", nullptr, this);
}

QModelIndex classTreeModel::index(int row, int column,
                                  const QModelIndex &parent) const {
  classTreeItem *parent_item = root_;
  if (parent.isValid()) {
    parent_item = static_cast<classTreeItem *>(parent.internalPointer());
  }
  if (row >= parent_item->children().size() || row < 0) {
    return QModelIndex();
  }
  return createIndex(row, column, parent_item->children().at(row));
}

QModelIndex classTreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) {
    return QModelIndex();
  }
  classTreeItem *child_item =
      static_cast<classTreeItem *>(index.internalPointer());
  classTreeItem *parent_item =
      qobject_cast<classTreeItem *>(child_item->parent());
  if (!parent_item || parent_item == root_) {
    return QModelIndex();
  }
  return createIndex(parent_item->row(), 0, parent_item);
}

int classTreeModel::rowCount(const QModelIndex &parent) const {
  classTreeItem *parent_item = root_;
  if (parent.isValid()) {
    parent_item = static_cast<classTreeItem *>(parent.internalPointer());
  }
  return parent_item->children().size();
}

int classTreeModel::columnCount(const QModelIndex &parent) const { return 2; }

QVariant classTreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  classTreeItem *item = static_cast<classTreeItem *>(index.internalPointer());
  switch (role) {
    case Qt::ToolTipRole:
    case Qt::DisplayRole:
    case Qt::EditRole:
      if (index.column() == 0) {
        return item->objectName();
      } else if (index.column() == 1) {
        return item->value(role);
      }
      break;
    case Qt::BackgroundRole:
      if (item->isRoot())
        return QApplication::palette("QTreeView")
            .brush(QPalette::Normal, QPalette::Button)
            .color();
      break;
  }
  return QVariant();
}

bool classTreeModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  if (index.isValid() && role == Qt::EditRole) {
    classTreeItem *item = static_cast<classTreeItem *>(index.internalPointer());
    item->setValue(value);
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

Qt::ItemFlags classTreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::ItemIsEnabled;
  classTreeItem *item = static_cast<classTreeItem *>(index.internalPointer());
  // only allow change of value attribute
  if (item->isRoot())
    return Qt::ItemIsEnabled;
  else if (item->isReadOnly())
    return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;
  else
    return Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable |
           Qt::ItemIsEditable;
}

QVariant classTreeModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return tr("Name");
      case 1:
        return tr("Value");
    }
  }
  return QVariant();
}

QModelIndex classTreeModel::buddy(const QModelIndex &index) const {
  if (index.isValid() && index.column() == 0)
    return createIndex(index.row(), 1, index.internalPointer());
  return index;
}

void classTreeModel::addItem(shared_ptr<maoMetaObject> propertyObject,
                             classTreeItem *root) {
  if (!propertyObject) {
    return;
  }
  size_t field_count = propertyObject->get_field_count();
  beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
  classTreeItem *propertyItem = new classTreeItem(
      QString::fromStdString(propertyObject->get_class_name()), 0,
      root ? root : root_);
  for (size_t field_idx = 0; field_idx < field_count; ++field_idx) {
    auto field = propertyObject->get_field(field_idx);
    auto field_type = field->type();
    bool is_array = field->is_list();
    if (!is_array) {
      if (field_type == "int" || field_type == "float" ||
          field_type == "double" || field_type == "bool" ||
          field_type == "string") {
        new classTreeItem(QString::fromStdString(field->name()), propertyObject,
                          propertyItem);
      } else {
        shared_ptr<maoMetaObject> val;
        field->get(propertyObject, val);
        if (val) {
          addItem(val, propertyItem);
        }
      }
    } else {
      auto sub_field_type = field->sub_type();
      if (sub_field_type == "int") {
        size_t array_size = field->size<int>(propertyObject);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          int *val = field->get<int>(propertyObject, array_idx);
          new classTreeItemCommon(QString::fromStdString(field->name()),
                                  (void *)val, QMetaType::Int, propertyItem);
        }
      } else if (sub_field_type == "float") {
        size_t array_size = field->size<float>(propertyObject);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          float *val = field->get<float>(propertyObject, array_idx);
          new classTreeItemCommon(QString::fromStdString(field->name()),
                                  (void *)val, QMetaType::Float, propertyItem);
        }
      } else if (sub_field_type == "double") {
        size_t array_size = field->size<double>(propertyObject);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          double *val = field->get<double>(propertyObject, array_idx);
          new classTreeItemCommon(QString::fromStdString(field->name()),
                                  (void *)val, QMetaType::Double, propertyItem);
        }
      } else if (sub_field_type == "bool") {
        size_t array_size = field->size<bool>(propertyObject);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          bool *val = field->get<bool>(propertyObject, array_idx);
          new classTreeItemCommon(QString::fromStdString(field->name()),
                                  (void *)val, QMetaType::Bool, propertyItem);
        }
      } else if (sub_field_type == "string") {
        size_t array_size = field->size<bool>(propertyObject);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          std::string *val = field->get<std::string>(propertyObject, array_idx);
          new classTreeItemCommon(QString::fromStdString(field->name()),
                                  (void *)val, QMetaType::QString,
                                  propertyItem);
        }
      } else {
        size_t array_size =
            field->size<std::shared_ptr<maoMetaObject>>(propertyObject);
        classTreeItem *row_item = new classTreeItem(
            QString::fromStdString(field->name()), 0, propertyItem);
        for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
          std::shared_ptr<maoMetaObject> subObj;
          field->get(propertyObject, array_idx, subObj);
          if (subObj) {
            addItem(subObj, row_item);
          }
        }
      }
    }
  }
  endInsertRows();
}

void classTreeModel::clear() {
  beginRemoveRows(QModelIndex(), 0, rowCount());
  delete root_;
  root_ = new classTreeItem("Root", 0, this);
  endRemoveRows();
}

}  // namespace mao::propertyEditor