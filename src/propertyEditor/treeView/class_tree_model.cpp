//
// Created by leo on 2023/3/2.
//

#include "class_tree_model.h"
#include <QApplication>
#include <QItemEditorFactory>
#include <QMetaProperty>
#include "class_tree_item.h"
#include "class_tree_item_common.h"
#include "class_tree_item_const.h"

namespace mao::propertyEditor {
classTreeModel::classTreeModel(QObject *parent) : QAbstractItemModel(parent) {
  root_ = new classTreeItem("root", nullptr, this);
}

QModelIndex classTreeModel::index(int row, int column, const QModelIndex &parent) const {
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
  classTreeItem *child_item = static_cast<classTreeItem *>(index.internalPointer());
  classTreeItem *parent_item = qobject_cast<classTreeItem *>(child_item->parent());
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
        return QApplication::palette("QTreeView").brush(QPalette::Normal, QPalette::Window).color();
      break;
  }
  return QVariant();
}

bool classTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
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
    return Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant classTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:return tr("Name");
      case 1:return tr("Value");
    }
  }
  return QVariant();
}

QModelIndex classTreeModel::buddy(const QModelIndex &index) const {
  if (index.isValid() && index.column() == 0)
    return createIndex(index.row(), 1, index.internalPointer());
  return index;
}

void classTreeModel::addItem(const QString &name, shared_ptr<maoMetaObject> propertyObject, classTreeItem *root) {
  if (!propertyObject) {
    return;
  }
  size_t field_count = propertyObject->get_field_count();
  beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
  classTreeItem *propertyItem =
      new classTreeItem(name.isEmpty() ? QString::fromStdString(propertyObject->get_class_name()) : name,
                        0,
                        root ? root : root_);
  for (size_t field_idx = 0; field_idx < field_count; ++field_idx) {
    auto field = propertyObject->get_field(field_idx);
    auto field_type = field->type().type();
    switch (field_type) {
      case maoMetaType::TYPE_INT:
      case maoMetaType::TYPE_INT64:
      case maoMetaType::TYPE_FLOAT:
      case maoMetaType::TYPE_DOUBLE:
      case maoMetaType::TYPE_BOOL:
      case maoMetaType::TYPE_STRING:
      case maoMetaType::TYPE_CHAR: new classTreeItem(QString::fromStdString(field->name()),
                                                     propertyObject,
                                                     propertyItem);
        break;
      case maoMetaType::TYPE_OBJECT: {
        auto val = *field->get<shared_ptr<maoMetaObject>>(propertyObject);
        if (!val) {
          val = reflection::classFactory::instance()->create_class(field->type().object_type_name());
        }
        if (val) {
          addItem(QString(), val, propertyItem);
        }
        break;
      }
      case maoMetaType::TYPE_LIST: {
        classTreeItem *row_item = new classTreeItem(QString::fromStdString(field->name()), 0, propertyItem);
        auto field_sub_type = field->sub_type().type();
        switch (field_sub_type) {
          case maoMetaType::TYPE_INT: {
            size_t array_size = field->size<int>(propertyObject);
            field->set_cap<int>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              int *val = field->get<int>(propertyObject, array_idx);
              new classTreeItemCommon(QString("[%1]").arg(array_idx), (void *)val, QMetaType::Int, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_INT64: {
            size_t array_size = field->size<int64_t>(propertyObject);
            field->set_cap<int64_t>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              int64_t *val = field->get<int64_t>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()),
                                      (void *)val,
                                      QMetaType::LongLong,
                                      row_item);
            }
            break;
          }
          case maoMetaType::TYPE_FLOAT: {
            size_t array_size = field->size<float>(propertyObject);
            field->set_cap<float>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              float *val = field->get<float>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()), (void *)val, QMetaType::Float, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_DOUBLE: {
            size_t array_size = field->size<double>(propertyObject);
            field->set_cap<double>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              double *val = field->get<double>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()), (void *)val, QMetaType::Double, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_BOOL: {
            size_t array_size = field->size<bool>(propertyObject);
            field->set_cap<bool>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              bool *val = field->get<bool>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()), (void *)val, QMetaType::Bool, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_STRING: {
            size_t array_size = field->size<string>(propertyObject);
            field->set_cap<string>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              string *val = field->get<string>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()), (void *)val, QMetaType::QString, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_CHAR: {
            size_t array_size = field->size<char>(propertyObject);
            field->set_cap<char>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              char *val = field->get<char>(propertyObject, array_idx);
              new classTreeItemCommon(QString::fromStdString(field->name()), (void *)val, QMetaType::Char, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_OBJECT: {
            size_t array_size = field->size<std::shared_ptr<maoMetaObject>>(propertyObject);
            field->set_cap<std::shared_ptr<maoMetaObject>>(propertyObject, 10240);
            for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
              std::shared_ptr<maoMetaObject>
                  subObj = *field->get<std::shared_ptr<maoMetaObject>>(propertyObject, array_idx);
              if (!subObj) {
                subObj = reflection::classFactory::instance()->create_class(field->sub_type().object_type_name());
              }
              if (subObj) {
                addItem(QString("[%1]").arg(array_idx), subObj, row_item);
              }
            }
            break;
          }
        }
        break;
      }
      case maoMetaType::TYPE_MAP: {
        classTreeItem *row_item = new classTreeItem(QString::fromStdString(field->name()), 0, propertyItem);
        auto field_sub_type = field->sub_type().type();
        switch (field_sub_type) {
          case maoMetaType::TYPE_INT: {
            auto map_val = field->get < map < string,
            int >> (propertyObject);
            for (auto item : *map_val) {
              int *val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first), (void *)val_val, QMetaType::Int, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_INT64: {
            auto map_val = field->get < map < string, int64_t>>(propertyObject);
            for (auto item : *map_val) {
              int64_t * val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first),
                                      (void *)val_val,
                                      QMetaType::LongLong,
                                      row_item);
            }
            break;
          }
          case maoMetaType::TYPE_FLOAT: {
            auto map_val = field->get < map < string,
            float >> (propertyObject);
            for (auto item : *map_val) {
              float *val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first), (void *)val_val, QMetaType::Float, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_DOUBLE: {
            auto map_val = field->get < map < string,
            double >> (propertyObject);
            for (auto item : *map_val) {
              double *val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first), (void *)val_val, QMetaType::Double, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_BOOL: {
            auto map_val = field->get < map < string,
            bool >> (propertyObject);
            for (auto item : *map_val) {
              bool *val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first), (void *)val_val, QMetaType::Bool, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_CHAR: {
            auto map_val = field->get < map < string,
            char >> (propertyObject);
            for (auto item : *map_val) {
              char *val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first), (void *)val_val, QMetaType::Char, row_item);
            }
            break;
          }
          case maoMetaType::TYPE_STRING: {
            auto map_val = field->get < map < string, string>>(propertyObject);
            for (auto item : *map_val) {
              string * val_val = &((*map_val)[item.first]);
              new classTreeItemCommon(QString::fromStdString(item.first),
                                      (void *)val_val,
                                      QMetaType::QString,
                                      row_item);
            }
            break;
          }
          case maoMetaType::TYPE_OBJECT: {
            auto map_val = field->get < map < string, std::shared_ptr<maoMetaObject>>>(propertyObject);
            for (auto item : *map_val) {
              std::shared_ptr<maoMetaObject> val_val = (*map_val)[item.first];
              if (!val_val) {
                val_val = reflection::classFactory::instance()->create_class(field->sub_type().object_type_name());
              }
              if (val_val) {
                addItem(QString("%1 [%2]").arg(QString::fromStdString(item.first),
                                               QString::fromStdString(field->sub_type().object_type_name())),
                        val_val,
                        row_item);
              }
            }
            break;
          }
        }
        break;
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