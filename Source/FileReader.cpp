/*
  ==============================================================================

    FileReader.cpp
    Created: 12 Mar 2023 10:44:30pm
    Author:  tls

  ==============================================================================
*/

#include "FileReader.h"

void FileReader::write (std::string filePath, std::vector<std::string>& lines)
{
  std::ofstream outfile {filePath};
  for (const std::string line: lines)
  {
    outfile << line << std::endl;
  }
}


std::vector<std::string> FileReader::read (std::string filePath)
{
  std::ifstream file {filePath};
  std::string line;
  std::vector<std::string> lines;

  if (file.is_open()) 
  {
    while (getline(file, line))
    {
      lines.push_back(line);
    } 
  }

  file.close();

  return lines;
}