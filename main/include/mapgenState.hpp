#ifndef MAPGEN_STATE_HPP
#define MAPGEN_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

//! Conway game of life cell state.
struct mapgenState {
	bool life;

	//! Default constructor function.
	mapgenState() : life(false) {}
};

//! It prints the life of the cell in an output stream.
std::ostream& operator<<(std::ostream& os, const mapgenState& x) {
	os << "<" << ((x.life)? 1 : 0) << ">";
	return os;
}

//! The simulator must be able to compare the equality of two state objects
bool operator!=(const mapgenState& x, const mapgenState& y) {
	return x.life != y.life;
}

//! It parses a JSON file and generates the corresponding mapgenState object.
void from_json(const nlohmann::json& j, mapgenState& s) {
	j.at("life").get_to(s.life);
}

#endif // MAPGEN_STATE_HPP
