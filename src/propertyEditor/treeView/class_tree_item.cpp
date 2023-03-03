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
    : QObject(parent), property_obj_(propertyObject) {
  setObjectName(name);
}

qvariant classTreeItem::value(int role) const {
  if (property_obj_) {
    auto field = property_obj_->get_field(objectName().toStdString());
    if (field) {
      auto field_type = field->type();
      if (field_type == "int") {
        int val = 0;
        field->get(property_obj_, val);
        return QVariant(val);
      } else if (field_type == "float") {
        float val = 0;
        field->get(property_obj_, val);
        return QVariant(val);
      } else if (field_type == "double") {
        double val = 0;
        field->get(property_obj_, val);
        return QVariant(val);
      } else if (field_type == "bool") {
        bool val = false;
        field->get(property_obj_, val);
        return QVariant(val);
      } else if (field_type == "string") {
        std::string val = "";
        field->get(property_obj_, val);
        return QVariant(QString::fromStdString(val));
      }
    }
  }
  return qvariant();
}

void classTreeItem::setValue(const qvariant &value) {
  if (property_obj_) {
    auto field = property_obj_->get_field(objectName().toStdString());
    if (field) {
      auto field_type = field->type();
      if (field_type == "int") {
        field->set(property_obj_, value.toInt());
      } else if (field_type == "float") {
        field->set(property_obj_, value.toFloat());
      } else if (field_type == "double") {
        field->set(property_obj_, value.toDouble());
      } else if (field_type == "bool") {
        field->set(property_obj_, value.toBool());
      } else if (field_type == "string") {
        field->set(property_obj_, value.toString().toStdString());
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
    default:
      break;
  }
  return false;
}

QVariant classTreeItem::editorData(QWidget *editor) {
  switch (value().type()) {
    case qvariant::Int: {
      return QVariant(static_cast<QSpinBox *>(editor)->value());
    }
    case qvariant::Double:
    case QMetaType::Float: {
      return QVariant(static_cast<QDoubleSpinBox *>(editor)->value());
    }
    case qvariant::Bool: {
      return QVariant(static_cast<classBoolCombo *>(editor)->value());
    }
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