//
// Created by leo on 2023/3/2.
//

#ifndef MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_MODEL_H_
#define MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_MODEL_H_

#include <QAbstractItemModel>
#include <map>
#include <memory>

namespace mao::reflection {
class metaObject;
}

namespace mao::propertyEditor {

using maoMetaObject = mao::reflection::metaObject;
using std::map;
using std::shared_ptr;

class classTreeItem;

class classTreeModel : public QAbstractItemModel {
  Q_OBJECT
 public:
  explicit classTreeModel(QObject *parent = 0);

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;

  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QModelIndex buddy(const QModelIndex &index) const override;

  // template <typename T>
  // void addItem(T& val) {

  //}

  void addItem(shared_ptr<maoMetaObject> propertyObject,
               classTreeItem *root = nullptr);

  void clear();

 private:
  classTreeItem *root_;
};
}  // namespace mao::propertyEditor
#endif  // MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_MODEL_H_
