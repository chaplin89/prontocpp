#include "Configuration.hpp"

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "version.hpp"

namespace po = boost::program_options;

bool Configuration::LoadConfig(int argc, const char * argv[]) {
	BuildSettingsDescription(argc, argv, config_);

	if (!ReadConfig(argc, argv, config_))
		return false;
	return true;
}

ProntoCppConfig Configuration::GetConfig()
{
	return config_;
}

bool Configuration::BuildSettingsDescription(int argc, const char *argv[], google::protobuf::Message& config, std::string postfix, std::string prefix) {
	std::vector<const google::protobuf::FieldDescriptor*> descriptors;
	auto descriptor = config.GetDescriptor();
	auto reflection = config.GetReflection();
	auto field_number = descriptor->field_count();

	for (int field_index = 0; field_index < field_number; field_index++) {
		auto field = descriptor->field(field_index);
		std::string field_name = prefix + field->containing_type()->name() + postfix + "." + field->name();
		description.add_options()(field_name.c_str(), "");
	}
	return true;
}

bool Configuration::ReadConfig(int argc, const char *argv[], google::protobuf::Message& config, std::string postfix, std::string prefix) {
	std::vector<const google::protobuf::FieldDescriptor*> descriptors;
	auto descriptor = config.GetDescriptor();
	auto reflection = config.GetReflection();
	auto field_number = descriptor->field_count();

	po::variables_map variable_map;
	po::store(po::parse_config_file(PROJECT_MAIN_CONFIG, description), variable_map);
	po::store(po::parse_command_line(argc, argv, description), variable_map);
	po::notify(variable_map);

	if (variable_map.count("help")) {
		std::cout << description << "\n";
		return false;
	}

	for (int field_index = 0; field_index < descriptor->field_count(); field_index++) {
		auto field = descriptor->field(field_index);
		std::string field_name = field->containing_type()->name() + postfix + "." + field->name();

		if (variable_map.count(field_name) == 0 && field->has_default_value())
			continue;

		std::string current_value = variable_map[field_name].as<std::string>();

		if (current_value.empty())
			continue;

		switch (field->cpp_type()) {
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
			reflection->SetInt32(&config, field, boost::lexical_cast<int32_t>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
			reflection->SetInt64(&config, field, boost::lexical_cast<int64_t>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
			reflection->SetUInt32(&config, field, boost::lexical_cast<uint32_t>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
			reflection->SetInt64(&config, field, boost::lexical_cast<uint64_t>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
			reflection->SetDouble(&config, field, boost::lexical_cast<double>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
			reflection->SetFloat(&config, field, boost::lexical_cast<float>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
			reflection->SetBool(&config, field, boost::lexical_cast<bool>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM:
			reflection->SetEnumValue(&config, field, boost::lexical_cast<int32_t>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
			reflection->SetString(&config, field, boost::lexical_cast<std::string>(current_value));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
			throw std::runtime_error("Nested objects are not supported.");
			break;
		default:
			throw std::runtime_error("Unrecognized type.");
			break;
		}
	}
	return true;
}
