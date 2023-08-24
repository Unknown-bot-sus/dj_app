/*
  ==============================================================================

    FileReader.h
    Created: 12 Mar 2023 10:44:30pm
    Author:  tls

  ==============================================================================
*/

#pragma once
#include <vector>
#include <string>
#include <fstream>

class FileReader {
  public:
    /** read file line by line and return a vector of strings*/
    static std::vector<std::string> read(std::string filePath);
    /** write to file from a vector of strings*/
    static void write (std::string filePath, std::vector<std::string>& lines);
};