#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <memory>
#include <string>
#include <array>
#include <boost/program_options.hpp>
#include <google/protobuf/message.h>
#include <ProntoCppConfig.pb.h>

class Configuration {
public:

	bool LoadConfig(int argc, const char *argv[]);

	ProntoCppConfig GetConfig();

private:
	
	ProntoCppConfig config_;

	boost::program_options::options_description description;

	bool BuildSettingsDescription(int argc, const char *argv[], google::protobuf::Message& config, std::string postfix = std::string(), std::string prefix = std::string());

	bool ReadConfig(int argc, const char *argv[], google::protobuf::Message& config, std::string postfix = std::string(), std::string prefix = std::string());
};

#endif