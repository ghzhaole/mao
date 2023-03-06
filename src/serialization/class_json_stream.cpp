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
		   PrettyWriter<StringBuffer> &writer,
		   classField *field,
		   bool isContainer);

void write(const std::shared_ptr<metaObject> &objPtr, PrettyWriter<StringBuffer> &writer, classField *field);

void write(const std::shared_ptr<metaObject> &objPtr, PrettyWriter<StringBuffer> &writer);

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj, classField *field, bool isContainer);

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj, classField *field);

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

void write(const std::shared_ptr<metaObject> &objPtr, PrettyWriter<StringBuffer> &writer, classField *field) {
  auto field_type = field->type().type();
  switch (field_type) {
	case metaTypes::TYPE_INT: writer.Int(*field->get<int>(objPtr));
	  break;
	case metaTypes::TYPE_INT64: writer.Int64(*field->get<int64_t>(objPtr));
	  break;
	case metaTypes::TYPE_FLOAT: writer.Double(*field->get<float>(objPtr));
	  break;
	case metaTypes::TYPE_DOUBLE: writer.Double(*field->get<double>(objPtr));
	  break;
	case metaTypes::TYPE_BOOL: writer.Bool(*field->get<bool>(objPtr));
	  break;
	case metaTypes::TYPE_STRING: writer.String(*field->get<string>(objPtr));
	  break;
	case metaTypes::TYPE_CHAR: writer.String(std::string(1, *field->get<char>(objPtr)));
	  break;
	case metaTypes::TYPE_OBJECT: write(*field->get<std::shared_ptr<metaObject>>(objPtr), writer);
	  break;
	case metaTypes::TYPE_LIST:
	case metaTypes::TYPE_MAP: write(objPtr, writer, field, true);
	  break;
  }
}

void write(const std::shared_ptr<metaObject> &objPtr,
		   PrettyWriter<StringBuffer> &writer,
		   classField *field,
		   bool isContainer) {
  if (field->is_list()) {
	writer.StartArray();
	auto sub_field_type = field->sub_type().type();
	switch (sub_field_type) {
	  case metaTypes::TYPE_INT: {
		size_t array_size = field->size<int>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  writer.Int(*field->get<int>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_INT64: {
		size_t array_size = field->size<int64_t>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  writer.Int64(*field->get<int64_t>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_FLOAT: {
		size_t array_size = field->size<float>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  writer.Double(*field->get<float>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_DOUBLE: {
		size_t array_size = field->size<double>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  writer.Double(*field->get<double>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_BOOL: {
		size_t array_size = field->size<bool>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  int val = 0;
		  writer.Bool(*field->get<bool>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_STRING: {
		size_t array_size = field->size<string>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  int val = 0;
		  writer.String(*field->get<string>(objPtr, array_idx));
		}
		break;
	  }
	  case metaTypes::TYPE_OBJECT: {
		using metaObject_ptr = std::shared_ptr<metaObject>;
		size_t array_size = field->size<metaObject_ptr>(objPtr);
		for (size_t array_idx = 0; array_idx < array_size; ++array_idx) {
		  write(*field->get<metaObject_ptr>(objPtr,  array_idx), writer);
		}
		break;
	  }
	}
	writer.EndArray();
  } else if (field->is_map()) {
	writer.StartObject();
	auto sub_field_type = field->sub_type().type();
	switch (sub_field_type) {
	  case metaTypes::TYPE_INT: {
		using map_int_type = map<string, int>;
		auto val = field->get<map_int_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.Int(item.second);
		  writer.EndObject();
		}
		break;
	  }
	  case metaTypes::TYPE_INT64: {
		using map_int64_type = map<string, int64_t>;
		auto val = field->get<map_int64_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.Int64(item.second);
		  writer.EndObject();
		}
		break;
	  }
	  case metaTypes::TYPE_FLOAT: {
		using map_float_type = map<string, float>;
		auto val = field->get<map_float_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.Double(item.second);
		  writer.EndObject();
		}
		break;
	  }
	  case metaTypes::TYPE_DOUBLE: {
		using map_double_type = map<string, double>;
		auto val = field->get<map_double_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.Double(item.second);
		  writer.EndObject();
		}
		break;
	  }
	  case metaTypes::TYPE_BOOL: {
		using map_bool_type = map<string, bool>;
		auto val = field->get<map_bool_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.Bool(item.second);
		  writer.EndObject();

		}
		break;
	  }
	  case metaTypes::TYPE_STRING: {
		using map_string_type = map<string, string>;
		auto val = field->get<map_string_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  writer.String(item.second);
		  writer.EndObject();
		}
		break;
	  }
	  case metaTypes::TYPE_OBJECT: {
		using map_obj_type = map<string, std::shared_ptr<metaObject>>;
		auto val = field->get<map_obj_type>(objPtr);
		for (auto item : *val) {
		  writer.StartObject();
		  writer.String("key");
		  writer.String(item.first);
		  writer.String("value");
		  write(item.second, writer);
		  writer.EndObject();
		}
		break;
	  }
	}
	writer.EndObject();
  }
}

void write(const std::shared_ptr<metaObject> &objPtr, PrettyWriter<StringBuffer> &writer) {
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

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj, classField *field) {
  auto ret = jsonObj.FindMember(field->name().data());
  if (ret == jsonObj.MemberEnd()) {
	return;
  }
  auto field_type = field->type().type();
  switch (field_type) {
	case metaTypes::TYPE_INT: field->set(objPtr, ret->value.GetInt());
	  break;
	case metaTypes::TYPE_INT64: field->set(objPtr, ret->value.GetInt64());
	  break;
	case metaTypes::TYPE_FLOAT: field->set(objPtr, ret->value.GetFloat());
	  break;
	case metaTypes::TYPE_DOUBLE: field->set(objPtr, ret->value.GetDouble());
	  break;
	case metaTypes::TYPE_BOOL: field->set(objPtr, ret->value.GetBool());
	  break;
	case metaTypes::TYPE_STRING:
	  field->set(objPtr, std::string(ret->value.GetString()));
	  break;
	case metaTypes::TYPE_CHAR: {
	  string val = ret->value.GetString();
	  if (val.length() > 0) {
		field->set(objPtr, val[0]);
	  } else {
		field->set<char>(objPtr, '\0');
	  }
	  break;
	}
	case metaTypes::TYPE_OBJECT: {
	  std::shared_ptr<metaObject> subObj;
	  read(subObj, ret->value);
	  field->set(objPtr, subObj);
	}
	case metaTypes::TYPE_MAP:
	case metaTypes::TYPE_LIST:read(objPtr, ret->value, field, true);
  }
}

void read(std::shared_ptr<metaObject> &objPtr, Value &jsonObj, classField *field, bool isContainer) {
  auto sub_field_type = field->sub_type().type();
  if (field->is_list()) {
	switch (sub_field_type) {
	  case metaTypes::TYPE_INT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetInt());
		}
		break;
	  case metaTypes::TYPE_INT64:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetInt64());
		}
		break;
	  case metaTypes::TYPE_FLOAT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetFloat());
		}
		break;
	  case metaTypes::TYPE_DOUBLE:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetDouble());
		}
		break;
	  case metaTypes::TYPE_BOOL:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetBool());
		}
		break;
	  case metaTypes::TYPE_STRING:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  field->set(objPtr, i, json_index_value.GetString());
		}
		break;
	  case metaTypes::TYPE_CHAR:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  string val = json_index_value.GetString();
		  if (val.size() > 0) {
			field->set(objPtr, i, val[0]);
		  } else {
			field->set(objPtr, i, '\0');
		  }
		}
		break;
	  case metaTypes::TYPE_OBJECT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  std::shared_ptr<metaObject> subObj;
		  read(subObj, json_index_value);
		  field->set(objPtr, i, subObj);
		}
		break;
	}
  } else if (field->is_map()) {
	switch (sub_field_type) {
	  case metaTypes::TYPE_INT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetInt());
		  }
		}
		break;
	  case metaTypes::TYPE_INT64:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetInt64());
		  }
		}
		break;
	  case metaTypes::TYPE_FLOAT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetFloat());
		  }
		}
		break;
	  case metaTypes::TYPE_DOUBLE:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetDouble());
		  }
		}
		break;
	  case metaTypes::TYPE_BOOL:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetBool());
		  }
		}
		break;
	  case metaTypes::TYPE_STRING:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			field->set(objPtr, key_itr->value.GetString(), value_itr->value.GetString());
		  }
		}
		break;
	  case metaTypes::TYPE_CHAR:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			std::string s = value_itr->value.GetString();
			char c = s.size() > 0 ? s[0] : '\0';
			field->set(objPtr, key_itr->value.GetString(), c);
		  }
		}
		break;
	  case metaTypes::TYPE_OBJECT:
		for (SizeType i = 0; i < jsonObj.Size(); ++i) {
		  auto &json_index_value = jsonObj[i];
		  Value::MemberIterator key_itr = json_index_value.FindMember("key");
		  Value::MemberIterator value_itr = json_index_value.FindMember("value");
		  if (key_itr != json_index_value.MemberEnd() && value_itr != json_index_value.MemberEnd()) {
			std::shared_ptr<metaObject> subObj;
			read(subObj, value_itr->value);
			field->set(objPtr, key_itr->value.GetString(), subObj);
		  }
		}
		break;
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
