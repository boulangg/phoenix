

//extern "C" const void* __start_TEST_SECTION_INIT_METADATA2;
//extern "C" const void* __stop_TEST_SECTION_INIT_METADATA2;
//
//__attribute((used)) void init_section()
//{
//	typedef void (*voidFunc)();
//	voidFunc* iter2 = (voidFunc*)&__start_TEST_SECTION_INIT_METADATA2;
//	for (; iter2 < (voidFunc*)&__stop_TEST_SECTION_INIT_METADATA2; ++iter2) {
//		(*iter2)();
//	}
//}