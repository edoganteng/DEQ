Compiling/running unit tests
------------------------------------

Unit tests will be automatically compiled if dependencies were met in configure
and tests weren't explicitly disabled.

After configuring, they can be run with 'make check'.

To run the dequantd tests manually, launch src/test/test_dequant .

To add more dequantd tests, add `BOOST_AUTO_TEST_CASE` functions to the existing
.cpp files in the test/ directory or add new .cpp files that
implement new BOOST_AUTO_TEST_SUITE sections.

To run the dequant-qt tests manually, launch src/qt/test/dequant-qt_test

To add more dequant-qt tests, add them to the `src/qt/test/` directory and
the `src/qt/test/test_main.cpp` file.
