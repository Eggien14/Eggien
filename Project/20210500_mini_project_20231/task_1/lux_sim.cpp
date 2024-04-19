#include <iostream>
#include <cstring>
#include <ctime>
 /*Error function
   Input:     Error report file(.log), Error code, Error description
   Output:    Error [error code] : [description] (in file task1.log) */
void LogError(const char* logFile, int errorCode, const char* description) 
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


/*Create Random Value from 0.01 - 40000.00
  Input:  
  Output: [random] value (0.01 - 40000.00)*/
 float RandomValue()
 {
    /*rand()create random interger value from 0 - 32767 so we use rand()/(32767 / 39999.90) to create random float value from 0.00 - 39999.90 */
    float value = static_cast<float>(rand() / (32767 / 39999.90));
    value = value +0.01;
    return value;
 }


/*Create simulation sensor
  Input:     number_sensor, sampling_time, interval_time
  Output:    [id],[time],[value] (in file lux_sensor.csv)\
  Assumes:   number_sensor, sampling_time, interval_time is nonnegative*/
 void Create_Simulate_Light_Sensor(int num_sensors, int sampling, int interval)
 {
    std::cout<<"Number of sen sensors  : "<<num_sensors<<" sensors"<<std::endl;
    std::cout<<"Sample collection time : "<<sampling<<" seconds"<<std::endl;
    std::cout<<"Simulation time period : "<<interval<<" hours"<<std::endl;
    FILE *file = fopen("lux_sensor.csv", "w");
    /*Check whether the output file can be created or overwritten*/
    if (file == nullptr)
    {
        std::cerr << "Error: Cannot create or override " << file << std::endl;
        LogError("task1.log", 03, "cannot create or override output file");
        return;
    }
    fprintf(file, "id,time,value\n");
    /*We create the first time simulation is[Start_time] [now] - [interval time]*/
    time_t Start_time = time(nullptr);
    Start_time -= interval*3600;
    /*For every loop, we check [time_period] < [interval_time] so [simulated_time]=[Start_time] + [time_period]*/
    for(int Time_Period = 0; Time_Period <= interval*3600; Time_Period +=sampling)
    {
        time_t SimulatedTime = Start_time + Time_Period;
        tm *SimulatedTime_GTM7 = localtime(&SimulatedTime);
        /*For every loop, we create [sensor_id] <= [number_sensor]*/
        for(int Sensorid = 1; Sensorid <= num_sensors; Sensorid++)
        {
            float value = RandomValue();
            fprintf(file, "%d,%04d:%02d:%02d %02d:%02d:%02d,%.2f\n",
                    Sensorid,
                    SimulatedTime_GTM7->tm_year+1900,
                    SimulatedTime_GTM7->tm_mon+1,
                    SimulatedTime_GTM7->tm_mday,
                    SimulatedTime_GTM7->tm_hour,
                    SimulatedTime_GTM7->tm_min,
                    SimulatedTime_GTM7->tm_sec,
                    value);
        }
    }
    fclose(file);
    std::cout<<"The sensor data has been recorded into the file lux_sensor.csv\n";
    return;    
 }


//main
 int main(int argc, char *argv[])
 {
    int num_sensors = 1;
    int sampling = 60;
    int interval = 24;
    for (int i = 1; i < argc; i += 2) 
    {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) 
        {
            /*Next input data after [-n] will be number of sensor */
            num_sensors = atoi(argv[i + 1]);
        } 
        else if (strcmp(argv[i], "-st") == 0 && i + 1 < argc) 
        {
            /*Next input data after [-si] will be sampling time*/
            sampling = atoi(argv[i + 1]);
        } 
        else if (strcmp(argv[i], "-si") == 0 && i + 1 < argc) 
        {
            /*Next input data after [-st] will be interval time*/
            interval = atoi(argv[i + 1]);
        }
        else
        {
            std::cerr << "Usage : -n [num_sensors] -st [sampling] -si [interval].\n";
            LogError("task1.log", 01, "invalid command");
            return 1;
        }
    }
    /*Check input data is valid number*/
    if(num_sensors <= 0 || sampling <= 0 || interval <= 0)
    {
        std::cerr<<"The parameters must be positive integers";
        LogError("task1.log", 02, "invalid argument");
        return 1;
    }
    Create_Simulate_Light_Sensor(num_sensors, sampling, interval);
    return 0;
 }