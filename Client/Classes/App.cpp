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
#include "LoadingScreen.h"

void App::initGLContextAttrs()
{
    // Set OpenGL context attributions, now can only set six attributions:
    // red, green, blue, alpha, depth, stencil.
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool App::applicationDidFinishLaunching() 
{
	// Adds search path, so we don't need to everywhere write "res" in path to file.
	// This function should not be over exceeded, due to possible ambiguous file names.
	FileUtils::getInstance()->addSearchPath("res");

	loadConfigFile();
	setupView();
	runFirstScene();

    return true;
}

void App::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void App::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void App::saveConfigFile()
{
	tinyxml2::XMLDocument config;

	// Inserts basic declaration.
	config.InsertFirstChild(config.NewDeclaration());

	// Inserts root node.
	auto rootNode = config.InsertEndChild(config.NewElement("config"));

	// Inserts display node.
	auto element = rootNode->InsertEndChild(config.NewElement("display"))->ToElement();
	element->SetAttribute("width", _resolution.width);
	element->SetAttribute("height", _resolution.height);
	element->SetAttribute("stats", _displayStats);
	element->SetAttribute("locale", _lang.getLocale());

	// Inserts sound node.
	element = rootNode->InsertEndChild(config.NewElement("sound"))->ToElement();
	element->SetAttribute("music", _musicVolume);
	element->SetAttribute("sfx", _sfxVolume);

	// Create config file in writable path.
	auto filePath = FileUtils::getInstance()->getWritablePath() + "config.xml";
	auto file = fopen(filePath.c_str(), "w");
	if (file == nullptr) {
		game->exit();
		return;
	}
	
	// Convert XML data structure to printable format.
	tinyxml2::XMLPrinter printer;
	config.Print(&printer);

	// Write to file.
	fwrite(printer.CStr(), sizeof(char), printer.CStrSize() - 1, file);

	// Saves changes and closes file.
	fclose(file);
}

void App::exit()
{
	SimpleAudioEngine::getInstance()->end();
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void App::setResolution(const Size& resolution)
{
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();

#if DESKTOP_PLATFORM
	// The same resolution has no effects.
	if (_resolution.equals(resolution))
		return;

	_resolution = resolution;
#else // DESKTOP_PLATFORM
	_resolution = glView->getFrameSize();
#endif // DESKTOP_PLATFORM

	
	if (glView == nullptr) {
		// Creates new OpenGL view.
		glView = GLViewImpl::createWithRect(APPLICATION_NAME, Rect(0.0f, 0.0f, 
			resolution.width, resolution.height));

		// Set recently created OpenGL view.
		director->setOpenGLView(glView);

#if (ONLY_2D == 1)
		director->setProjection(Director::Projection::_2D);
#endif // (ONLY_2D == 1)
	} else {
		// Reset sizes.
#if DESKTOP_PLATFORM
		glView->setFrameSize(resolution.width, resolution.height);
#endif // DESKTOP_PLATFORM
		glView->setViewPortInPoints(0.0f, 0.0f, resolution.width, resolution.height);
	}

	// Determines if we have the same aspect ratio as design resolution.
	bool sameAspectRatio = ((resolution.width / resolution.height) == 
							(DESIGN_RESOLUTION_WIDTH / DESIGN_RESOLUTION_HEIGHT));

	// Set design resolution.
	glView->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT,
									sameAspectRatio ? ResolutionPolicy::EXACT_FIT :
									ResolutionPolicy::SHOW_ALL);
}


void App::setLanguage(const SString& locale)
{
	// We don't want to reload language.
	if (locale == _lang.getLocale())
		return;

	// Loads new language.
	_lang.load(locale.c_str());

	getCurrentScreen<Screen>()->setLanguage();
	game->getOptionsWindow()->setLanguage();
}

void App::setDisplayStats(bool displayStats)
{
	_displayStats = displayStats;
	Director::getInstance()->setDisplayStats(displayStats);
}

void App::setMusicVolume(u8 musicVolume)
{
	_musicVolume = musicVolume;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(
		1.0f / static_cast<float>(musicVolume));
}

void App::setSFXVolume(u8 sfxVolume)
{
	_sfxVolume = sfxVolume;
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f / static_cast<float>(sfxVolume));
}

void App::createConfigFile()
{
	tinyxml2::XMLDocument config;

	// Inserts basic declaration.
	config.InsertFirstChild(config.NewDeclaration());

	// Inserts root node.
	auto rootNode = config.InsertEndChild(config.NewElement("config"));
	
	// Inserts display node.
	auto element = rootNode->InsertEndChild(config.NewElement("display"))->ToElement();
	element->SetAttribute("width", DESIGN_RESOLUTION_WIDTH);
	element->SetAttribute("height", DESIGN_RESOLUTION_HEIGHT);
	element->SetAttribute("stats", false);
	element->SetAttribute("locale", "en_EN");

	// Inserts sound node.
	element = rootNode->InsertEndChild(config.NewElement("sound"))->ToElement();
	element->SetAttribute("music", 100);
	element->SetAttribute("sfx", 100);

	// Create config file in writable path.
	auto filePath = FileUtils::getInstance()->getWritablePath() + "config.xml";
	auto file = fopen(filePath.c_str(), "w");
	if (file == nullptr) {
		game->exit();
		return;
	}
	// Convert XML data structure to printable format.
	tinyxml2::XMLPrinter printer;
	config.Print(&printer);

	// Write to file.
	fwrite(printer.CStr(), sizeof(char), printer.CStrSize() - 1, file);

	// Saves changes and closes file.
	fclose(file);
}

void App::loadConfigFile()
{
	auto io = FileUtils::getInstance();

	// If config file doesn't exist then we need to create it in writable path.
	auto configPath = io->getWritablePath() + "config.xml";
	if (!io->isFileExist(configPath))
		createConfigFile();

	tinyxml2::XMLDocument config;
	if (!Loader::loadXMLFile(&config, configPath)) {
		// Something went wrong, so we try to create/recreate file...
		createConfigFile();
		
		// ... and try to load once again.
		if (!Loader::loadXMLFile(&config, configPath)) {
			game->exit();
			return;
		}
	}
	
	auto rootElement = config.RootElement();

	// Loads data from display node.
	auto element = rootElement->FirstChildElement("display");
	_displayStats = element->BoolAttribute("stats");
	setResolution(Size(element->IntAttribute("width"), element->IntAttribute("height")));
	if (!_lang.load(element->Attribute("locale"))) {
		game->exit();
		return;
	}

	// Loads data from sound node.
	element = rootElement->FirstChildElement("sound");
	setMusicVolume(element->IntAttribute("music"));
	setSFXVolume(element->IntAttribute("sfx"));
}

void App::setupView()
{
	// Set max FPS, 60 FPS is maximum. There's no need to see more.
	Director::getInstance()->setAnimationInterval(1.0f / 60.0f);
	setDisplayStats(_displayStats);
}

void App::runFirstScene()
{
	// Create and run first scene - Loading Screen.
	Director::getInstance()->runWithScene(LoadingScreen::createScene());
}