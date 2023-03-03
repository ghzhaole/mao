//
// Created by leo on 2023/3/1.
//

#include "class_json_stream.h"

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>      // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h>  // for stringify JSON

using namespace rapidjson;
namespace mao::serialization {
void write(const std::shared_ptr<metaObject> &objPtr,
           PrettyWriter<StringBuffer> &writer, classField *field);

void write(const std::shared_ptr<metaObject> &objPtr,
           PrettyWriter<StringBuffer> &writer);

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj,
          classField *field);

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj);

std::string classJsonStream::to_json(const shared_ptr<metaObject> &objPtr) {
  if (!objPtr) {
    return std::string();
  }
  StringBuffer sb;
  PrettyWriter<StringBuffer> writer(sb);
  write(objPtr, writer);
  return sb.GetString();
}

std::shared_ptr<metaObject> classJsonStream::from_json(const string &jsonStr) {
  Document document;
  if (document.Parse(jsonStr.c_str()).HasParseError()) return nullptr;
  std::shared_ptr<metaObject> objPtr;
  read(objPtr, document);
  return objPtr;
}

void write(const std::shared_ptr<metaObject> &objPtr,
           PrettyWriter<StringBuffer> &writer, classField *field) {
  bool field_is_array = field->is_array();
  if (!field_is_array) {
    auto field_type = field->type();
    if (field_type == "int") {
      int val = 0;
      field->get(objPtr, val);
      writer.Int(val);
    } else if (field_type == "int32_t") {
      int32_t val = 0;
      field->get(objPtr, val);
      writer.Int(val);
    } else if (field_type == "int64_t") {
      int64_t val = 0;
      field->get(objPtr, val);
      writer.Int64(val);
    } else if (field_type == "float") {
      float val = 0;
      field->get(objPtr, val);
      writer.Double(val);
    } else if (field_type == "double") {
      double val = 0;
      field->get(objPtr, val);
      writer.Double(val);
    } else if (field_type == "bool") {
      bool val = false;
      field->get(objPtr, val);
      writer.Bool(val);
    } else {
      std::shared_ptr<metaObject> subObj;
      field->get(objPtr, subObj);
      write(subObj, writer);
    }
  } else {
    writer.StartArray();
    auto sub_field_type = field->sub_type();
    if (sub_field_type == "int") {
      size_t array_size = field->size<int>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        int val = 0;
        field->get(objPtr, array_idx, val);
        writer.Int(val);
      }
    } else if (sub_field_type == "int32_t") {
      size_t array_size = field->size<int32_t>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        int32_t val = 0;
        field->get(objPtr, array_idx, val);
        writer.Int(val);
      }
    } else if (sub_field_type == "int64_t") {
      size_t array_size = field->size<int64_t>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        int64_t val = 0;
        field->get(objPtr, array_idx, val);
        writer.Int64(val);
      }
    } else if (sub_field_type == "float") {
      size_t array_size = field->size<float>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        float val = 0;
        field->get(objPtr, array_idx, val);
        writer.Double(val);
      }
    } else if (sub_field_type == "double") {
      size_t array_size = field->size<double>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        double val = 0;
        field->get(objPtr, array_idx, val);
        writer.Double(val);
      }
    } else if (sub_field_type == "bool") {
      size_t array_size = field->size<bool>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        bool val = 0;
        field->get(objPtr, array_idx, val);
        writer.Bool(val);
      }
    } else {
      size_t array_size = field->size<std::shared_ptr<metaObject>>(objPtr);
      for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
        std::shared_ptr<metaObject> subObj;
        field->get(objPtr, array_idx, subObj);
        write(subObj, writer);
      }
    }
    writer.EndArray();
  }
}

void write(const std::shared_ptr<metaObject> &objPtr,
           PrettyWriter<StringBuffer> &writer) {
  writer.StartObject();
  if (!objPtr) {
    writer.EndObject();
    return;
  }
  std::string class_name_tag = "class_name";
  auto class_name = objPtr->get_class_name();
  writer.String(class_name_tag);
  writer.String(class_name);
  std::string data_tag = "data";
  writer.String(data_tag);
  writer.StartObject();
  size_t field_count = objPtr->get_field_count();
  for (size_t field_idx = 0; field_idx < field_count; ++field_idx) {
    auto field = objPtr->get_field(field_idx);
    auto field_name = field->name();
    writer.String(field_name);
    write(objPtr, writer, field);
  }
  writer.EndObject();
  writer.EndObject();
}

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj,
          classField *field) {
  auto ret = jsonObj.FindMember(field->name().data());
  if (ret == jsonObj.MemberEnd()) {
    return;
  }
  bool field_is_array = field->is_array();
  if (!field_is_array) {
    auto field_type = field->type();
    if (field_type == "int") {
      field->set(objPtr, ret->value.GetInt());
    } else if (field_type == "int32_t") {
      field->set(objPtr, ret->value.GetInt());
    } else if (field_type == "int64_t") {
      field->set(objPtr, ret->value.GetInt64());
    } else if (field_type == "float") {
      field->set(objPtr, ret->value.GetFloat());
    } else if (field_type == "double") {
      field->set(objPtr, ret->value.GetDouble());
    } else if (field_type == "bool") {
      field->set(objPtr, ret->value.GetBool());
    } else {
      std::shared_ptr<metaObject> subObj;
      read(subObj, ret->value);
      field->set(objPtr, subObj);
    }
  } else {
    auto &ret_value = ret->value;
    auto sub_field_type = field->sub_type();
    if (sub_field_type == "int") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetInt());
      }
    } else if (sub_field_type == "int32_t") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetInt());
      }
    } else if (sub_field_type == "int64_t") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetInt64());
      }
    } else if (sub_field_type == "float") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetFloat());
      }
    } else if (sub_field_type == "double") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetDouble());
      }
    } else if (sub_field_type == "bool") {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        field->set(objPtr, i, json_index_value.GetBool());
      }
    } else {
      for (SizeType i = 0; i < ret_value.Size(); ++i) {
        auto &json_index_value = ret_value[i];
        std::shared_ptr<metaObject> subObj;
        read(subObj, json_index_value);
        field->set(objPtr, i, subObj);
      }
    }
  }
}

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj) {
  std::string class_name_tag = "class_name";
  std::string data_tag = "data";
  auto class_name_ret = jsonObj.FindMember(class_name_tag.data());
  if (class_name_ret == jsonObj.MemberEnd()) {
    return;
  }
  std::string className = class_name_ret->value.GetString();
  objPtr = classFactory::instance()->create_class(className);
  if (!objPtr) {
    return;
  }
  auto data_ret = jsonObj.FindMember(data_tag.data());
  if (data_ret == jsonObj.MemberEnd()) {
    return;
  }
  auto &data_value = data_ret->value;

  size_t field_count = objPtr->get_field_count();
  for (size_t field_idx = 0; field_idx < field_count; ++field_idx) {
    auto field = objPtr->get_field(field_idx);
    if (field) {
      read(objPtr, data_value, field);
    }
  }
}

}  // namespace mao::serialization
