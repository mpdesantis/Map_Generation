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
            if(non_water_neighbors > state.land_birth_limit) {
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
            if(non_water_neighbors < state.land_death_limit) {
                // A LAND cell dies
                state.terrain = MapgenStateName::WATER;
            }

            // Case: LAND --> DESERT | LAND
            // DESERT can only form near WATER (ie. at least one WATER neighbor)
            else if (water_neighbors) {
                // Get random number in [0, 1] to test against DESERT rules
                double r = randomProbability();
                // Get base threshold for becoming DESERT
                double desert_threshold = state.desert_base_rate;
                // DESERT is more likely to form near WATER.
                // Apply multiplier to increase chance of becoming DESERT
                // based on number of WATER neighbours
                desert_threshold += state.desert_multiplier * water_neighbors;
                // Case: This cell is below threshold, so it becomes DESERT
                if (r <= desert_threshold) {
                    // A DESERT cell is born
                    state.terrain = MapgenStateName::DESERT;
                }
                // Case: This cell remains LAND
            }

            // Case: LAND --> FOREST | LAND
            // Forests only grow in interior (ie. no WATER neighbors)
            else {
                // Get random number in [0, 1] to test against FOREST rules
                double r = randomProbability();
                // Get base threshold for becoming DESERT
                double forest_threshold = state.forest_base_rate;
                // FOREST is more likely to form near FOREST.
                // Apply multiplier to increase chance of becoming FOREST
                // based on number of FOREST neighbors.
                forest_threshold += state.forest_multiplier * forest_neighbors;
                // Case: This cell is below threshold, so it becomes DESERT
                if (r <= forest_threshold) {
                    // A FOREST cell is born
                    state.terrain = MapgenStateName::FOREST;
                }
                // Case: This cell remains LAND
            }

        }

        // Case: FOREST cell
        // FOREST can only revert to LAND
        else if (state.terrain == MapgenStateName::FOREST) {
            // Uncount this cell from its own neighborhood tally
            forest_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: FOREST --> LAND
            // If too much WATER nearby, revert to LAND
            if(water_neighbors > state.forest_death_limit) {
                // A FOREST cell reverts to LAND
                state.terrain = MapgenStateName::LAND;
            }
        }

        // Case: DESERT cell
        // DESERT can only revert to LAND
        else if (state.terrain == MapgenStateName::DESERT) {
            // Uncount this cell from its own neighborhood tally
            desert_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + desert_neighbors;

            // Case: DESERT --> LAND
            // If not enough WATER nearby, revert to LAND
            if(water_neighbors < state.desert_death_limit) {
                // A DESERT cell reverts to LAND
                state.terrain = MapgenStateName::LAND;
            }
        }

        // Case: LAND --> LAND

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
