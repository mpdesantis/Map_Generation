#ifndef MAPGEN_CELL_HPP
#define MAPGEN_CELL_HPP

// Standard
#include <cmath>
#include <random>

// Other
#include <nlohmann/json.hpp>

// Cadmium V2
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>

// Models
#include "mapgenState.hpp"

using namespace cadmium::celldevs;


/**
 * Atomic Model Cell Definition
 */
class mapgen : public GridCell<MapgenState, double> {

    private:

    /**
     * Constants
     */

    // Delay time units
    static constexpr double DEFAULT_DELAY_TIME = 1.00;

    // LAND constants
    static constexpr int LAND_BIRTH_LIMIT = 3;
    static constexpr int LAND_DEATH_LIMIT = 2;

    // FOREST constants
    static constexpr int FOREST_BIRTH_LIMIT = 5;
    static constexpr int FOREST_DEATH_LIMIT = 2;

    // DESERT constants
    static constexpr int DESERT_BIRTH_LIMIT = 8;
    static constexpr int DESERT_DEATH_LIMIT = 4;
    static constexpr double DESERT_BASE_RATE = 0.3;
    static constexpr double DESERT_MULTIPLIER = 2.0;


    public:

    /**
     * Constructor
     */
    mapgen(const std::vector<int>& id, 
            const std::shared_ptr<const GridCellConfig<MapgenState, double>>& config
          ): GridCell<MapgenState, double>(id, config) { }

    /**
     * Local Computation Function (tau)
     */
    [[nodiscard]] MapgenState localComputation(MapgenState state,
        const std::unordered_map<std::vector<int>, NeighborData<MapgenState, double>>& neighborhood) const override {

        /* Local Variables */

        // Accumulators for all neighbor types (differentiated by terrain)
        int water_neighbors = 0;
        int land_neighbors = 0;
        int forest_neighbors = 0;
        int desert_neighbors = 0;
        int non_water_neighbors = 0;

        /* Canvas the Neighborhood */

        // Canvas this cell's neighborhood to categorize neighbor types
        // ie. tally neighbor cells by terrain type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            // State of the neighbor cell for this iteration
            auto nState = neighborData.state;

            // WATER neighbors
            if(nState->terrain == MapgenStateName::WATER) {
                water_neighbors++;
            }
            // LAND neighbors
            if(nState->terrain == MapgenStateName::LAND) {
                land_neighbors++;
            }
            // FOREST neighbors
            if(nState->terrain == MapgenStateName::FOREST) {
                forest_neighbors++;
            }
            // DESERT neighbors
            if(nState->terrain == MapgenStateName::DESERT) {
                desert_neighbors++;
            }
        }

        /* Mutate State Based on Rules and Return */

        // Case: WATER cell
        if(state.terrain == MapgenStateName::WATER) {
            // Uncount this cell from its own neighborhood tally
            water_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: WATER --> LAND
            if(non_water_neighbors < LAND_BIRTH_LIMIT) {
                // A LAND cell is born
                state.terrain = MapgenStateName::LAND; 
            }
        } 

        // Case: LAND cell
        else if (state.terrain == MapgenStateName::LAND) {
            // Uncount this cell from its own neighborhood tally
            land_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: LAND --> WATER
            if(non_water_neighbors < LAND_DEATH_LIMIT) {
                // A LAND cell dies
                state.terrain = MapgenStateName::WATER;
            }

            // Case: LAND --> FOREST
            else if(non_water_neighbors < FOREST_BIRTH_LIMIT) {
                // A FOREST cell is born
                state.terrain = MapgenStateName::FOREST;
            }

            // Case: LAND --> DESERT | LAND
            else {
                // Get random number in [0, 1] to test against DESERT rules
                double r = randomProbability();
                // Get base threshold for becoming DESERT
                double desert_threshold = DESERT_BASE_RATE;
                // Apply multiplier to increase chance of becoming DESERT
                // if near WATER.
                if (water_neighbors) {
                    desert_threshold *= DESERT_MULTIPLIER;
                }
                // Case: This cell is below threshold, so it becomes DESERT
                if (r <= desert_threshold) {
                    // A DESERT cell is born
                    state.terrain = MapgenStateName::DESERT;
                }
                // Case: This cell exceeds threshold, so it remains LAND
                else {
                    // Cell remains as LAND
                }
            }

            // Case: LAND --> LAND
            //else {
            //    // Cell remains as LAND
            //}
        }

        // Case: FOREST cell
        else if (state.terrain == MapgenStateName::FOREST) {
            // Uncount this cell from its own neighborhood tally
            forest_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: FOREST --> LAND
            if(non_water_neighbors < FOREST_DEATH_LIMIT) {
                // A FOREST cell reverts to LAND
                state.terrain = MapgenStateName::LAND;
            }

            // Case: FOREST --> FOREST
            else {
                // Cell remains as FOREST
            }
        }

        // Case: DESERT cell
        else if (state.terrain == MapgenStateName::DESERT) {
            // Uncount this cell from its own neighborhood tally
            desert_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: DESERT --> LAND
            if(non_water_neighbors < DESERT_DEATH_LIMIT) {
                // A DESERT cell reverts to LAND
                state.terrain = MapgenStateName::LAND;
            }

            // Case: DESERT --> DESERT
            else {
                // Cell remains as DESERT
            }
            
        }

        return state;
    }

    /**
     * Delay function (D)
     */
    [[nodiscard]] double outputDelay(const MapgenState& state) const override {
        return DEFAULT_DELAY_TIME;
    }

    /**
     * Get a random double within defined limits
     */
    double randomProbability() const {     
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }
};

#endif // MAPGEN_CELL_HPP
