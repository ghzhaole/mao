//
// Created by leo on 2023/2/28.
//
#include <iostream>
#include <sstream>

#include "json/class_json.h"
#include "reflection/class_factory.h"
#include "serialization/class_json_stream.h"
#include "utils/library/class_library_loader.h"

using namespace mao::library;
using namespace mao::reflection;
using namespace mao::json;

int main() {
	classJson json = classJson::from_string("{\"1\":2,   \"2\":[1,2,{\"a\":{\"1\":2,   \"2\":[1,2,{\"a\":true}]}}]}");
	std::cout << json;
	return 0;
}