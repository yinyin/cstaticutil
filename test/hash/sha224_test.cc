/*
 * Test SHA-224 implementation (in the same code module of SHA-256)
 */

extern "C" {
#include "../../src/hash/sha256.c"
#include <string.h>
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace {

class SHA224Test : public testing::Test {
	protected:

	// SHA224Test();
	// virtual ~SHA224Test();
	virtual void SetUp();
	// virtual void TearDown();

	sha224_context digest_context;
};

void SHA224Test::SetUp() {
	sha224_init(&this->digest_context);
}

TEST_F(SHA224Test, DataSet1) {
	const char * content = "abc";
	const unsigned char expect[28] = {
			0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22,
			0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3,
			0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7,
			0xE3, 0x6C, 0x9D, 0xA7};
	unsigned char output[28];
	sha224_update(&this->digest_context, content, 3);
	sha224_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 28));
}

TEST_F(SHA224Test, DataSet2A) {
	const char * content = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	const unsigned char expect[28] = {
			0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,
			0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50,
			0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,
			0x52, 0x52, 0x25, 0x25};
	unsigned char output[28];
	sha224_update(&this->digest_context, content, 56);
	sha224_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 28));
}

TEST_F(SHA224Test, DataSet2B) {
	const char * content1 = "abcdb";
	const char * content2 = "cdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	const unsigned char expect[28] = {
			0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,
			0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50,
			0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,
			0x52, 0x52, 0x25, 0x25};
	unsigned char output[28];
	sha224_update(&this->digest_context, content1, 5);
	sha224_update(&this->digest_context, content2, 51);
	sha224_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 28));
}

TEST_F(SHA224Test, DataSet3) {
	const char * content[1000];
	const unsigned char expect[28] = {
			0x20, 0x79, 0x46, 0x55, 0x98, 0x0C, 0x91, 0xD8,
			0xBB, 0xB4, 0xC1, 0xEA, 0x97, 0x61, 0x8A, 0x4B,
			0xF0, 0x3F, 0x42, 0x58, 0x19, 0x48, 0xB2, 0xEE,
			0x4E, 0xE7, 0xAD, 0x67};
	unsigned char output[28];
	memset(content, 'a', 1000);
	for (int i = 0; i < 1000; i++) {
		sha224_update(&this->digest_context, content, 1000);
	}
	sha224_finish(&this->digest_context, output);
	EXPECT_THAT(output, testing::ElementsAreArray(expect, 28));
}

}	// name-space
