#include "class_tree_item_common.h"

namespace mao::propertyEditor {
classTreeItemCommon::classTreeItemCommon(const qstring &name,
                                         void *propertyObject,
                                         QMetaType::Type type,
                                         QObject *parent) : classTreeItem(name, nullptr, parent) {
  val_ = qvariant::fromValue(propertyObject);
  type_ = type;
}

qvariant classTreeItemCommon::value(int role) const {
  if (val_.isValid()) {
    switch (type_) {
      case QMetaType::Int:return *(int *)(val_.value<void *>());
        break;
      case QMetaType::LongLong:return *(int64_t *)(val_.value<void *>());
        break;
      case QMetaType::Double:return *(double *)(val_.value<void *>());
        break;
      case QMetaType::Float:return *(float *)(val_.value<void *>());
        break;
      case QMetaType::Bool:return *(bool *)(val_.value<void *>());
        break;
      case QMetaType::QString:return QString::fromStdString(*(std::string *)(val_.value<void *>()));
        break;
      case QMetaType::Char:return *(char *)(val_.value<void *>());
        break;
      default:break;
    }
  }
  return qvariant();
}

void classTreeItemCommon::setValue(const qvariant &value) {
  switch (type_) {
    case QMetaType::Int:*(int *)(val_.value<void *>()) = value.toInt();
      break;
    case QMetaType::LongLong:*(int64_t *)(val_.value<void *>()) = value.toInt();
      break;
    case QMetaType::Double:*(double *)(val_.value<void *>()) = value.toDouble();
      break;
    case QMetaType::Float:*(float *)(val_.value<void *>()) = value.toFloat();
      break;
    case QMetaType::Bool:*(bool *)(val_.value<void *>()) = value.toBool();
      break;
    case QMetaType::Char:*(char *)(val_.value<void *>()) = value.toChar().toLatin1();
      break;
    case QMetaType::QString:*(std::string *)(val_.value<void *>()) = value.toString().toStdString();
      break;
    default:break;
  }
}
}  // namespace mao::propertyEditor