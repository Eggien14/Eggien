#include <iostream>
#include <map>
#include "(T2.1)check_valid.cpp"
#include "(T2.2)check_con.cpp"
#include "(T2.3)lux_summary.cpp"


/*Check sensor don't have location
  Input:     +lux_sensor.csv(output of task 1)
             +location_test.csv
  Output:    +Error report in file task2.log*/
void Check_Sensor_Location(const char* lux_sensor_file, const char* location_test_file)
{
    std::map<int, bool> sensor;
    FILE *sensorfile = fopen(lux_sensor_file, "r");
    FILE *locationfile = fopen(location_test_file, "r");
    int id;
    char line[64];
    fscanf(sensorfile, "%*[^\n]\n");
    for(int maxsensornumber = 1; maxsensornumber < 50; ++maxsensornumber)    
    {
        fgets(line, sizeof(line), sensorfile);
        sscanf(line, "%d,%*[^\n]\n", &id); 
        sensor[id] = false;
    }
    fscanf(locationfile, "%*[^\n]\n");  
    while (fscanf(locationfile, "%d,%*[^\n]\n", &id) == 1) 
    {
        sensor[id] = true;
    }
    for (const auto& key : sensor) 
    {
        if (!key.second) {
            char errorMsg[100];
            sprintf(errorMsg, "unknown location of sensor %d", key.first); 
            LogError22("task2.log", 05, errorMsg);            
        }
    }
    fclose(sensorfile);
    fclose(locationfile);        
}


//Check input file is .csv file or not
bool isCSV(const char* filename) 
{
    int length = strlen(filename);
    return strcmp(filename + length - 4, ".csv") == 0;
}


//main
int main(int argc, char *argv[]) 
{
    /*Check input data is valid command*/
    if(argc !=3 || !isCSV(argv[1]) || !isCSV(argv[2]))
    {
        std::cerr << "Usage: lux_process [data_filename.csv] [location.csv].\n";
        LogError22("task2.log", 03, "invalid command");
        return 1;        
    }


    /*Task2.1*/
    Check_Valid_Sensors(argv[1]);

    
    /*Task2.2*/
    Check_Condition_Sensor("lux_inliner.csv", argv[2]);
    Check_Sensor_Location(argv[1], argv[2]);


    /*Task2.3*/
    Create_Summary_Sensor("lux_inliner.csv", argv[1]);
    return 0;
}