#include "Octopi.h"
#include "Utils.h"

void Octopi::Update(Entities& entities) {
    printStatus = entities.Take<std::string>("sensor.octoprint_print_status");

    // To stop later operations from failing
    if (printStatus == "unavailable" || printStatus == "Offline")
        return;

    printFile = entities.Take<std::string>("sensor.octoprint_print_file");
    printProgress = entities.Take<int>("sensor.octoprint_print_progress");

    std::string completionDate = entities.Take<std::string>("sensor.octoprint_approximate_completion_time");
    completionTime = completionDate.substr(completionDate.length() - 8, 5);
    printTimeTotal = entities.Take<std::string>("sensor.octoprint_approximate_total_print_time");
    printTimeDone = entities.Take<std::string>("sensor.octoprint_print_time");
    printTimeLeft = entities.Take<std::string>("sensor.octoprint_print_time_left");

    socTemp = entities.Take<float>("sensor.octoprint_soc_temperature");
    bedTarget = entities.Take<float>("sensor.octoprint_bed_target");
    bedTemp = entities.Take<float>("sensor.octoprint_bed_temperature");
    toolTarget = entities.Take<float>("sensor.octoprint_tool_0_target");
    toolTemp = entities.Take<float>("sensor.octoprint_tool_0_temperature");
}
