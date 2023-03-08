#include <iostream>

#include "class_tree_model.h"
#include "class_tree_widget.h"
#include "class_variant_delegate.h"

namespace mao::propertyEditor {
classTreeWdiget::classTreeWdiget(QWidget *parent) : QTreeView(parent) {
  model_ = new classTreeModel();
  setModel(model_);
  setItemDelegate(new classVariantDelegate(this));
}

void classTreeWdiget::addObject(shared_ptr<maoMetaObject> propertyObject) {
  model_->addItem(QString(), propertyObject);
  expandToDepth(0);
}
}  // namespace mao::propertyEditor