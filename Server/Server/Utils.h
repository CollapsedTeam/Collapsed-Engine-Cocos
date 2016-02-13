/*
 * Copyright 2016 Collapsed Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

class Utils
{
public:
	/**
	 * Clamps value.
	 * @param value The value to clamp.
	 * @param low The lowest value.
	 * @param high The highest value.
	 * @return Return clamped value
	 */
	template <typename Type> 
	static Type clamp(const Type& value, const Type& low, const Type& high) { 
		return (value < low) ? low : ((value > high) ? high : value); 
	}

	/**
	 * Converts string to float number.
	 * @param string String to be converted.
	 * @return Return converted number.
	 */
	static float strToFloat(cchar* string) {
		float variable;
		strToFloat(string, &variable);
		return variable;
	}

	/**
	 * Faster version of converting string to float number.
	 * @param string String to be converted.
	 * @param variable Pointer to float variable.
	 */
	static void strToFloat(cchar* string, float* variable) {
		sscanf(string, "%f", variable); 
	}

	/**
	 * Converts string to boolean.
	 * @param string String to be converted.
	 * @return Return converted boolean.
	 */
	static bool strToBool(cchar* string) {
		return !(strcmp(string, "false") == 0 || strcmp(string, "0") == 0); 
	}

	/**
	 * Converts integer to std::string.
	 * @param value Integer to be converted.
	 * @return Return converted string.
	 */
	static StdString intToStr(s32 value) {
		char buffer[11];
		intToStr(value, buffer);
		return buffer;
	}

	/**
	 * Faster version of converting integer to string.
	 * @param value Integer to be converted.
	 * @param buffer Buffer for string.
	 */
	static void intToStr(s32 value, char* buffer) { 
		sprintf(buffer, "%d", value); 
	}

	/**
	 * Converts unsigned integer to std::string.
	 * @param value Unsigned integer to be converted.
	 * @return Return converted string.
	 */
	static std::string uintToStr(u32 value) {
		char buffer[11];
		uintToStr(value, buffer);
		return buffer;
	}

	/**
	 * Faster version of converting unsigned integer to string.
	 * @param value Unsigned integer to be converted.
	 * @param buffer Buffer for string.
	 */
	static void uintToStr(u32 value, char* buffer) { 
		sprintf(buffer, "%u", value); 
	}

	/**
	 * Checks if ip has valid format.
	 * @param ip IP to check.
	 * @return Return true if IP has valid format. Otherwise, it returns false.
	 */
	static bool isIPFormat(const String& ip) {
		sockaddr_in sa;
		return (inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 0);
	}
};
