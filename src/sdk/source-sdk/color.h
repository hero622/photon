#pragma once

#include <cstdint>

struct color_t {
	uint8_t r, g, b, a;

	color_t( ) {}
	color_t( uint8_t r, uint8_t g, uint8_t b ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}
	color_t( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
