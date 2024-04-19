#include <iostream>
#include <map>
#include <ctime>
#include <cstring>
struct SensorData23
{
    int id;
    char time[20];
    float value;
};

struct SensorAverage
{
    std::tm Start_time {};
    std::tm End_time {};
};

struct SummarysensorData
{
    float maxvalue = 0;
    char maxtime[20];
    float minvalue = 30000.00;
    char mintime[20];
    float Value=0;
    int count=0;
    std::string simulationtime;
};


/*Error function
   Input:     Error report file(.log), Error code, Error description
   Output:    Error [error code] : [description] (in file task2.log) */
void LogError23(const char* logFile, int errorCode, const char* description) 
{
    FILE* ErrorFile = fopen(logFile, "a");
    if (ErrorFile == nullptr) 
    {
        std::cerr << "Error opening log file.\n";
        return;
    }
    fprintf(ErrorFile, "Error %02d: %s\n", errorCode, description);
    fclose(ErrorFile);
}


/*Find min - max - total value of sensor during Simulation time in lux_inliner.csv file
  Input:     +lux_inliner.csv(output file of task 2.1)
  Output:    +map key : sensor id
             +map value: max value-time; min value-time; total value; number simulation*/
void Find_min_max_value (const char* lux_inliner_file, std::map<int, SummarysensorData>& SensorSummary)
{
    SensorData23 sensor;
    FILE *lux_inliner = fopen(lux_inliner_file, "r");
    if (lux_inliner == nullptr)
    {
        std::cerr << "Error: Can't open file" << lux_inliner_file << std::endl;
        return;
    }
    fscanf(lux_inliner, "%*[^\n]\n");
    while (fscanf(lux_inliner, "%d,%19[^,],%f\n", &sensor.id, sensor.time, &sensor.value) == 3)
    {
        auto i = SensorSummary.find(sensor.id);
        if(i != SensorSummary.end())
        {
            if(sensor.value < i->second.minvalue)
            {
                i->second.minvalue = sensor.value;
                strcpy(i->second.mintime, sensor.time);
            }
            if(sensor.value > i->second.maxvalue)
            {
                i->second.maxvalue = sensor.value;
                strcpy(i->second.maxtime, sensor.time);
            }
            i->second.Value += sensor.value;
            i->second.count++;            
        }
    }
    fclose(lux_inliner);
}


/*Take a string time and transfer to std::tm
  Input:     String time
  Output:    std::tm
  Assumes:   The input string follow the rule y:m:d H:M:S*/
void Take_time(const char* strtime, std::tm &time)
{
        sscanf(strtime, "%d:%d:%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
        time.tm_year -= 1900; 
        time.tm_mon -= 1;    
        return;
}


/*Caculate Average value during simulation time lux_inliner.csv file, total time simulation in lux_sensor.csv file
  Input:     +lux_inliner.csv(output file of task 2.1)
             +lux_sensor.csv file (output of task 1)
  Output:    lux_summary.csv*/
inline void Create_Summary_Sensor(const char* lux_inliner_file, const char* lux_sensor_file)
{
    std::map<int, SummarysensorData> SensorSummary;
    std::map<int, SensorAverage> averageSensor;
    FILE *lux_sensor = fopen(lux_sensor_file, "r");
    FILE *lux_summary = fopen("lux_summary.csv", "w");
    /*Check whether the output file can be created or overwritten*/
    if (lux_summary== nullptr) 
    {
        std::cerr << "Error: Cannot create or override " << lux_summary<< std::endl;
        LogError21("task2.log", 06, "cannot create or override output file");
        fclose(lux_summary);
        fclose(lux_sensor);
        return;
    }
    SensorData23 sensor;
    /*We have to caculate the all time simulate using lux_sensor.csv because after task 2.1 some data has been removed*/
    fscanf(lux_sensor, "%*[^\n]\n");
    while (fscanf(lux_sensor, "%d,%19[^,],%f\n", &sensor.id, sensor.time, &sensor.value) == 3)
    {
        if(averageSensor[sensor.id].Start_time.tm_year == 0)
        {
            Take_time(sensor.time, averageSensor[sensor.id].Start_time);                       
        }
        Take_time(sensor.time, averageSensor[sensor.id].End_time);        
    }
    for (const auto& key :averageSensor)
    {
        std::tm start = key.second.Start_time;
        std::tm end = key.second.End_time;
        std::time_t simulationtime = std::mktime(&end) - std::mktime(&start);
        int hours = simulationtime / 3600;
        int minutes = (simulationtime % 3600) / 60;
        int seconds = simulationtime % 60;
        std::string formattedTime;
        /*transfer simulation total time to hour*/
        if (minutes == 0 && seconds == 0) 
        {
            formattedTime = std::to_string(hours) + ":00:00";
        } 
        else if (minutes == 0) 
        {
            formattedTime = std::to_string(hours) + ":00:" + std::to_string(seconds);
        } 
        else if (seconds == 0) 
        {
            formattedTime = std::to_string(hours) + ":" + std::to_string(minutes) + ":00";
        } 
        else 
        {
            formattedTime = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
        }
        SensorSummary[key.first].simulationtime = formattedTime;    
    }


    //fprintf result
    fprintf(lux_summary, "id, parameter,time,value\n");
    Find_min_max_value(lux_inliner_file, SensorSummary);
    for (auto& key :SensorSummary)
    {
        key.second.Value = key.second.Value / key.second.count;
        fprintf(lux_summary, "%d,max,%s,%.2f\n", key.first, key.second.maxtime,key.second.maxvalue);
        fprintf(lux_summary, "%d,min,%s,%.2f\n", key.first, key.second.mintime, key.second.minvalue);
        fprintf(lux_summary, "%d,mean,%s,%.2f\n", key.first, key.second.simulationtime.c_str(),key.second.Value);
    }
    fclose(lux_sensor);
    fclose(lux_summary);
    std::cout<<"The sensor summary have been saved in file : lux_summary.csv\n";
}