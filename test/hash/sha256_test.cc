/*
 * Test SHA-256 implementation
 */

extern "C" {
#include "../../src/hash/sha256.c"
#include <string.h>
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace {

class SHA256Test : public testing::Test {
	protected:

	// SHA256Test();
	// virtual ~SHA256Test();
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

TEST_F(SHA256Test, DataSet2A) {
	const char * content = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	const unsigned char expect[32] = {
			0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
			0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
			0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
			0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1};
	unsigned char output[32];
	sha256_update(&this->digest_context, content, 56);
	sha256_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 32));
}

TEST_F(SHA256Test, DataSet2B) {
	const char * content1 = "abcdb";
	const char * content2 = "cdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	const unsigned char expect[32] = {
			0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
			0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
			0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
			0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1};
	unsigned char output[32];
	sha256_update(&this->digest_context, content1, 5);
	sha256_update(&this->digest_context, content2, 51);
	sha256_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 32));
}

TEST_F(SHA256Test, DataSet3) {
	const char * content[1000];
	const unsigned char expect[32] = {
			0xCD, 0xC7, 0x6E, 0x5C, 0x99, 0x14, 0xFB, 0x92,
			0x81, 0xA1, 0xC7, 0xE2, 0x84, 0xD7, 0x3E, 0x67,
			0xF1, 0x80, 0x9A, 0x48, 0xA4, 0x97, 0x20, 0x0E,
			0x04, 0x6D, 0x39, 0xCC, 0xC7, 0x11, 0x2C, 0xD0};
	unsigned char output[32];
	memset(content, 'a', 1000);
	for (int i = 0; i < 1000; i++) {
		sha256_update(&this->digest_context, content, 1000);
	}
	sha256_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 32));
}

}	// name-space
