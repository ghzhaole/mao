//
// Created by leo on 2023/3/7.
//

#include "class_tree_item_const.h"

namespace mao {
namespace propertyEditor {
classTreeItemConst::classTreeItemConst(const qstring &name,
                                       void *propertyObject,
                                       QMetaType::Type type, QObject *parent)
    : classTreeItemCommon(name, propertyObject, type, parent) {}

bool classTreeItemConst::isReadOnly() { return true; }

}  // namespace propertyEditor
}  // namespace mao