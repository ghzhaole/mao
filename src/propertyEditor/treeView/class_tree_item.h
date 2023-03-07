//
// Created by leo on 2023/3/2.
//

#ifndef MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_H_
#define MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_H_

#include <QStyleOption>
#include <QVariant>
#include <QWidget>
#include <memory>
#include <string>

#include "reflection/class_factory.h"

namespace mao::propertyEditor {
using maoMetaObject = mao::reflection::metaObject;
using maoField = mao::reflection::classField;
using maoMetaType = mao::reflection::metaTypes;
using std::shared_ptr;
using qstring = QString;
using std::string;
using qvariant = QVariant;

class classTreeItem : public QObject {
  Q_OBJECT
 public:
  classTreeItem(const qstring &name = QString(),
                shared_ptr<maoMetaObject> propertyObject = nullptr,
                QObject *parent = 0);

  virtual qvariant value(int role = Qt::UserRole) const;

  virtual void setValue(const qvariant &value);

  shared_ptr<maoMetaObject> property_Object() { return property_obj_; }

  virtual bool isRoot() {
    if (!property_obj_) {
      return true;
    }
    return false;
  }

  virtual bool isReadOnly();

  int row() { return parent()->children().indexOf(this); }

  QString editorHints() { return hints_; }

  virtual void setEditorHints(const QString &hints) { hints_ = hints; }

  virtual QWidget *createEditor(QWidget *parent,
                                const QStyleOptionViewItem &option);

  virtual bool setEditorData(QWidget *editor, const QVariant &data);

  virtual QVariant editorData(QWidget *editor);

  classTreeItem *findPropertyObject(shared_ptr<maoMetaObject> propertyObject);

 private:
  shared_ptr<maoMetaObject> property_obj_;

  qstring hints_;
};
}  // namespace mao::propertyEditor
#endif  // MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_TREE_ITEM_H_
