#pragma once

#include <QTreeView>
#include <memory>

namespace mao::reflection {
class metaObject;
}

namespace mao::propertyEditor {
class classTreeModel;

using maoMetaObject = mao::reflection::metaObject;
using std::shared_ptr;

class classTreeWdiget : public QTreeView {
 Q_OBJECT
 public:
  classTreeWdiget(QWidget *parent = 0);

  void addObject(shared_ptr<maoMetaObject> propertyObject);

 private:
  classTreeModel *model_;
};
}  // namespace mao::propertyEditor