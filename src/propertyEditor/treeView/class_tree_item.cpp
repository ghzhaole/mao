//
// Created by leo on 2023/3/2.
//

#include "class_tree_item.h"

#include <QLineEdit>
#include <QSpinBox>

#include "class_bool_combo.h"

namespace mao::propertyEditor {
classTreeItem::classTreeItem(const qstring &name,
                             shared_ptr<maoMetaObject> propertyObject,
                             QObject *parent)
    : QObject(parent), property_obj_(propertyObject),field_name_(name) {
  setObjectName(name);
}

classTreeItem::classTreeItem(const qstring &objectName,
							 const qstring &fieldName,
							 shared_ptr<maoMetaObject> propertyObject,
							 QObject *parent):field_name_(fieldName) {
  setObjectName(objectName);
}

qvariant classTreeItem::value(int role) const {
  if (property_obj_) {
    auto field = property_obj_->get_field(field_name_.toStdString());
    if (field) {
      auto field_type = field->type().type();
      switch (field_type) {
        case maoMetaType::TYPE_INT:
          return QVariant(*field->get<int>(property_obj_));
        case maoMetaType::TYPE_INT64:
          return QVariant(*field->get<int64_t>(property_obj_));
        case maoMetaType::TYPE_FLOAT:
          return QVariant(*field->get<float>(property_obj_));
        case maoMetaType::TYPE_DOUBLE:
          return QVariant(*field->get<double>(property_obj_));
        case maoMetaType::TYPE_BOOL:
          return QVariant(*field->get<bool>(property_obj_));
        case maoMetaType::TYPE_STRING:
          return QVariant(
              QString::fromStdString(*field->get<string>(property_obj_)));
        case maoMetaType::TYPE_CHAR:
          return QVariant(*field->get<char>(property_obj_));
      }
    }
  }
  return qvariant();
}

void classTreeItem::setValue(const qvariant &value) {
  if (property_obj_) {
    auto field = property_obj_->get_field(field_name_.toStdString());
    if (field) {
      auto field_type = field->type().type();
      switch (field_type) {
        case maoMetaType::TYPE_INT:
          field->set(property_obj_, value.toInt());
          break;
        case maoMetaType::TYPE_INT64:
          field->set(property_obj_, value.toLongLong());
          break;
        case maoMetaType::TYPE_FLOAT:
          field->set(property_obj_, value.toFloat());
          break;
        case maoMetaType::TYPE_DOUBLE:
          field->set(property_obj_, value.toDouble());
          break;
        case maoMetaType::TYPE_BOOL:
          field->set(property_obj_, value.toBool());
          break;
        case maoMetaType::TYPE_STRING:
          field->set<string>(property_obj_, value.toString().toStdString());
          break;
        case maoMetaType::TYPE_CHAR: {
          string s = value.toString().toStdString();
          char c = s.size() > 0 ? s[0] : '\0';
          field->set<char>(property_obj_, c);
          break;
        }
      }
    }
  }
}

QWidget *classTreeItem::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option) {
  switch (value().type()) {
    case qvariant::Int: {
      auto editor = new QSpinBox(parent);
      editor->setProperty("minimum", INT_MIN);
      editor->setProperty("maximum", INT_MAX);
      return editor;
      break;
    }
    case qvariant::LongLong: {
      auto editor = new QSpinBox(parent);
      editor->setProperty("minimum", INT_MIN);
      editor->setProperty("maximum", INT_MAX);
      return editor;
      break;
    }
    case qvariant::Double:
    case QMetaType::Float: {
      auto editor = new QDoubleSpinBox(parent);
      editor->setProperty("minimum", INT_MIN);
      editor->setProperty("maximum", INT_MAX);
      return editor;
      break;
    }
    case qvariant::Bool: {
      auto editor = new classBoolCombo(parent);
      return editor;
      break;
    }
    case qvariant::Char:
    case qvariant::String: {
      auto editor = new QLineEdit(parent);
      return editor;
      break;
    }
    default:
      break;
  }
  return nullptr;
}

bool classTreeItem::setEditorData(QWidget *editor, const QVariant &data) {
  switch (value().type()) {
    case qvariant::Int: {
      editor->blockSignals(true);
      static_cast<QSpinBox *>(editor)->setValue(data.toInt());
      editor->blockSignals(false);
      return true;
    }
    case qvariant::LongLong: {
      editor->blockSignals(true);
      static_cast<QSpinBox *>(editor)->setValue(data.toLongLong());
      editor->blockSignals(false);
      return true;
    }
    case qvariant::Double:
    case QMetaType::Float: {
      editor->blockSignals(true);
      static_cast<QDoubleSpinBox *>(editor)->setValue(data.toDouble());
      editor->blockSignals(false);
      return true;
    }
    case qvariant::Bool: {
      editor->blockSignals(true);
      static_cast<classBoolCombo *>(editor)->setValue(data.toBool());
      editor->blockSignals(false);
      return true;
    }
    case qvariant::String: {
      editor->blockSignals(true);
      static_cast<QLineEdit *>(editor)->setText(data.toString());
      editor->blockSignals(false);
      return true;
    }
    case qvariant::Char: {
      editor->blockSignals(true);
      static_cast<QLineEdit *>(editor)->setText(data.toChar());
      editor->blockSignals(false);
      return true;
    }
    default:
      break;
  }
  return false;
}

QVariant classTreeItem::editorData(QWidget *editor) {
  switch (value().type()) {
    case qvariant::Int:
    case qvariant::LongLong: {
      return QVariant(static_cast<QSpinBox *>(editor)->value());
    }
    case qvariant::Double:
    case QMetaType::Float: {
      return QVariant(static_cast<QDoubleSpinBox *>(editor)->value());
    }
    case qvariant::Bool: {
      return QVariant(static_cast<classBoolCombo *>(editor)->value());
    }
    case qvariant::Char:
    case qvariant::String: {
      return QVariant(static_cast<QLineEdit *>(editor)->text());
    }
    default:
      break;
  }
  return QVariant();
}

classTreeItem *classTreeItem::findPropertyObject(
    shared_ptr<maoMetaObject> propertyObject) {
  if (propertyObject) {
    return nullptr;
  }
  if (propertyObject == property_obj_) {
    return this;
  }
  auto child_list = children();
  for (int idx = 0; idx < child_list.size(); ++idx) {
    auto child = static_cast<classTreeItem *>(child_list[idx])
                     ->findPropertyObject(propertyObject);
    if (child) {
      return child;
    }
  }
  return nullptr;
}

bool classTreeItem::isReadOnly() {
  if (isRoot()) {
    return true;
  }
  return false;
}

}  // namespace mao::propertyEditor