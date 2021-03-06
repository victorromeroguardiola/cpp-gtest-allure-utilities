#include "AllureAPI.h"

#include "Model/TestProperty.h"
#include "Services/ServicesFactory.h"
#include "Services/EventHandlers/ITestStepStartEventHandler.h"
#include "Services/EventHandlers/ITestStepEndEventHandler.h"
#include "Services/GoogleTest/IGTestStatusChecker.h"
#include "Services/Property/ITestCasePropertySetter.h"
#include "Services/Property/ITestSuitePropertySetter.h"


namespace systelab { namespace gtest_allure {

	model::TestProgram AllureAPI::m_testProgram = model::TestProgram();
	service::IServicesFactory* AllureAPI::m_servicesFactory = new service::ServicesFactory(m_testProgram);

	std::unique_ptr<::testing::TestEventListener> AllureAPI::buildListener()
	{
		return getServicesFactory()->buildGTestEventListener();
	}

	model::TestProgram& AllureAPI::getTestProgram()
	{
		return m_testProgram;
	}

	void AllureAPI::setTestProgramName(const std::string& name)
	{
		m_testProgram.setName(name);
	}

	void AllureAPI::setOutputFolder(const std::string& outputFolder)
	{
		m_testProgram.setOutputFolder(outputFolder);
	}

	void AllureAPI::setTMSLinksPattern(const std::string& tmsLinkPattern)
	{
		m_testProgram.setTMSLinksPattern(tmsLinkPattern);
	}

	void AllureAPI::setTMSId(const std::string& value)
	{
		auto testSuitePropertySetter = getServicesFactory()->buildTestSuitePropertySetter();
		testSuitePropertySetter->setProperty(model::test_property::TMS_ID_PROPERTY, value);
	}

	void AllureAPI::setTestSuiteName(const std::string& name)
	{
		setTestSuiteLabel(model::test_property::NAME_PROPERTY, name);
	}

	void AllureAPI::setTestSuiteDescription(const std::string& description)
	{
		setTestSuiteLabel(model::test_property::FEATURE_PROPERTY, description);
	}

	void AllureAPI::setTestSuiteEpic(const std::string& epic)
	{
		setTestSuiteLabel(model::test_property::EPIC_PROPERTY, epic);
	}

	void AllureAPI::setTestSuiteSeverity(const std::string& severity)
	{
		setTestSuiteLabel(model::test_property::SEVERITY_PROPERTY, severity);
	}

	void AllureAPI::setTestSuiteLabel(const std::string& name, const std::string& value)
	{
		auto testSuitePropertySetter = getServicesFactory()->buildTestSuitePropertySetter();
		testSuitePropertySetter->setProperty(name, value);
	}

	void AllureAPI::setTestCaseName(const std::string& name)
	{
		auto testCasePropertySetter = getServicesFactory()->buildTestCasePropertySetter();
		testCasePropertySetter->setProperty(model::test_property::NAME_PROPERTY, name);
	}

	void AllureAPI::addAction(const std::string& name, std::function<void()> actionFunction)
	{
		addStep(name, true, actionFunction);
	}

	void AllureAPI::addExpectedResult(const std::string& name, std::function<void()> verificationFunction)
	{
		addStep(name, false, verificationFunction);
	}

	void AllureAPI::addStep(const std::string& name, bool isAction, std::function<void()> stepFunction)
	{
		auto stepStartEventHandler = getServicesFactory()->buildTestStepStartEventHandler();
		stepStartEventHandler->handleTestStepStart(name, isAction);

		stepFunction();

		auto statusChecker = getServicesFactory()->buildGTestStatusChecker();
		auto currentStatus = statusChecker->getCurrentTestStatus();

		auto stepEndEventHandler = getServicesFactory()->buildTestStepEndEventHandler();
		stepEndEventHandler->handleTestStepEnd(currentStatus);
	}

	service::IServicesFactory* AllureAPI::getServicesFactory()
	{
		auto configuredServicesFactoryInstance = service::ServicesFactory::getInstance();
		if (configuredServicesFactoryInstance)
		{
			return configuredServicesFactoryInstance;
		}
		else
		{
			return m_servicesFactory;
		}
	}

}}
