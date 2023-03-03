#ifndef CLASSPROPERTYEDITOR_H
#define CLASSPROPERTYEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class classPropertyEditor;
}
QT_END_NAMESPACE

class classPropertyEditor : public QWidget {
  Q_OBJECT

 public:
  classPropertyEditor(QWidget *parent = nullptr);
  ~classPropertyEditor();

 private:
  Ui::classPropertyEditor *ui;
};
#endif  // CLASSPROPERTYEDITOR_H
