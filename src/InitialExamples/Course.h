#pragma once
#include <iostream>
#include <vector>
#include "Student.h"
#include <fstream>

class Course {
private:
	std::string m_name = "Course";
	std::vector<Student> m_students;
public:
	Course() {}

	Course(const std::string& name)
		: m_name(name)
	{
	}

	void addStudent(const Student& s)
	{
		m_students.push_back(s);
	}

	// First const means whatever we are returning cannot be changed
	// const immediately after the getStudents means we are not changing any member variables with this function
	// const on a variable means it is initialized and never allowed to change from initialization
	const auto& getStudents() const
	{
		return m_students;
	}

	void loadFromFile(const std::string& fileName)
	{
		std::ifstream fin(fileName);
		std::string first, last;
		int id;
		float avg;

		// while fin reads something in that is "first"
		while (fin >> first)
		{
			// reads in last id and avg and puts the values into their variables
			fin >> last >> id >> avg;
			
			// Adds the student based on the loaded variables
			addStudent(Student(first, last, id, avg));
		}
	}

	void print() const
	{
		for (const auto& s : m_students)
		{
			s.print();
		}
	}
};
