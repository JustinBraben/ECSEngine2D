#pragma once
#include <iostream>
#include <vector>

class Student {
private:
	std::string m_first	= "First";
	std::string m_last	= "Last";
	int m_id			= 0;
	float m_avg			= 0;

public:

	Student() {}

	Student(std::string first, std::string last, int id, float avg)
		:	m_first(first)
		,	m_last(last)
		,	m_id(id)
		,	m_avg(avg)
	{
	}

	auto getAvg() const
	{
		return m_avg;
	}

	auto getID() const
	{
		return m_id;
	}

	auto getFirst() const
	{
		return m_first;
	}

	auto getLast() const
	{
		return m_last;
	}

	void print() const // This will never change the object, hence the const
	{
		std::cout << m_first << " " << m_last << " ";
		std::cout << m_id << " " << m_avg << "\n";
	}
};