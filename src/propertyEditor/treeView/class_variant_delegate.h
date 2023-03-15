#pragma once

#include <QItemDelegate>

class QSignalMapper;

namespace mao::propertyEditor {
class classVariantDelegate : public QItemDelegate {
 Q_OBJECT
 public:
  classVariantDelegate(QObject *parent = 0);

  ~classVariantDelegate();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

  virtual void updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const override;

 private:
  void parseEditorHints(QWidget *editor, const QString &editorHints) const;

  QSignalMapper *m_finishedMapper;
};

}  // namespace mao::propertyEditor