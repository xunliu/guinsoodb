#pragma once

#include <string>
#include <cassert>
#include <cstring>

namespace guinsoodb {

enum class UnicodeType {INVALID, ASCII, UNICODE};


class Utf8Proc {
public:
	//! Distinguishes ASCII, Valid UTF8 and Invalid UTF8 strings
	static UnicodeType Analyze(const char *s, size_t len);
	//! Performs UTF NFC normalization of string, return value needs to be free'd
	static char* Normalize(const char* s, size_t len);
	//! Returns whether or not the UTF8 string is valid
	static bool IsValid(const char *s, size_t len);
	//! Returns the position (in bytes) of the next grapheme cluster
	static size_t NextGraphemeCluster(const char *s, size_t len, size_t pos);
	//! Returns the position (in bytes) of the previous grapheme cluster
	static size_t PreviousGraphemeCluster(const char *s, size_t len, size_t pos);

	//! Transform a codepoint to utf8 and writes it to "c", sets "sz" to the size of the codepoint
	static bool CodepointToUtf8(int cp, int &sz, char *c);
	//! Returns the codepoint length in bytes when encoded in UTF8
	static int CodepointLength(int cp);
	//! Transform a UTF8 string to a codepoint; returns the codepoint and writes the length of the codepoint (in UTF8) to sz
	static int32_t UTF8ToCodepoint(const char *c, int &sz);
    static size_t RenderWidth(const char *s, size_t len, size_t pos);

};

}
