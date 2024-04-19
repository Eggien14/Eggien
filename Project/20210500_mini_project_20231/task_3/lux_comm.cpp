#include <iostream>
#include <cstring>
#include <map>
#include <ctime>
#include "(T2.2)check_con.cpp"

//Sensor Data is type of data in .csv file (in (T2.2)check_con.cpp)

//CommunicationPacket is type of data in .dat file
struct CommunicationPacket 
{
    unsigned char startByte;
    unsigned char packetLength;
    unsigned char id;
    unsigned char time[4];
    unsigned char location;
    unsigned char lux[4];
    unsigned char checksum;
    unsigned char stopByte;
};


/*Error function
   Input:     Error report file(.log), Error code, Error description
   Output:    Error [error code] : [description] (in file task3.log) */
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
/*Check if input time is valid time (y:m:d H:M:S)
  Input:     A string of time
  Output     A bool that return TRUE if the string follow the rule y:m:d H:M:S or FALSE for else)*/
bool isValidTime(const char* timeStr) 
{
    std::tm tm = {};
    return sscanf(timeStr, "%d:%d:%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6;
}
/*Check if Data is preserved when transmitted
  Input:     packet data
  Output     A bool that return TRUE if Data is preserved (FALSE for else)*/
bool Validchecksum(CommunicationPacket* Packet)
{
    unsigned char sum = Packet->packetLength + Packet->id + Packet->location;
    for (int i = 0; i < 4; ++i) {
        sum += Packet->time[i];
        sum += Packet->lux[i];
    }
    unsigned char testsum = ~sum +1 & 0xFF;
    return (testsum == Packet->checksum);
}
/*Check if input time hex is valid time
  Input:     packet data
  Output     A bool that return FALSE if hex time is future (TRUE for else)*/
bool isFuture(unsigned char* timeHex)
{
    std::time_t time = 0;
    for (int i = 0; i < 4; ++i) {
        time = (time << 8) | timeHex[i];
    }
    std::time_t now = std::time(nullptr);
    return time < now;    
}


//Convert .csv type data to .dat file data
//Convert time to hex
void Time2Hex (const char input_time[20], CommunicationPacket& Packet)
{
    std::tm time = {};
    sscanf(input_time, "%d:%d:%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900; 
    time.tm_mon -= 1;
    std::time_t totaltime = std::mktime(&time);
    for (int i = 0; i < 4; ++i) 
    {
        Packet.time[i] = (totaltime >> (24 - i * 8)) & 0xFF;
    }
}
//Convert lux to hex
void Lux2Hex (float input_lux, CommunicationPacket& Packet)
{
    unsigned char hex_lux[4];
    std::memcpy(hex_lux, &input_lux, sizeof(input_lux));
    for (int i = 0; i < 4; ++i) 
    {
        Packet.lux[i] = hex_lux[3 - i];
    }   
}
//Caculate checksum
void CaculateChecksum (CommunicationPacket& Packet)
{
    unsigned char sum = Packet.packetLength + Packet.id + Packet.location;
    for (int i = 0; i < 4; ++i) 
    {
        sum += Packet.time[i];
        sum += Packet.lux[i];
    }
    Packet.checksum = ~sum +1 & 0xFF;    
}
//Convert SensorData to CommunicationPacket
CommunicationPacket csv2dat (SensorData Sensor)
{
    CommunicationPacket Packet;
    Packet.startByte = 0xa0;
    Packet.id = static_cast<unsigned char>(Sensor.id);
    Time2Hex(Sensor.time, Packet);
    Packet.location = static_cast<unsigned char>(Sensor.location);
    Lux2Hex(Sensor.value, Packet);
    CaculateChecksum(Packet);
    Packet.stopByte = 0xa9;
    Packet.packetLength = sizeof(Packet.startByte) + sizeof(Packet.packetLength) + sizeof(Packet.id) + sizeof(Packet.time) + sizeof(Packet.location) + sizeof(Packet.lux) + sizeof(Packet.checksum) + sizeof(Packet.stopByte);
    return Packet;   
}


//Convert .dat type data to .csv type data
//Convert hex to int
int hex2int(const unsigned char* input_hex, int length_of_byte) 
{
    int value = 0;
    for (int i = 0; i < length_of_byte; i++) 
    {
        value = (value << 8) | input_hex[i];
    }
    return value;
}
//Convert hex to time
void hex2time(const unsigned char* input_hex, SensorData& Sensor)
{
    std::time_t totaltime = static_cast<std::time_t>(hex2int(input_hex, 4));
    std::tm* time = std::localtime(&totaltime);
    strftime(Sensor.time, sizeof(Sensor.time), "%Y:%m:%d %H:%M:%S", time);
}
//Convert hex to value
void hex2float(const unsigned char* input_hex, SensorData& Sensor)
{
    unsigned char reversed[4];
    for (int i = 0; i < 4; ++i) 
    {
        reversed[i] = input_hex[3 - i];
    }
    std::memcpy(&Sensor.value, reversed, sizeof(Sensor.value));
}
//Convert CommunicationPacket to SensorData
SensorData dat2csv (CommunicationPacket Packet)
{
    SensorData Sensor;
    Sensor.id = hex2int(&Packet.id, 1);
    hex2time(Packet.time, Sensor);
    hex2float(Packet.lux, Sensor);
    Sensor.location= hex2int(&Packet.location, 1);
    //Determine Condition of Sensor
    readMinMaxLux("location_min_max.csv", minMaxLux);
    determineCondition(Sensor, minMaxLux);
    return Sensor;
}


//Case 3.1: If the input file is a data file with the same format as the file described in task 2.2
/*Check if input file is csv
  Input:     file name
  Output     A bool that return TRUE if input file is csv (FALSE for else)*/
bool isCSV(const char* filename) 
{
    int length = strlen(filename);
    return strcmp(filename + length - 4, ".csv") == 0;
}
void Read_csv_and_write_to_dat(const char* inputcsvFilename, const char* outputdatFilename) 
{ 
    FILE* inputcsvFile = fopen(inputcsvFilename, "r");
    FILE* outputdatFile = fopen(outputdatFilename, "w");
    /*Check whether the input file is accessible or exists*/
    if (inputcsvFile == nullptr) 
    {
        std::cerr << "Error: Cannot open :"<< inputcsvFilename <<std::endl;
        LogError("task3.log", 01, "input file not found or not accessible");
        return;
    }
    /*Check whether the output file can be created or overwritten*/
    if (outputdatFile == nullptr) 
    {
        std::cerr << "Error: Cannot create or override " << outputdatFilename << std::endl;
        LogError("task3.log", 07, "cannot create or override output file");
        fclose(inputcsvFile);
        return;
    }
    SensorData sensor;
    const char* expectedHeader = "id,time,location,value,condition\n";
    char line[64];
    int lineCount = 0;
    if (fgets(line, sizeof(line), inputcsvFile)) 
    {
        /*Check if input file have valid header (break)*/
        if (strcmp(line, expectedHeader) != 0) 
        {
            LogError("task3.log", 02, "invalid input file format");
            fclose(inputcsvFile);
            fclose(outputdatFile);
            return;
        }
    }
    while (fgets(line, sizeof(line), inputcsvFile)) 
    {
        lineCount++;
        int scanResult = sscanf(line, "%d,%19[^,],%d,%f,%19s", &sensor.id, sensor.time, &sensor.location, &sensor.value, sensor.condition);
        /*Check if input data is valid (continue)*/
        if (scanResult != 5 || sensor.id <= 0 || !isValidTime(sensor.time)) 
        {
            char errorMsg[100];
            sprintf(errorMsg, "invalid data at line %d", lineCount);
            LogError("task3.log", 04, errorMsg);
            continue; 
        }
        if (scanResult == 5)
        {
            CommunicationPacket packet = csv2dat(sensor);
            fprintf(outputdatFile, "%02X %02X %02X ", packet.startByte, packet.packetLength, packet.id);
            for (int i = 0; i < 4; ++i) fprintf(outputdatFile, "%02X ", packet.time[i]);
            fprintf(outputdatFile, "%02X ", packet.location);
            for (int i = 0; i < 4; ++i) fprintf(outputdatFile, "%02X ", packet.lux[i]);
            fprintf(outputdatFile, "%02X %02X\n", packet.checksum, packet.stopByte);
        }
    }
    fclose(inputcsvFile);
    fclose(outputdatFile);
}


//Case 3.2: If the input file is a text file with the extension “.dat”
bool isDAT(const char* input_file) 
{
    int length = strlen(input_file);
    return strcmp(input_file + length - 4, ".dat") == 0;
}
void Read_dat_and_write_to_csv(const char* inputdatFilename, const char* outputcsvFilename)
{
    FILE* inputdatFile = fopen(inputdatFilename, "r");
    FILE* outputcsvFile = fopen(outputcsvFilename, "w");
    /*Check whether the input file is accessible or exists*/
    if (inputdatFile == nullptr) 
    {
        std::cerr << "Error: Cannot open :\n"<< inputdatFilename <<std::endl;
        LogError("task3.log", 01, "input file not found or not accessible");
        return;
    }
    /*Check whether the output file can be created or overwritten*/
    if (outputcsvFile == nullptr) 
    {
        std::cerr << "Error: Cannot create or override " << outputcsvFilename << std::endl;
        LogError("task3.log", 07, "cannot create or override output file");
        fclose(inputdatFile);
        return;
    }
    CommunicationPacket packet;
    char line[64];
    int lineCount =0;
    fprintf(outputcsvFile, "id,time,location,value,condition\n");
    while (fgets(line, sizeof(line), inputdatFile)) 
    {
        lineCount++;
        int scanResult = sscanf(line, "%hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX",
                  &packet.startByte, 
                  &packet.packetLength, 
                  &packet.id, 
                  &packet.time[0], &packet.time[1], &packet.time[2], &packet.time[3],
                  &packet.location, 
                  &packet.lux[0], &packet.lux[1], &packet.lux[2], &packet.lux[3], 
                  &packet.checksum, 
                  &packet.stopByte);
        /*Check if input data is valid (continue)*/
        if (scanResult != 14 || packet.startByte != 0xA0 || packet.stopByte != 0xA9 ||packet.packetLength != 14 || !Validchecksum(&packet) || !isFuture(packet.time)) 
        {
            char errorMsg[100];
            sprintf(errorMsg, "data packet error at line %d", lineCount);
            LogError("task3.log", 05, errorMsg);
            continue;
        }
        if (scanResult == 14)
        {
            SensorData sensor = dat2csv(packet);
            fprintf(outputcsvFile, "%d,%s,%d,%.2f,%s\n",sensor.id, sensor.time, sensor.location, sensor.value, sensor.condition);
        }
    } 
    fclose(inputdatFile);
    fclose(outputcsvFile);
}


//main
int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        /*Check input data is valid command*/
        std::cerr << "Usage: lux_comm [input_file] [output_file].\n";
        LogError("task3.log", 03, "invalid command");
        return 1;
    }    
    if (isCSV(argv[1])) 
    {
        Read_csv_and_write_to_dat(argv[1], argv[2]);
    } 
    else if (isDAT(argv[1])) 
    {
        Read_dat_and_write_to_csv(argv[1], argv[2]);
    } 
    else 
    {
        /*Check input data is valid file*/
        std::cerr << "Unknown file format.\n";
        LogError("task3.log", 02, "invalid input file format");
        return 1;
    }
    return 0;
}