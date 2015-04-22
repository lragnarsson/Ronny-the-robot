/*
 * flood_fill.c
 *
 */ 

#include <avr/io.h>

#include "flood_fill.h"

uint8_t current_wavefront_size;
uint8_t new_wavefront_size;
coordinate current_wavefront[100];
coordinate new_wavefront[100];

void swap_wavefronts(coordinate **wavefront1, coordinate **wavefront2)
{
	coordinate *temp = *wavefront1;
	*wavefront1 = *wavefront2;
	*wavefront2 = temp;
}

void reset_flood_fill_values()
{
	for (int i = 0; i < MAP_SIZE; ++i)
		for(int j = 0; j < MAP_SIZE; ++j)
			if(map[i][j] != UNMAPPED && map[i][j] != WALL)
				map[i][j] = NOT_WALL;
}

void calculate_route(coordinate destination)
{
	coordinate current_coordinate = destination;
	current_route[map[destination.x][destination.y]] = ROUTE_END; 
	for(uint8_t i = map[destination.x][destination.y] - 1; i != 255; --i)
	{
		switch(current_route[i+1]) // Run straight ahead if possible
		{
			case NORTH:
				if(map[current_coordinate.x + 1][current_coordinate.y] == i)
				{
					current_route[i] = NORTH;
					++current_coordinate.x;
					continue;
				}
				break;
			case EAST:
				if(map[current_coordinate.x][current_coordinate.y - 1] == i)
				{
					current_route[i] = EAST;
					--current_coordinate.y;
					continue;
				}
				break;
			case SOUTH:
				if(map[current_coordinate.x - 1][current_coordinate.y] == i)
				{
					current_route[i] = SOUTH;
					--current_coordinate.x;
					continue;
				}
				break;
			case WEST:
				if(map[current_coordinate.x][current_coordinate.y + 1] == i)
				{
					current_route[i] = WEST;
					++current_coordinate.y;
					continue;
				}
				break;
			default:
				break;
		}
		
		if(map[current_coordinate.x + 1][current_coordinate.y] == i) // GO NORTH?
		{
			current_route[i] = NORTH;
			++current_coordinate.x;	
		} else if(map[current_coordinate.x][current_coordinate.y - 1] == i) // etc.
		{
			current_route[i] = EAST;
			--current_coordinate.y;
		} else if(map[current_coordinate.x - 1][current_coordinate.y] == i)
		{
			current_route[i] = SOUTH;
			--current_coordinate.x;
		} else if(map[current_coordinate.x][current_coordinate.y + 1] == i)
		{
			current_route[i] = WEST;
			++current_coordinate.y;
		}
	}
}

void flood_fill_to_destination(coordinate destination) {
	reset_flood_fill_values();
	current_wavefront[0] = current_position;
	current_wavefront_size = 1;
	
	uint8_t distance = 0;
	while(1)
	{
		new_wavefront_size = 0;
		for(int i = 0; i < current_wavefront_size; ++i)
		{
			map[current_wavefront[i].x][current_wavefront[i].y] = distance;
			if(current_wavefront[i].x == destination.x && current_wavefront[i].y == destination.y)
			{
				calculate_route(destination);
				return;
			}
			
			if(map[current_wavefront[i].x - 1][current_wavefront[i].y] == NOT_WALL) // NORTH
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x - 1, current_wavefront[i].y};
				++new_wavefront_size;		
			}
			if(map[current_wavefront[i].x][current_wavefront[i].y + 1] == NOT_WALL) // EAST
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x, current_wavefront[i].y + 1};
				++new_wavefront_size;
			}
			if(map[current_wavefront[i].x + 1][current_wavefront[i].y] == NOT_WALL) // SOUTH
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x + 1, current_wavefront[i].y};
				++new_wavefront_size;
			}
			if(map[current_wavefront[i].x][current_wavefront[i].y - 1] == NOT_WALL) // WEST
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x, current_wavefront[i].y - 1};
				++new_wavefront_size;
			}
		}
		if(new_wavefront_size == 0) // Route to destination not found
		{
			current_route[0] = ROUTE_END;
			return;
		}
		++distance;
		swap_wavefronts(&current_wavefront, &new_wavefront);
	}
}

void flood_fill_to_unmapped() {
	reset_flood_fill_values();
	current_wavefront[0] = current_position;
	current_wavefront_size = 1;
	
	uint8_t distance = 0;
	while(1)
	{
		new_wavefront_size = 0;
		for(int i = 0; i < current_wavefront_size; ++i)
		{
			if(map[current_wavefront[i].x][current_wavefront[i].y] == UNMAPPED)
			{
				map[current_wavefront[i].x][current_wavefront[i].y] = distance;
				calculate_route((coordinate){ current_wavefront[i].x, current_wavefront[i].y });
				return;
			}
			map[current_wavefront[i].x][current_wavefront[i].y] = distance;
			
			if(map[current_wavefront[i].x - 1][current_wavefront[i].y] == NOT_WALL || map[current_wavefront[i].x - 1][current_wavefront[i].y] == UNMAPPED) // NORTH
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x - 1, current_wavefront[i].y};
				++new_wavefront_size;
			}
			if(map[current_wavefront[i].x][current_wavefront[i].y + 1] == NOT_WALL || map[current_wavefront[i].x][current_wavefront[i].y + 1] == UNMAPPED) // EAST
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x, current_wavefront[i].y + 1};
				++new_wavefront_size;
			}
			if(map[current_wavefront[i].x + 1][current_wavefront[i].y] == NOT_WALL || map[current_wavefront[i].x + 1][current_wavefront[i].y] == UNMAPPED) // SOUTH
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x + 1, current_wavefront[i].y};
				++new_wavefront_size;
			}
			if(map[current_wavefront[i].x][current_wavefront[i].y - 1] == NOT_WALL || map[current_wavefront[i].x][current_wavefront[i].y - 1] == UNMAPPED) // WEST
			{
				new_wavefront[new_wavefront_size] = (coordinate){current_wavefront[i].x, current_wavefront[i].y - 1};
				++new_wavefront_size;
			}
		}
		if(new_wavefront_size == 0) // Route to destination not found
		{
			current_route[0] = ROUTE_END;
			return;
		}
		++distance;
		swap_wavefronts(&current_wavefront, &new_wavefront);				
	}
}

void flood_fill_home_optimistic() {
	//TODO: copy and modify other flood fill.
}