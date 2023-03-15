#include "class_property_editor.h"
#include <qdebug.h>
#include <iostream>
#include "./ui_classpropertyeditor.h"
#include "reflection/class_factory.h"
#include "serialization/class_json_stream.h"
#include "utils/library/class_library_loader.h"

using namespace mao::library;
using namespace mao::reflection;

classPropertyEditor::classPropertyEditor(QWidget *parent) : QWidget(parent), ui(new Ui::classPropertyEditor) {
  ui->setupUi(this);
#ifdef WINDOWS
#ifdef _DEBUG
  std::string dllfilepath = R"(mao_data_d.dll)";
#else
  std::string dllfilepath = R"(mao_data.dll)";
#endif
#endif
#ifdef LINUX
#ifdef _DEBUG
  std::string dllfilepath = R"(mao_data_d.so)";
#else
  std::string dllfilepath = R"(mao_data.so)";
#endif
#endif
#ifdef DARWIN
#ifdef _DEBUG
  std::string dllfilepath = R"(../../../lib/libmao_data_d.dylib)";
#else
  std::string dllfilepath = R"(../../../lib/libmao_data.dylib)";
#endif
#endif
  std::cout << dllfilepath;
  auto ptr_shared_library_ = new classLibraryLoader();
  if (ptr_shared_library_->Load(dllfilepath) == false) {
    return;
  }
  obj_ = classFactory::instance()->create_class("child");
  auto p3 = classFactory::instance()->create_class("point");
  p3->set("x", 4.f);
  p3->set("y", 6.f);
  obj_->set("ps", 0, p3);
  auto p4 = classFactory::instance()->create_class("point");
  p4->set("x", 8.f);
  p4->set("y", 9.f);
  obj_->set("ps", 1, p4);
  obj_->set("mi", "first", 11);
  obj_->set("mi", "second", 12);
  ui->treeWidget->addObject(obj_);
}

classPropertyEditor::~classPropertyEditor() {
  mao::serialization::classJsonStream cjs;
  std::string s = cjs.to_json(obj_);
  std::cout << s;
  delete ui;
}
