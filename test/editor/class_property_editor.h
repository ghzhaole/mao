#ifndef CLASSPROPERTYEDITOR_H
#define CLASSPROPERTYEDITOR_H

#include <QWidget>

#include "reflection/class_factory.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class classPropertyEditor;
}
QT_END_NAMESPACE
class classPropertyEditor : public QWidget {
  Q_OBJECT
 public:
  classPropertyEditor(QWidget* parent = nullptr);

  ~classPropertyEditor();

 public slots:
  void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&);

 private:
  void write_to_text_edit();

 private:
  Ui::classPropertyEditor* ui;

  std::shared_ptr<mao::reflection::metaObject> obj_;
};

#endif  // CLASSPROPERTYEDITOR_H
