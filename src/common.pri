# Give code coverage with CONFIG+=testing
CONFIG(testing) {
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -lgcov -coverage
}
