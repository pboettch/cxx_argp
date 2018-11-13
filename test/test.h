#ifndef TEST_H__
#define TEST_H__

static int result__;
static std::string test_name__;
std::vector<std::function<void(void)>> tests__;

#define TEST(cat, name)                                                              \
	static class cat##name##instantiator                                               \
	{                                                                                  \
	public:                                                                            \
		cat##name##instantiator() { tests__.push_back(&cat##name##instantiator::test); } \
		static void test();                                                              \
		static void test_impl();                                                         \
	} cat##name##instantiator__;                                                       \
	void cat##name##instantiator::test()                                               \
	{                                                                                  \
		test_name__ = std::string(#cat) + "::" + #name;                                  \
		auto result = result__;                                                          \
		test_impl();                                                                     \
		if (result != result__)                                                          \
			std::cerr << test_name__ << " FAILED\n";                                       \
		else                                                                             \
			std::cerr << test_name__ << " SUCCESS\n";                                      \
	}                                                                                  \
	void cat##name##instantiator::test_impl()

#define ASSERT_EQ(a, b)                                                \
	{                                                                    \
		auto a__ = a;                                                      \
		if (a__ != b) {                                                    \
			std::cerr << "ASSERTION FAILED in " << test_name__ << ": " << #a \
			          << " values to '" << a__ << "', expected '" << b << "'\n"; \
			result__++;                                                      \
			return;                                                          \
		}                                                                  \
	}

#define EXPECT_EQ(a, b)                                                \
	{                                                                    \
		auto a__ = a;                                                      \
		if (a__ != b) {                                                    \
			std::cerr << "EXPECT FAILED in " << test_name__ << ": " << #a    \
			          << " values to '" << a__ << "', expected '" << b << "'\n"; \
			result__++;                                                      \
		}                                                                  \
	}

#endif // TEST_H__
