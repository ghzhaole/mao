#include "class_variant_delegate.h"

#include <QAbstractItemView>
#include <QSignalMapper>

#include "class_tree_item_common.h"

namespace mao::propertyEditor {
classVariantDelegate::classVariantDelegate(QObject* parent)
    : QItemDelegate(parent) {
  m_finishedMapper = new QSignalMapper(this);
  connect(m_finishedMapper, SIGNAL(mapped(QWidget*)), this,
          SIGNAL(commitData(QWidget*)));
  connect(m_finishedMapper, SIGNAL(mapped(QWidget*)), this,
          SIGNAL(closeEditor(QWidget*)));
}

classVariantDelegate::~classVariantDelegate() {}
QWidget* classVariantDelegate::createEditor(QWidget* parent,
                                            const QStyleOptionViewItem& option,
                                            const QModelIndex& index) const {
  QWidget* editor = 0;
  classTreeItem* p = static_cast<classTreeItem*>(index.internalPointer());
  switch (p->value().type()) {
    case QVariant::Int:
    case QVariant::LongLong:
    case QMetaType::Float:
    case QVariant::Double:
    case QVariant::Bool:
    case QVariant::Char:
    case QVariant::String:
      editor = p->createEditor(parent, option);
      if (editor) {
        if (editor->metaObject()->indexOfSignal("editFinished()") != -1) {
          connect(editor, SIGNAL(editFinished()), m_finishedMapper,
                  SLOT(map()));
          m_finishedMapper->setMapping(editor, editor);
        }
        break;  // if no editor could be created take default case
      }
    default:
      editor = QItemDelegate::createEditor(parent, option, index);
  }
  parseEditorHints(editor, p->editorHints());
  return editor;
}
void classVariantDelegate::setEditorData(QWidget* editor,
                                         const QModelIndex& index) const {
  m_finishedMapper->blockSignals(true);
  QVariant data = index.model()->data(index, Qt::EditRole);

  switch (data.type()) {
    case QMetaType::Double:
    case QMetaType::Float:
    case QVariant::Bool:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Char:
    case QVariant::String:
      if (static_cast<classTreeItem*>(index.internalPointer())
              ->setEditorData(
                  editor,
                  data))  // if editor couldn't be recognized use default
        break;
    default:
      QItemDelegate::setEditorData(editor, index);
      break;
  }
  m_finishedMapper->blockSignals(false);
}
void classVariantDelegate::setModelData(QWidget* editor,
                                        QAbstractItemModel* model,
                                        const QModelIndex& index) const {
  QVariant data = index.model()->data(index, Qt::EditRole);
  switch (data.type()) {
    case QMetaType::Double:
    case QMetaType::Float:
    case QVariant::Bool:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Char:
    case QVariant::String: {
      QVariant data = static_cast<classTreeItem*>(index.internalPointer())
                          ->editorData(editor);
      if (data.isValid()) {
        model->setData(index, data, Qt::EditRole);
        break;
      }
    }
    default:
      QItemDelegate::setModelData(editor, model, index);
      break;
  }
}
void classVariantDelegate::updateEditorGeometry(
    QWidget* editor, const QStyleOptionViewItem& option,
    const QModelIndex& index) const {
  return QItemDelegate::updateEditorGeometry(editor, option, index);
}
void classVariantDelegate::parseEditorHints(QWidget* editor,
                                            const QString& editorHints) const {
  if (editor && !editorHints.isEmpty()) {
    editor->blockSignals(true);
    // Parse for property values
    QRegExp rx("(.*)(=\\s*)(.*)(;{1})");
    rx.setMinimal(true);
    int pos = 0;
    while ((pos = rx.indexIn(editorHints, pos)) != -1) {
      // qDebug("Setting %s to %s", qPrintable(rx.cap(1)),
      // qPrintable(rx.cap(3)));
      editor->setProperty(qPrintable(rx.cap(1).trimmed()), rx.cap(3).trimmed());
      pos += rx.matchedLength();
    }
    editor->blockSignals(false);
  }
}
}  // namespace mao::propertyEditor