#include <iostream>
#include <cstring>
#include <ctime>


//Struct sensor data in Task 2.1 and luxsensor csv file
struct SensorsData21
{
    int id;
    char time[20];
    float value;
};


 /*Error function
   Input:     Error report file(.log), Error code, Error description
   Output:    Error [error code] : [description] (in file task2.log) */
void LogError21(const char* logFile, int errorCode, const char* description) 
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
/*Check if input time is valid time (y:m:d H:M:S)
  Input:     A string of time
  Output     A bool that return TRUE if the string follow the rule y:m:d H:M:S or FALSE for else*/
bool isValidTime21(const char* timeStr) 
{
    std::tm tm = {};
    return sscanf(timeStr, "%d:%d:%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6;
}


/*Check that the input sensor value data is inliner(1.00 - 30000.00) or outliner(else) value
  Input:     lux_sensor.csv file (output of task 1)
  Output     + lux_inliner.csv:  The valid sensor data that have value inline
             + lux_outliner.csv: The valid sensor data that have value outliner
  Assumes:   + the input file has header: id,time,value
             + id is interger number
             + time is string follow the rule y:m:d H:M:S
             + value is nonnegative */
inline void Check_Valid_Sensors(const char *lux_sensor_file)
{
    FILE *lux_sensor, *Valid_sensors, *Invalid_sensors;
    int n_Outliner = 0;
    SensorsData21 sensor;

    /*First: read lux_sensor and count number of outliner sensor data*/
    lux_sensor = fopen(lux_sensor_file,"r");
    /*Check whether the input file is accessible or exists*/
    if (lux_sensor == nullptr) 
    {
        std::cerr << "Error: Cannot open :"<< lux_sensor_file <<std::endl;
        LogError21("task2.log", 01, "input file not found or not accessible");
        return;
    }
    fscanf(lux_sensor, "%*[^\n]\n");                                                          
    while(fscanf(lux_sensor, "%d,%19[^,],%f\n", &sensor.id, sensor.time, &sensor.value)==3)    
    {
        if(sensor.value<1.00 || sensor.value>30000.00)
        {
            n_Outliner++;
        }
    }
    fclose(lux_sensor);

    //Second: Copy Valid sensor to lux_inliner and Invalid sensor to lux_outliner
    lux_sensor = fopen(lux_sensor_file, "r");
    Valid_sensors = fopen("lux_inliner.csv","w");
    Invalid_sensors = fopen("lux_outliner.csv","w");
    /*Check whether the output file can be created or overwritten*/
    if(Valid_sensors == nullptr || Invalid_sensors == nullptr)
    {
        std::cerr << "Error: Cannot create or override " << Valid_sensors<<" or "<<Invalid_sensors<< std::endl;
        LogError21("task2.log", 06, "cannot create or override output file");
        fclose(lux_sensor);
        fclose(Valid_sensors);
        fclose(Invalid_sensors);
        return;
    }
    fprintf(Valid_sensors,"id,time,value\n");
    fprintf(Invalid_sensors,"number of outliner: %d\n",n_Outliner);
    fprintf(Invalid_sensors,"id,time,value\n");
    const char* expectedHeader = "id,time,value\n";
    char line[64];
    int lineCount = 0;
    if (fgets(line, sizeof(line), lux_sensor)) 
    {
        /*Check if input file have valid header (break)*/
        if (strcmp(line, expectedHeader) != 0) 
        {
            LogError21("task2.log", 02, "invalid input file format");
            fclose(lux_sensor);
            fclose(Valid_sensors);
            fclose(Invalid_sensors);
            return;
        }
    }
    while (fgets(line, sizeof(line), lux_sensor)) 
    {
        lineCount++;
        int scanResult = sscanf(line, "%d,%19[^,],%f\n", &sensor.id, sensor.time, &sensor.value);
        /*Check if input data is valid (continue)*/
        if(scanResult != 3 || sensor.id <= 0 || sensor.value < 0 || !isValidTime21(sensor.time))
        {
            char errorMsg[100];
            sprintf(errorMsg, "invalid data at line %d", lineCount);
            LogError21("task2.log", 04, errorMsg);
            continue;            
        }
        /*Check if input value is inliner or outliner*/
        if (sensor.value < 1.00 || sensor.value > 30000.00)
        {
            fprintf(Invalid_sensors, "%d,%s,%.2f\n", sensor.id, sensor.time, sensor.value);
        } 
        else 
        {
            fprintf(Valid_sensors, "%d,%s,%.2f\n", sensor.id, sensor.time, sensor.value);
        }
    }    
    fclose(lux_sensor);
    fclose(Valid_sensors);
    fclose(Invalid_sensors);
    std::cout<<"The valid values measured by the sensors are saved in the file: lux_inliner.csv\n";
    std::cout<<"The invalid values measured by the sensors are saved in the file: lux_outliner.csv\n";
}

