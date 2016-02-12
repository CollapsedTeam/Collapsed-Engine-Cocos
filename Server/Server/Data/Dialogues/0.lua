-- @REQUIRED
function init(this, character)
	writeDebugLine(string.format("Dialogue %u has been initialized by %s.", this:getID(), character:getName()))

	-- Adds 1 normal flag for this dialogue to character. 
	character:addFlags(1)
	
	-- Adds 1 savable flag (to database) for this dialogue to character.
	character:addSavableFlags(this, 1)
	
	-- Return true if initial requirements of character are met. False otherwise.
	return true
end

-- @REQUIRED
function start(this, character)
	writeDebugLine(string.format("Dialogue %u has been started by %s.", this:getID(), character:getName()))
	
	-- Starts dialogue with specified id of text and id of answer.
	return firstTexts()
end

-- @REQUIRED
function update(this, character, lastID)
	if lastID == "o1" then
		return "t2"
	elseif lastID == "t2" then
		return firstTexts()
	elseif lastID == "o2" then
		return questions(this, character)
	elseif lastID == "o8" then
		return "[END]"
	elseif lastID == "o3" then
		character:setFlag(0, TRUE)
		return "t5"
	elseif lastID == "o4" then
		character:setFlag(0, TRUE)
		return "t6", "o5", "o6"
	elseif lastID == "o6" then
		return "t7"
	elseif lastID == "o7" then
		character:setFlag(0, TRUE)
		return firstTexts()
	elseif lastID == "t5" or lastID == "o5" then
		return questions(this, character)
	elseif lastID == "t7" then
		return "t8"
	elseif lastID == "t8" then
		return "t9"
	elseif lastID == "t9" then
		character:setSavableFlag(this, 0, TRUE)
		character:levelUp()
		return questions(this, character)
	end
	
	return "[ERROR]"
end

function firstTexts()
	return "t1", "o1", "o2", "o8"
end

function questions(this, character)
	local text
	if character:getFlag(0) == FALSE then
		text = "t3"
	else
		text = "t4"
	end

	if character:getSavableFlag(this, 0) == FALSE then
		return text, "o3", "o4", "o7"
	else
		return text, "o3", "o7"
	end
end