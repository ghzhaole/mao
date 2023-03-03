#include "class_property_editor.h"

#include <qdebug.h>

#include <iostream>

#include "./ui_classpropertyeditor.h"
#include "reflection/class_factory.h"
#include "serialization/class_json_stream.h"
#include "utils/library/class_library_loader.h"

using namespace mao::library;
using namespace mao::reflection;

classPropertyEditor::classPropertyEditor(QWidget *parent)
    : QWidget(parent), ui(new Ui::classPropertyEditor) {
  ui->setupUi(this);

  std::string dllfilepath =
      "/Users/leo/git/mao/bin/Debug/lib/libmao_data_d.dylib";
  auto ptr_shared_library_ = new classLibraryLoader();
  if (ptr_shared_library_->Load(dllfilepath) == false) {
    return;
  }
  auto obj = classFactory::instance()->create_class("child");
  auto p3 = classFactory::instance()->create_class("point");
  p3->set("x", 4.f);
  p3->set("y", 6.f);
  obj->set("points", 0, p3);
  auto p4 = classFactory::instance()->create_class("point");
  p4->set("x", 8.f);
  p4->set("y", 9.f);
  obj->set("points", 1, p4);

  ui->treeWidget->addObject(obj);
}

classPropertyEditor::~classPropertyEditor() { delete ui; }
