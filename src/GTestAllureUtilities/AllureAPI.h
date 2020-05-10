#pragma once

#include "Model/TestProgram.h"

#include <functional>
#include <gtest/gtest.h>
#include <memory>


namespace systelab { namespace gtest_allure {

	class AllureAPI
	{
	public:
		static std::unique_ptr<::testing::TestEventListener> buildListener();

		static model::TestProgram& getTestProgram();
		static void setTestProgramName(const std::string&);
		static void setOutputFolder(const std::string&);

		static void addAction(const std::string& name, std::function<void()>);
		static void addExpectedResult(const std::string& name, std::function<void()>);
		static void addParameter(const std::string& name, const std::string& value);

	private:
		static model::TestProgram m_testProgram;
	};

}}
