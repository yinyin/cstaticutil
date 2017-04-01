/*
 * Test SHA-256 and SHA-224
 */

extern "C" {
#include "../../src/hash/sha256.c"
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace {

class SHA256Test : public testing::Test {
	protected:

	SHA256Test() {
	}

	virtual ~SHA256Test() {
	}

	virtual void SetUp();

	// virtual void TearDown();



	sha256_context digest_context;

};

void SHA256Test::SetUp() {
	sha256_init(&this->digest_context);
}

TEST_F(SHA256Test, DataSet1) {
	const char * content = "abc";
	const unsigned char expect[32] = {
			0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
			0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
			0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
			0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD};
	unsigned char output[32];
	sha256_update(&this->digest_context, content, 3);
	sha256_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 32));
}

}	// name-space

/*
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/
