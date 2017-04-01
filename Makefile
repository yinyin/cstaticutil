
CFLAGS += -Wall

CXXFLAGS += -Wall

TESTCXXFLAGS := $(CXXFLAGS) $(TESTCXXFLAGS)

TESTLDFLAGS := $(LDFLAGS) $(TESTLDFLAGS)

TESTLIBS := $(LIBS) -lgtest_main -lgmock


test/%.o: test/%.cc
	$(CXX) $(TESTCXXFLAGS) -c -o $@ $^


all: test


test/hash/sha256_test: test/hash/sha256_test.cc src/hash/sha256.c
	$(CXX) $(TESTCXXFLAGS) -DDISABLE_SHA224=1 -o $@ $< $(TESTLDFLAGS) $(TESTLIBS)


test: test/hash/sha256_test
	$(foreach f, $^, ./$(f);)

clean:
	$(RM) test/hash/sha256_test
