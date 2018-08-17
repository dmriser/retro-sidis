#ifndef CONFIG_CXX 
#define CONFIG_CXX 

/*

  Author: David Riser 
  Date:   August 17, 2018 

  This code manages the configuration needed
  to run the analysis code mysidis.C.  A simple 
  input format is prescribed, please look at the 
  example configuration file example.conf. 

*/

struct RunConfiguration {
  std::string inputFileList;
  int numberOfFiles, acceptanceIteration, strictToChange, strictness;
  bool correctElectronP, correctPionP, changeStrictness, expOrSim;
};

std::pair<std::string, std::string> parseConfigurationEntry(std::string inputString, char splitToken){

  // Determine where to split the input string
  unsigned int splitIndex = 0;
  for (int index = 0; index < inputString.size(); index++){
    if (inputString.at(index) == splitToken){
      splitIndex = index;
      break;
    }
  }

  // Setup proper place to hold results
  std::string key;
  std::string value;

  // Split string
  for (int index = 0; index < splitIndex; index++){
    key.push_back(inputString.at(index));
  }

  // Skip the split index
  for (int index = splitIndex + 1; index < inputString.size(); index++){
    value.push_back(inputString.at(index));
  }

  return std::make_pair(key, value);
}

RunConfiguration loadRunConfiguration(std::string configurationFilename){

  std::ifstream configFile(configurationFilename);
  
  RunConfiguration runConfig; 
  if ( configFile.is_open() ){

    std::string currentLine;
    while( getline(configFile, currentLine) ){
      if (currentLine.front() != '#'){
	std::pair<std::string, std::string> configEntry = parseConfigurationEntry(currentLine, ' ');

	// String options 
	if (configEntry.first == "inputFileList"){
	  runConfig.inputFileList = configEntry.second; 
	}
	
	// Integer options 
	else if (configEntry.first == "numberOfFiles"){
	  runConfig.numberOfFiles = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "acceptanceIteration"){
	  runConfig.acceptanceIteration = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "strictToChange"){
	  runConfig.strictToChange = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "strictness"){
	  runConfig.strictness = atoi(configEntry.second.c_str());
	}
	
	// Boolean options (casted as integers)
	else if (configEntry.first == "expOrSim"){
	  runConfig.expOrSim = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "correctElectronP"){
	  runConfig.correctElectronP = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "correctPionP"){
	  runConfig.correctPionP = atoi(configEntry.second.c_str());
	}

	else if (configEntry.first == "changeStrictness"){
	  runConfig.changeStrictness = atoi(configEntry.second.c_str());
	}
	
	else {
	  std::cout << "Error parsing configuration option: " << configEntry.first << std::endl; 
	  std::cout << "If this is new, please add it to the loadConfguration method in configuration.C" << std::endl;
	}

      }
    }

  } else {
    std::cerr << "[loadRunConfiguration] Fatal: Configuration file not opened." << std::endl;
    return runConfig; 
  }

  return runConfig; 
}

#endif 
