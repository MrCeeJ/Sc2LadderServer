#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

#include "sc2utils/sc2_manage_process.h"
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "LadderManager.h"

const static char *DLLDir = "e:\\sc2Dll\\";

//*************************************************************************************************
void LadderManager::StartGame(AgentInfo Agent1, AgentInfo Agent2) {
	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(CoordinatorArgc, CoordinatorArgv)) {
		return;
	}

	// Add the custom bot, it will control the players.

	coordinator.SetParticipants({
		CreateParticipant(sc2::Race::Random, Agent1.Agent),
		CreateParticipant(sc2::Race::Random, Agent2.Agent),
	});

	// Start the game.
	coordinator.LaunchStarcraft();

	// Step forward the game simulation.
	bool do_break = false;
	while (!do_break) {
		coordinator.StartGame(sc2::kMapBelShirVestigeLE);
		while (coordinator.Update() && !do_break) {
			if (sc2::PollKeyPress()) {
				do_break = true;
			}
		}
	}
}

LadderManager::LadderManager(int InCoordinatorArgc, char** inCoordinatorArgv, const char *InDllDirectory)
	:CoordinatorArgc(InCoordinatorArgc)
	, CoordinatorArgv(inCoordinatorArgv)
{
	std::string inputFolderPath = InDllDirectory;
	std::string extension = "*.dll*";
	std::vector<std::string> filesPaths;
	getFilesList(inputFolderPath, extension, filesPaths);
	std::vector<std::string>::const_iterator it = filesPaths.begin();

	while (it != filesPaths.end())
	{
		HINSTANCE hGetProcIDDLL = LoadLibrary(it->c_str());
		if (hGetProcIDDLL) {
			// resolve function address here
			GetAgentFunction GetAgent = (GetAgentFunction)GetProcAddress(hGetProcIDDLL, "?CreateNewAgent@@YAPEAXXZ");
			GetAgentNameFunction GetAgentName = (GetAgentNameFunction)GetProcAddress(hGetProcIDDLL, "?GetAgentName@@YAPEBDXZ");
			if (GetAgent && GetAgentName) {
				sc2::Agent *NewAgent = (sc2::Agent *)(GetAgent());
				const char *AgentName = GetAgentName();
				if (NewAgent && AgentName)
				{
					AgentInfo NewAgentInfo(NewAgent, std::string(AgentName), *it);
				}
			}
		}
		it++;
	}

}

void LadderManager::getFilesList(std::string filePath, std::string extension, std::vector<std::string> & returnFileName)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	std::string  fullPath = filePath + extension;
	hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE) {
		returnFileName.push_back(filePath + fileInfo.cFileName);
		while (FindNextFile(hFind, &fileInfo) != 0) {
			returnFileName.push_back(filePath + fileInfo.cFileName);
		}
	}
}

int main(int argc, char** argv)
{
	LadderMan = new LadderManager(argc, argv, DLLDir);
}