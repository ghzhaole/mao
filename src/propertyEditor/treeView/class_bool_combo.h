//
// Created by leo on 2023/3/2.
//

#ifndef MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_BOOL_COMBO_H_
#define MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_BOOL_COMBO_H_

#include <QComboBox>

namespace mao::propertyEditor {
class classBoolCombo : public QComboBox {
 Q_OBJECT
 public:
  classBoolCombo(QWidget *parent = 0);

  virtual ~classBoolCombo();

  bool value() const;

  void setValue(bool val);

 private slots:

  void currentChanged(int idx);

 signals:

  void valueChanged(bool val);

 private:
  bool value_;

  QStringList bool_list_;
};
}  // namespace mao::propertyEditor
#endif  // MAO_SRC_PROPERTYEDITOR_TREEVIEW_CLASS_BOOL_COMBO_H_
