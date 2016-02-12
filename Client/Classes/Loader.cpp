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

#include "Main.h"

Node* Loader::loadScene(const SString& fileName)
{
	auto screen = CSLoader::createNode(fileName);
	auto timeline = CSLoader::createTimeline(fileName);
	screen->runAction(timeline);
	timeline->gotoFrameAndPlay(0, true);
	return screen;
}

bool Loader::loadXMLFile(tinyxml2::XMLDocument* document, const SString& fileName)
{
	auto buffer = FileUtils::getInstance()->getStringFromFile(fileName);
	auto parseResult = document->Parse(buffer.c_str(), buffer.length());
	if (parseResult != tinyxml2::XML_NO_ERROR) {
		handleXMLError(parseResult);
		return false;
	}
	return true;
}

bool Loader::loadJSONFile(rapidjson::Document* document, const SString& fileName)
{
	document->Parse<0>(FileUtils::getInstance()->getStringFromFile(fileName).c_str());
	auto parseResult = document->GetParseError();
	if (parseResult != rapidjson::kParseErrorNone) {
		handleJSONError(parseResult);
		return false;
	}
	return true;
}

void Loader::handleXMLError(tinyxml2::XMLError xmlError)
{
	// TODO: Handle it.
	switch (xmlError) {
		case tinyxml2::XML_NO_ATTRIBUTE:
			CCLOG("XML_NO_ATTRIBUTE");
			break;

		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			CCLOG("XML_WRONG_ATTRIBUTE_TYPE");
			break;

		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			CCLOG("XML_ERROR_FILE_NOT_FOUND");
			break;

		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			CCLOG("XML_ERROR_FILE_COULD_NOT_BE_OPENED");
			break;

		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			CCLOG("XML_ERROR_FILE_READ_ERROR");
			break;

		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			CCLOG("XML_ERROR_ELEMENT_MISMATCH");
			break;

		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			CCLOG("XML_ERROR_PARSING_ELEMENT");
			break;

		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			CCLOG("XML_ERROR_PARSING_ATTRIBUTE");
			break;

		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			CCLOG("XML_ERROR_IDENTIFYING_TAG");
			break;

		case tinyxml2::XML_ERROR_PARSING_TEXT:
			CCLOG("XML_ERROR_PARSING_TEXT");
			break;

		case tinyxml2::XML_ERROR_PARSING_CDATA:
			CCLOG("XML_ERROR_PARSING_CDATA");
			break;

		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			CCLOG("XML_ERROR_PARSING_COMMENT");
			break;

		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			CCLOG("XML_ERROR_PARSING_DECLARATION");
			break;

		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			CCLOG("XML_ERROR_PARSING_UNKNOWN");
			break;

		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			CCLOG("XML_ERROR_EMPTY_DOCUMENT");
			break;

		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			CCLOG("XML_ERROR_MISMATCHED_ELEMENT");
			break;

		case tinyxml2::XML_ERROR_PARSING:
			CCLOG("XML_ERROR_PARSING");
			break;

		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			CCLOG("XML_CAN_NOT_CONVERT_TEXT");
			break;

		case tinyxml2::XML_NO_TEXT_NODE:
			CCLOG("XML_NO_TEXT_NODE");
			break;
	}
}

void Loader::handleJSONError(rapidjson::ParseErrorCode jsonError)
{
	// TODO: Handle it.
	switch (jsonError) {
		case rapidjson::kParseErrorDocumentEmpty:
			CCLOG("kParseErrorDocumentEmpty");
			break;

		case rapidjson::kParseErrorDocumentRootNotSingular:
			CCLOG("kParseErrorDocumentRootNotSingular");
			break;

		case rapidjson::kParseErrorValueInvalid:
			CCLOG("kParseErrorValueInvalid");
			break;

		case rapidjson::kParseErrorObjectMissName:
			CCLOG("kParseErrorObjectMissName");
			break;

		case rapidjson::kParseErrorObjectMissColon:
			CCLOG("kParseErrorObjectMissColon");
			break;

		case rapidjson::kParseErrorObjectMissCommaOrCurlyBracket:
			CCLOG("kParseErrorObjectMissCommaOrCurlyBracket");
			break;

		case rapidjson::kParseErrorArrayMissCommaOrSquareBracket:
			CCLOG("kParseErrorArrayMissCommaOrSquareBracket");
			break;

		case rapidjson::kParseErrorStringUnicodeEscapeInvalidHex:
			CCLOG("kParseErrorStringUnicodeEscapeInvalidHex");
			break;

		case rapidjson::kParseErrorStringUnicodeSurrogateInvalid:
			CCLOG("kParseErrorStringUnicodeSurrogateInvalid");
			break;

		case rapidjson::kParseErrorStringEscapeInvalid:
			CCLOG("kParseErrorStringEscapeInvalid");
			break;

		case rapidjson::kParseErrorStringMissQuotationMark:
			CCLOG("kParseErrorStringMissQuotationMark");
			break;

		case rapidjson::kParseErrorStringInvalidEncoding:
			CCLOG("kParseErrorStringInvalidEncoding");
			break;

		case rapidjson::kParseErrorNumberTooBig:
			CCLOG("kParseErrorNumberTooBig");
			break;

		case rapidjson::kParseErrorNumberMissFraction:
			CCLOG("kParseErrorNumberMissFraction");
			break;

		case rapidjson::kParseErrorNumberMissExponent:
			CCLOG("kParseErrorNumberMissExponent");
			break;

		case rapidjson::kParseErrorTermination:
			CCLOG("kParseErrorTermination");
			break;

		case rapidjson::kParseErrorUnspecificSyntaxError:
			CCLOG("kParseErrorUnspecificSyntaxError");
			break;
	}
}