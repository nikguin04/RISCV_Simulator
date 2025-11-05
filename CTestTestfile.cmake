# Create the tests

file(GLOB TEST_FILES tests/task*)

foreach(test IN LISTS TEST_FILES)
	message("Looping through ${test}")
	file(GLOB TEST_SUB_FILES ${test}/*.bin)
	foreach(subtest IN LISTS TEST_SUB_FILES)
		string(REGEX REPLACE ".+(task[0-9]+)\\/(.+)\\.bin$" "\\1_\\2" NAME "${subtest}")
		#message("- Adding test ${NAME}")
		cmake_path(REMOVE_EXTENSION subtest OUTPUT_VARIABLE TESTFILE)
		add_test(${NAME} build/TestInstantiator ${TESTFILE})
	endforeach()
endforeach()
