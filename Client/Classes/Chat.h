#pragma once

class Chat
{
	friend class LoadingScreen;

public:
	typedef std::tuple<Shared::ChatChannelID, std::string, std::function<void(Ref*)>> 
		Channel;

public:
	void create(const std::vector<Chat::Channel>& channels, Node* scene = nullptr);

	void addChannel(Shared::ChatChannelID channelID, const std::string& text,
					const std::function<void(Ref*)>& function);
	
	void addText(const SString& text);

	void sendText();

	void removeAllChannels();

	void setActiveChannel(Shared::ChatChannelID channelID);

private:
	static void load(Node* scene);

	void adjust();

private:
	ImageView* _main;
	std::vector<std::pair<Shared::ChatChannelID, Button*>> _channels;

	static ImageView* _template;
};
