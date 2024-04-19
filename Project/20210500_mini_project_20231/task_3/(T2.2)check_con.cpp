#include <iostream>
#include <cstring>
#include <map>
#include <ctime>


//Struct of min max data in location (read in location_min_max.csv)
struct LuxRange 
{
    int minLux;
    int maxLux;
};
//Struct of average value in 1 hour of 1 sensor
struct SensorHourData
{
    float AverageValue =0;
    int count =0;
};
//Struct sensor data in Task 2.1 and Task3
struct SensorData 
{
    int id;
    char time[20];
    float value;
    int location;
    char condition[7];
};


 /*Error function
   Input:     Error report file(.log), Error code, Error description
   Output:    Error [error code] : [description] (in file task2.log) */
inline void LogError22(const char* logFile, int errorCode, const char* description) 
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


/*Read Min - Max lux range from location_min_max.csv
  Input:     Location min max file (.csv)
  Output:    map <key: locationid; value: struct - luxrange>
  Assumes:   Must have location_min_max.csv where header is (id,min,max)*/
std::map<int, LuxRange> minMaxLux;
inline void readMinMaxLux(const char* location_min_max_file, std::map<int,LuxRange>& minMaxLux) 
{
    FILE* file = fopen(location_min_max_file, "r");
    if (file == nullptr) 
    {
        std::cerr << "Error: Cannot open or missing " << location_min_max_file << std::endl;
        return;
    }    
    int id, min, max;
    fscanf(file, "%*[^\n]\n");    
    while (fscanf(file, "%d,%d,%d\n", &id, &min, &max) == 3) 
    {
        minMaxLux[id]={min,max};
    }
    fclose(file);
}


/*Read Sensors location from location_test.csv
  Input:     Location test file (.csv)
  Output:    map<key: sensorid; value: locationid of sensor>
  Assumes:   the input file has header: id,location*/
void readSensorLocations(const char* location_test_file, std::map<int, int>& sensorLocations) {
    FILE* file = fopen(location_test_file, "r");
    /*Check whether the input file is accessible or exists*/
    if (file == nullptr) 
    {
        std::cerr << "Error: Cannot open :"<< file <<std::endl;
        LogError22("task2.log", 01, "input file not found or not accessible");
        return;
    }
    int sensorId, locationId;
    const char* expectedHeader = "id,location\n";
    char line[64];
    if (fgets(line, sizeof(line), file)) 
    {
        /*Check if input file have valid header (break)*/
        if (strcmp(line, expectedHeader) != 0) 
        {
            LogError22("task2.log", 02, "invalid input file format");
            fclose(file);
            return;
        }
    }
    while (fgets(line, sizeof(line), file)) 
    {
        sscanf(line, "%d,%d\n", &sensorId, &locationId);
        sensorLocations[sensorId] = locationId;
    }
    fclose(file);
}


/*Determine the condition of sensor
  Input:     +struct SensorData that already have id,location
             +map <key: locationid; value: struct - luxrange>
  Output:    +sensor condition*/
inline void determineCondition(SensorData &sensor,const std::map<int, LuxRange> minMaxLux)
{
    /*Find lux range of sensor use locationid*/
    auto i = minMaxLux.find(sensor.location);
    if (i != minMaxLux.end()) 
    {
        if (sensor.value < i->second.minLux) 
        {
            strcpy(sensor.condition, "dark");
        } 
        else if (sensor.value > i->second.maxLux) 
        {
            strcpy(sensor.condition, "bright");
        } 
        else 
        {
            strcpy(sensor.condition, "good");
        }
    }
    else
    {       
        strcpy(sensor.condition, "NA");
    }
}


/*Caculate the average lux value in 1 hour and consider condition
  Input:     +lux_inliner.csv(output file of task 2.1)
             +location of sensor test file
  Output:    +Average value of sensor in 1 hour
             +Condition of sensor*/
inline void Check_Condition_Sensor(const char* lux_inliner_file, const char* location_test_file)
{
    std::map<int, int> sensorLocations;
    readSensorLocations(location_test_file, sensorLocations);
    readMinMaxLux("location_min_max.csv", minMaxLux);
    FILE *inliner = fopen(lux_inliner_file, "r");
    FILE *lux_condition = fopen("lux_condition.csv", "w");
    /*Check whether the output file can be created or overwritten*/
    if (lux_condition == nullptr) 
    {
        std::cerr << "Error: Cannot create or override " << lux_condition<< std::endl;
        LogError22("task2.log", 06, "cannot create or override output file");
        fclose(inliner);
        fclose(lux_condition);
        return;
    }
    SensorData sensor;

    /*Caculate the average lux value in 1 hour
      Detail of this function in report.docx*/ 
    std::map<std::map<std::string,int>, SensorHourData> SensorHourData;
    char averageTime[20];
    fscanf(inliner, "%*[^\n]\n");
    while (fscanf(inliner, "%d,%19[^,],%f\n", &sensor.id, sensor.time, &sensor.value) == 3)
    {
        std::tm time;
        sscanf(sensor.time, "%d:%d:%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
        time.tm_year -= 1900; 
        time.tm_mon -= 1;    
        time.tm_hour++;
        mktime(&time);
        strftime(averageTime, sizeof(averageTime), "%Y:%m:%d %H:00:00", &time);
        std::map<std::string,int> key;
        key[std::string(averageTime)] = sensor.id;      
        SensorHourData[key].AverageValue += sensor.value;
        SensorHourData[key].count++;
    }

    //fprint result
    fprintf(lux_condition, "id,time,location,value,condition\n");
    for (const auto& key1 : SensorHourData)
    {
        for(const auto& key2 : key1.first)
        {
            sensor.id = key2.second;
            strcpy(sensor.time, key2.first.c_str());
            sensor.value = key1.second.AverageValue / key1.second.count;
            auto i = sensorLocations.find(sensor.id);
            if (i != sensorLocations.end())
            {
                sensor.location = i->second;
            }
            else
            {
                sensor.location = 0;
            }
            determineCondition(sensor, minMaxLux);
            fprintf(lux_condition, "%d,%s,%d,%.2f,%s\n", sensor.id, sensor.time, sensor.location, sensor.value, sensor.condition);
        }
    }
    fclose(inliner);
    fclose(lux_condition);
    std::cout<<"The sensor condition and average values calculated over an hour are saved in the file : lux_condition.csv"<<std::endl;
}