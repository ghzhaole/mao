//
// Created by leo on 2023/3/2.
//

#include "class_bool_combo.h"

namespace mao::propertyEditor {
classBoolCombo::classBoolCombo(QWidget *parent)
    : QComboBox(parent), bool_list_({"true", "false"}) {
  addItems(bool_list_);
  connect(this, SIGNAL(currentIndexChanged(int)), this,
          SLOT(currentChanged(int)));
}

classBoolCombo::~classBoolCombo() {}

bool classBoolCombo::value() const { return value_; }

void classBoolCombo::setValue(bool val) {
  value_ = val;
  setCurrentIndex(val ? 0 : 1);
  emit valueChanged(val);
}

void classBoolCombo::currentChanged(int idx) {
  if (idx < bool_list_.size()) {
    if (bool_list_[idx] == "true") {
      setValue(true);
    } else {
      setValue(false);
    }
    setCurrentIndex(idx);
  }
}
}  // namespace mao::propertyEditor