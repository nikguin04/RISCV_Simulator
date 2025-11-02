# Create the tests


file(GLOB TEST_FILES tests/task*)

foreach(test IN LISTS TEST_FILES)
    message("Looping through: ${test}")
	file(GLOB TEST_SUB_FILES ${test}/*.bin)
	foreach(subtest IN LISTS TEST_SUB_FILES)
		#message("${subtest}")
		string(REGEX REPLACE ".+(task[0-9]+)\\/(.+)\\.bin$" "\\1_\\2" NAME "${subtest}")
		#message("${NAME}")
		string(REGEX REPLACE "(.+)\\.bin$" "\\1" TESTFILE "${subtest}")
		add_test(${NAME} build/TestInstantiator ${TESTFILE})
	endforeach()
endforeach()

#message(TEST_FILES="${TEST_FILES}")
