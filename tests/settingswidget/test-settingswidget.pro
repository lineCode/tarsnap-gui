TARGET = test-settingswidget
QT = core gui widgets sql network

FORMS += ../../forms/settingswidget.ui			\
	../../forms/logindialog.ui

HEADERS  +=						\
	../../src/debug.h				\
	../../src/scheduling.h				\
	../../src/tarsnapaccount.h			\
	../../src/translator.h				\
	../../src/utils.h				\
	../../src/widgets/confirmationdialog.h		\
	../../src/widgets/elidedlabel.h			\
	../../src/widgets/settingswidget.h		\
	../../tests/qtest-platform.h

SOURCES += test-settingswidget.cpp			\
	../../src/debug.cpp				\
	../../src/scheduling.cpp			\
	../../src/tarsnapaccount.cpp			\
	../../src/translator.cpp			\
	../../src/utils.cpp				\
	../../src/widgets/confirmationdialog.cpp	\
	../../src/widgets/elidedlabel.cpp		\
	../../src/widgets/settingswidget.cpp

include(../tests-include.pro)

# Add to normal definition for Unix (other than OSX) tests
unix:!macx {
test_home_prep.commands += ; mkdir -p "$${TEST_HOME}/$${TARGET}";	\
	cp confdir/* "$${TEST_HOME}/$${TARGET}"
}
