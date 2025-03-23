#ifndef MAPGEN_STATE_HPP
#define MAPGEN_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

/**
 * Enum for easy state representation.
 */
enum MapgenStateName {
    WATER,  // 0
    LAND,   // 1
    FOREST, // 2
    DESERT  // 3
};


/**
 * Atomic model cell state variables.
 */
struct MapgenState {
    /**
     * Members
     */
    int terrain;

    /**
     * Constructor
     */
	MapgenState() : terrain(MapgenStateName::WATER) {}
};

/**
 * operator<< overload definition.
 *
 * Defines the output format of the MapgenState enum class in order to
 * print the cell state.
 *  <0> : WATER
 *  <1> : LAND
 *  <2> : FOREST
 *  <3> : DESERT
 */
std::ostream& operator<<(std::ostream& os, const MapgenState& s) {
    os << "<" << s.terrain << ">";
    return os;
}

/**
 * operator!= overload definition.
 *
 * Enables simulator to check for equality between two state objects.
 */
bool operator!=(const MapgenState& x, const MapgenState& y) {
	return x.terrain != y.terrain;
}

/**
 * Parse a JSON file to populate a cell's state.
 */
void from_json(const nlohmann::json& j, MapgenState& s) {
    // terrain
	j.at("terrain").get_to(s.terrain);
}

#endif // MAPGEN_STATE_HPP
