#include "fepch.h"
#include "ScriptSerializer.h"

#include "ScriptManager.h"

#include <yaml-cpp/yaml.h>

namespace Frostic {

	void ScriptSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Scripts
		out << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
		ScriptManager::Each([&](uint64_t id, const ScriptManager::Data& data) 
			{
				out << YAML::BeginMap; // Script
				out << YAML::Key << "ID" << YAML::Value << id;
				out << YAML::Key << "Name" << YAML::Value << data.Name;
				out << YAML::Key << "HFilepath" << YAML::Value << data.HFilepath;
				out << YAML::Key << "CPPFilepath" << YAML::Value << data.CPPFilepath;
				out << YAML::EndMap; // Script
			});
		out << YAML::EndMap; // Scripts

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool ScriptSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scripts"])
		{
			FE_CORE_ERROR("File may be corrupted!");
			return false;
		}

		auto scripts = data["Scripts"];
		for (auto script : scripts)
			ScriptManager::AddScript(script["ID"].as<uint64_t>(), script["Name"].as<std::string>(), script["HFilepath"].as<std::string>(), script["CPPFilepath"].as<std::string>());
		return true;
	}

}