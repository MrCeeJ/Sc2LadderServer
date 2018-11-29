#pragma once

#include <string>
#include "Types.h"

void StartBotProcess(const BotConfig &Agent, const std::string& CommandLine, unsigned long *ProcessId);

void SleepFor(int seconds);

void KillBotProcess(unsigned long pid);

bool MoveReplayFile(const char* lpExistingFileName, const char* lpNewFileName);

void StartExternalProcess(const std::string CommandLine);

std::string PerformRestRequest(const std::string &location, const std::vector<std::string> &arguments);

bool ZipArchive(const std::string &InDirectory, const std::string &OutArchive);

bool UnzipArchive(const std::string &InArchive, const std::string &OutDirectory);
