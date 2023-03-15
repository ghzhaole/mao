#pragma once

#include "class_tree_item.h"

namespace mao::propertyEditor {
class classTreeItemCommon : public classTreeItem {
 Q_OBJECT
 public:
  classTreeItemCommon(const qstring &name = QString(),
                      void *propertyObject = nullptr,
                      QMetaType::Type type = QMetaType::Nullptr,
                      QObject *parent = 0);

  virtual qvariant value(int role = Qt::UserRole) const;

  virtual void setValue(const qvariant &value);

  qvariant property_Object() { return val_; }

  virtual bool isRoot() {
    if (!val_.isValid()) return true;
    return false;
  }

 private:
  qvariant val_;

  QMetaType::Type type_;
};

}  // namespace mao::propertyEditor