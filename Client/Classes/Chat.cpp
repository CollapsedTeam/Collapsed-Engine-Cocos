#include "Main.h"
#include "Chat.h"

ImageView* Chat::_template = nullptr;

void Chat::create(const std::vector<Channel>& channels, Node* scene)
{
	_main = static_cast<ImageView*>(_template->clone());

	removeAllChannels();

	for (const auto& channel : channels)
		addChannel(std::get<0>(channel), std::get<1>(channel), std::get<2>(channel));

	adjust();
	setActiveChannel(Shared::ChatChannelID::LOCAL);

	GET_CHILD(_main, "btnSend", Button)->addClickEventListener([=](Ref* sender) {
		sendText();
	});

	if (scene == nullptr) 
		scene = game->getSceneFromRunningScreen();
	scene->addChild(_main);
}

void Chat::addChannel(Shared::ChatChannelID channelID, const std::string& text,
					  const std::function<void(Ref*)>& function)
{
	auto btnChannel = static_cast<Button*>(
		GET_CHILD(_template, "btnChannel0", Button)->clone());
	btnChannel->setTitleText(text);
	btnChannel->addClickEventListener([=](Ref* sender) {
		setActiveChannel(channelID);
	});
	if (function != nullptr)
		btnChannel->addClickEventListener(function);
	_main->addChild(btnChannel);
	_channels.push_back(std::pair<Shared::ChatChannelID, Button*>(channelID, btnChannel));
}

void Chat::addText(const SString& text)
{
	auto lstChat = GET_CHILD(_main, "lstChat", ListView);
}

void Chat::sendText()
{
	auto txtChat = GET_CHILD(_main->getChildByName("txtChatBg"), "txtChat", TextField);

	auto text = txtChat->getString();

	BITSTREAM(bitStream, Shared::PacketID::CHAT);
	// CHANNEL ID bitStream.write();
	bitStream.write(text);
	game->send(&bitStream);

	txtChat->setString("");
}

void Chat::removeAllChannels()
{
	s32 id = 0;
	for (s32 i = 0; i < _main->getChildrenCount(); ++i) {
		auto btnChannel = GET_CHILD(_main, StringUtils::format("btnChannel%d", id), Button);
		if (btnChannel == nullptr)
			break;

		btnChannel->removeFromParent();
	}
}

void Chat::setActiveChannel(Shared::ChatChannelID channelID)
{
	if (_channels.size() < 1)
		return;

	for (const auto& channel : _channels) {
		auto button = channel.second;
		button->setEnabled((channel.first != channelID));
		button->setBright((channel.first != channelID));
	}
}

void Chat::load(Node* scene)
{
	_template = GET_CHILD(scene, "Chat", ImageView);
	_template->retain();
	_template->removeFromParent();
}

void Chat::adjust()
{
	u32 channelsCount = _channels.size();
	float chatWidth = _main->getContentSize().width;
	float btnChannelWidth = chatWidth / channelsCount;

	for (u32 i = 0u; i < channelsCount; ++i) {
		auto button = _channels[i].second;
		button->setContentSize(Size(btnChannelWidth, button->getContentSize().height));
		button->setPositionX(btnChannelWidth * i);
	}
}

