#pragma once
#include <string>

class Action {

public:
	Action();
	Action(const std::string& name, const std::string& type);

	const std::string& name() const;
	const std::string& type() const;

private:
	std::string m_name;
	std::string m_type;
};