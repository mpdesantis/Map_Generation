#include "nlohmann/json.hpp"
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <chrono>
#include <fstream>
#include <string>
#include "include/conwayCell.hpp"

using namespace cadmium::celldevs;
using namespace cadmium;

std::shared_ptr<GridCell<conwayState, double>> addGridCell(const coordinates & cellId, const std::shared_ptr<const GridCellConfig<conwayState, double>>& cellConfig) {
	auto cellModel = cellConfig->cellModel;

	if (cellModel == "conway") {
		return std::make_shared<conway>(cellId, cellConfig);
	} else {
		throw std::bad_typeid();
	}
}

int main(int argc, char ** argv) {

    /* Constants and Defaults */
    const std::string DEFAULT_OUTPUT_FILE_PATH = "./output/map_generation.csv";
    constexpr int DEFAULT_SIM_TIME = 500;
    const std::string DEFAULT_CONFIG_FILE_PATH = "./config/map_generation.json";

    /* Argument Parsing */
//	if (argc < 2) {
//		std::cout << "Program used with wrong parameters. The program must be invoked as follows:";
//		std::cout << argv[0] << " SCENARIO_CONFIG.json [MAX_SIMULATION_TIME (default: 500)]" << std::endl;
//		return -1;
//	}

    // Set file path
	//std::string configFilePath = argv[1];
	std::string configFilePath = (argc > 1) ? argv[1] : DEFAULT_CONFIG_FILE_PATH;

    // Set simulation duration
	double simTime = (argc > 2) ? std::stod(argv[2]) : DEFAULT_SIM_TIME;

    // Set output path
	std::string outputFilePath = (argc > 3) ? argv[3] : DEFAULT_OUTPUT_FILE_PATH;

    /* Models */

	auto model = std::make_shared<GridCellDEVSCoupled<conwayState, double>>("conway", addGridCell, configFilePath);
	model->buildModel();
	
	auto rootCoordinator = RootCoordinator(model);
	//rootCoordinator.setLogger<CSVLogger>("conway_log.csv", ";");
	rootCoordinator.setLogger<CSVLogger>(outputFilePath, ";");
	
    // Execute the simulation
	rootCoordinator.start();
	rootCoordinator.simulate(simTime);
	rootCoordinator.stop();
    return 0;
}
