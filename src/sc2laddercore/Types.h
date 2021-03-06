#pragma once

#include <string>
#include <sc2api/sc2_api.h>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>

class PrintThread : public std::ostringstream
{
public:
    PrintThread() = default;

    ~PrintThread()
    {
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::cout << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ": " << this->str();
    }

private:
    static std::mutex _mutexPrint;
};

enum BotType
{
	BinaryCpp,
	CommandCenter,
	Python,
	Wine,
	Mono,
	DotNetCore,
	DefaultBot,
    Java,
    NodeJS,
};

enum ResultType
{
	InitializationError,
	Timeout,
	ProcessingReplay,
	Player1Win,
	Player1Crash,
	Player2Win,
	Player2Crash,
	Tie,
	Error
};

enum MatchupListType
{
    File,
    URL,
    None
};

enum ExitCase
{
	InProgress,
	GameEnd,
	ClientRequestExit,
	ClientTimeout,
	GameTimeout
};

struct GameState
{
	bool IsInGame;
	int GameLoop;
	float Score;
	GameState()
		: IsInGame(true)
		, GameLoop(-1)
		, Score(-1)
	{}

};

struct GameResult
{
    ResultType Result;
    float Bot1AvgFrame;
    float Bot2AvgFrame;
    uint32_t GameLoop;
    std::string TimeStamp;
    GameResult()
        : Result(ResultType::InitializationError)
        , Bot1AvgFrame(0)
        , Bot2AvgFrame(0)
        , GameLoop(0)
        , TimeStamp("")
    {}

};

struct BotConfig
{
	BotType Type;
	std::string BotName;
	std::string RootPath;
	std::string FileName;
	sc2::Race Race;
	sc2::Difficulty Difficulty;
	std::string Args; //Optional arguments
	std::string PlayerId;
	bool Debug;
    bool Enabled;
    bool Skeleton;
    int ELO;
	BotConfig()
		: Type(BotType::BinaryCpp)
		, Race(sc2::Race::Random)
		, Difficulty(sc2::Difficulty::Easy)
		, Debug(false)
        , Enabled(true)
        , Skeleton(false)
        , ELO(0)
	{}
	BotConfig(BotType InType, const std::string & InBotName, sc2::Race InBotRace, const std::string & InBotPath, const std::string & InFileName, sc2::Difficulty InDifficulty = sc2::Difficulty::Easy, const std::string & InArgs = "")
		: Type(InType)
		, Race(InBotRace)
		, BotName(InBotName)
		, RootPath(InBotPath)
		, FileName(InFileName)
		, Difficulty(InDifficulty)
		, Args(InArgs)
		, Debug(false)
        , Skeleton(false)
    {}
	bool operator ==(const BotConfig &Other) const
	{
		return BotName == Other.BotName;
	}
};

struct Matchup
{
	BotConfig Agent1;
	BotConfig Agent2;
    std::string Bot1Id;
    std::string Bot1Checksum;
    std::string Bot2Id;
    std::string Bot2Checksum;
    std::string Map;
	Matchup() {}
	Matchup(const BotConfig &InAgent1, const BotConfig &InAgent2, const std::string &InMap)
		: Agent1(InAgent1),
		Agent2(InAgent2),
		Map(InMap)
	{

	}


};

static MatchupListType GetMatchupListTypeFromString(const std::string GeneratorType)
{
    std::string type(GeneratorType);
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "url")
    {
        return MatchupListType::URL;
    }
    else if (type == "file")
    {
        return MatchupListType::File;
    }
    else
    {
        return MatchupListType::None;
    }
}

static std::string GetExitCaseString(ExitCase ExitCaseIn)
{
	switch (ExitCaseIn)
	{
	case ExitCase::ClientRequestExit:
		return "ClientRequestExit";
	case ExitCase::ClientTimeout:
		return "ClientTimeout";
	case ExitCase::GameEnd:
		return "GameEnd";
	case ExitCase::GameTimeout:
		return "GameTimeout";
	case ExitCase::InProgress:
		return "InProgress";
	}
	return "Error";
}

static sc2::Race GetRaceFromString(const std::string & RaceIn)
{
	std::string race(RaceIn);
	std::transform(race.begin(), race.end(), race.begin(), ::tolower);

	if (race == "terran")
	{
		return sc2::Race::Terran;
	}
	else if (race == "protoss")
	{
		return sc2::Race::Protoss;
	}
	else if (race == "zerg")
	{
		return sc2::Race::Zerg;
	}
	else if (race == "random")
	{
		return sc2::Race::Random;
	}

	return sc2::Race::Random;
}

static std::string GetRaceString(const sc2::Race RaceIn)
{
	switch (RaceIn)
	{
	case sc2::Race::Protoss:
		return "Protoss";
	case sc2::Race::Random:
		return "Random";
	case sc2::Race::Terran:
		return "Terran";
	case sc2::Race::Zerg:
		return "Zerg";
	}
	return "Random";
}



static BotType GetTypeFromString(const std::string &TypeIn)
{
	std::string type(TypeIn);
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	if (type == "binarycpp")
	{
		return BotType::BinaryCpp;
	}
	else if (type == "commandcenter")
	{
		return BotType::CommandCenter;
	}
	else if (type == "computer")
	{
		return BotType::DefaultBot;
	}
	else if (type == "python")
	{
		return BotType::Python;
	}
	else if (type == "wine")
	{
		return BotType::Wine;
	}
	else if (type == "dotnetcore")
	{
		return BotType::DotNetCore;
	}
	else if (type == "mono")
	{
		return BotType::Mono;
	}
    else if (type == "java")
    {
        return BotType::Java;
    }
    else if (type == "nodejs")
    {
        return BotType::NodeJS;
    }
	return BotType::BinaryCpp;
}

static sc2::Difficulty GetDifficultyFromString(const std::string &InDifficulty)
{
	if (InDifficulty == "VeryEasy")
	{
		return sc2::Difficulty::VeryEasy;
	}
	if (InDifficulty == "Easy")
	{
		return sc2::Difficulty::Easy;
	}
	if (InDifficulty == "Medium")
	{
		return sc2::Difficulty::Medium;
	}
	if (InDifficulty == "MediumHard")
	{
		return sc2::Difficulty::MediumHard;
	}
	if (InDifficulty == "Hard")
	{
		return sc2::Difficulty::Hard;
	}
	if (InDifficulty == "HardVeryHard")
	{
		return sc2::Difficulty::HardVeryHard;
	}
	if (InDifficulty == "VeryHard")
	{
		return sc2::Difficulty::VeryHard;
	}
	if (InDifficulty == "CheatVision")
	{
		return sc2::Difficulty::CheatVision;
	}
	if (InDifficulty == "CheatMoney")
	{
		return sc2::Difficulty::CheatMoney;
	}
	if (InDifficulty == "CheatInsane")
	{
		return sc2::Difficulty::CheatInsane;
	}

	return sc2::Difficulty::Easy;
}
static std::string GetDifficultyString(sc2::Difficulty InDifficulty)
{
	switch (InDifficulty)
	{
	case sc2::Difficulty::VeryEasy:
	{
		return "VeryEasy";
	}
	case sc2::Difficulty::Easy:
	{
		return "Easy";
	}
	case sc2::Difficulty::Medium:
	{
		return "Medium";
	}
	case sc2::Difficulty::MediumHard:
	{
		return "MediumHard";
	}
	case sc2::Difficulty::Hard:
	{
		return "Hard";
	}
	case sc2::Difficulty::HardVeryHard:
	{
		return "HardVeryHard";
	}
	case sc2::Difficulty::VeryHard:
	{
		return "VeryHard";
	}
	case sc2::Difficulty::CheatVision:
	{
		return "CheatVision";
	}
	case sc2::Difficulty::CheatMoney:
	{
		return "CheatMoney";
	}
	case sc2::Difficulty::CheatInsane:
	{
		return "CheatInsane";
	}
	}
	return "Easy";
}

static std::string GetResultType(ResultType InResultType)
{
	switch (InResultType)
	{
	case InitializationError:
		return "InitializationError";

	case Timeout:
		return "Timeout";

	case ProcessingReplay:
		return "ProcessingReplay";

	case Player1Win:
		return "Player1Win";

	case Player1Crash:
		return "Player1Crash";

	case Player2Win:
		return "Player2Win";

	case Player2Crash:
		return "Player2Crash";

	case Tie:
		return "Tie";

	default:
		return "Error";
	}
}

static std::string RemoveMapExtension(const std::string& filename)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos)
    {
        return filename;
    }
    return filename.substr(0, lastdot);
}
