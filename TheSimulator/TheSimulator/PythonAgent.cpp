#include "PythonAgent.h"
#include "Simulation.h"
#include <filesystem>

#include <pybind11/stl.h>

PythonAgent::PythonAgent(const Simulation* simulation, const std::string& pythonClass, const std::string& file)
	: Agent(simulation), m_class(pythonClass), m_file(file) { }

PythonAgent::PythonAgent(const Simulation* simulation, const std::string& name)
	: Agent(simulation, name), m_class(""), m_file("") { }

void PythonAgent::configure(const pugi::xml_node& node, const std::string& configurationPath) {
	Agent::configure(node, configurationPath);

	for (const pugi::xml_attribute& attr : node.attributes()) {
		if (std::string(attr.name()) != "file" && std::string(attr.name()) != "name") {
			m_parameters[std::string(attr.name())] = simulation()->parameters().processString(attr.as_string());
		}
	}

	py::object agentClass;
	std::string moduleName;
	std::string moduleDir;

	if (m_file == "") {
		// No file specified, use class name as module name
		moduleName = m_class;
		// Assume it's already in the Python path
	} else {
		// File specified, extract module name and directory
		std::filesystem::path filePath(m_file);
		moduleName = filePath.stem().string();  // filename without extension
		moduleDir = filePath.parent_path().string();
		
		// Add the directory to Python path so we can import it
		if (!moduleDir.empty()) {
			py::module sys = py::module::import("sys");
			py::list path = sys.attr("path");
			path.insert(0, moduleDir);
		}
	}

	// Now use the same import approach for both
	py::module m = py::module::import(moduleName.c_str());
	agentClass = m.attr(m_class.c_str());

	py::cpp_function nameStringFunction = [this]() {
		return this->name();
	};
	//py::object nameStringObject = py::str(name());
	agentClass.attr("name") = nameStringFunction;
	m_instance = agentClass();

	py::function fun = py::reinterpret_borrow<py::function>(m_instance.attr("configure"));
	py::object _ret = fun(m_parameters);
}

#include <iostream>

void PythonAgent::receiveMessage(const MessagePtr& msg) {
	py::function receiveMessageFunction = py::reinterpret_borrow<py::function>(m_instance.attr("receiveMessage"));
	py::object _ret = receiveMessageFunction(simulation(), msg->type, msg->payload, msg->source);
}
