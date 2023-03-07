//
// Created by leo on 2023/3/7.
//

#ifndef MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_CONST_H_
#define MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_CONST_H_

#include "class_tree_item_common.h"

namespace mao::propertyEditor {
class classTreeItemConst : public classTreeItemCommon {
  Q_OBJECT
 public:
  classTreeItemConst(const qstring &name = QString(),
                     void *propertyObject = nullptr,
                     QMetaType::Type type = QMetaType::Nullptr,
                     QObject *parent = 0);

  bool isReadOnly() override;
};

}  // namespace mao::propertyEditor
#endif  // MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_CONST_H_
