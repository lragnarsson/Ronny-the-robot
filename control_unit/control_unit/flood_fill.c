/*
 * flood_fill.c
 *
 */ 

#include <avr/io.h>

#include "flood_fill.h"

uint8_t current_wavefront_size;
uint8_t new_wavefront_size;
coordinate wavefront_1[100];
coordinate wavefront_2[100];
coordinate* current_wavefront = &wavefront_1[0];
coordinate* new_wavefront = &wavefront_2[0];

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
			ff_map[i][j] = FF_DONT_CARE;
}

void calculate_route(coordinate destination)
{
	coordinate current_coordinate = destination;
	current_route[ff_map[destination.x][destination.y]] = ROUTE_END; 
	for(uint8_t i = ff_map[destination.x][destination.y] - 1; i != 255; --i)
	{
		switch(current_route[i+1]) // Run straight ahead if possible
		{
			case NORTH:
				if(ff_map[current_coordinate.x + 1][current_coordinate.y] == i)
				{
					current_route[i] = NORTH;
					++current_coordinate.x;
					continue;
				}
				break;
			case EAST:
				if(ff_map[current_coordinate.x][current_coordinate.y - 1] == i)
				{
					current_route[i] = EAST;
					--current_coordinate.y;
					continue;
				}
				break;
			case SOUTH:
				if(ff_map[current_coordinate.x - 1][current_coordinate.y] == i)
				{
					current_route[i] = SOUTH;
					--current_coordinate.x;
					continue;
				}
				break;
			case WEST:
				if(ff_map[current_coordinate.x][current_coordinate.y + 1] == i)
				{
					current_route[i] = WEST;
					++current_coordinate.y;
					continue;
				}
				break;
			default:
				break;
		}
		
		if(ff_map[current_coordinate.x + 1][current_coordinate.y] == i) // GO NORTH?
		{
			current_route[i] = NORTH;
			++current_coordinate.x;	
		} else if(ff_map[current_coordinate.x][current_coordinate.y - 1] == i) // etc.
		{
			current_route[i] = EAST;
			--current_coordinate.y;
		} else if(ff_map[current_coordinate.x - 1][current_coordinate.y] == i)
		{
			current_route[i] = SOUTH;
			--current_coordinate.x;
		} else if(ff_map[current_coordinate.x][current_coordinate.y + 1] == i)
		{
			current_route[i] = WEST;
			++current_coordinate.y;
		}
	}
}

void flood_fill_to_destination(coordinate destination) {
	reset_flood_fill_values();
	*current_wavefront = current_position;
	current_wavefront_size = 1;
	
	uint8_t distance = 0;
	while(1)
	{
		new_wavefront_size = 0;
		for(int i = 0; i < current_wavefront_size; ++i)
		{
			ff_map[(current_wavefront + i)->x][(current_wavefront + i)->y] = distance;
			if((current_wavefront + i)->x == destination.x && (current_wavefront + i)->y == destination.y)
			{
				calculate_route(destination);
				return;
			}
			
			if(map[(current_wavefront + i)->x - 1][(current_wavefront + i)->y] == NOT_WALL) // NORTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x - 1, (current_wavefront + i)->y};
				++new_wavefront_size;		
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y + 1] == NOT_WALL) // EAST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y + 1};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x + 1][(current_wavefront + i)->y] == NOT_WALL) // SOUTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x + 1, (current_wavefront + i)->y};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y - 1] == NOT_WALL) // WEST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y - 1};
				++new_wavefront_size;
			}
		}
		if(new_wavefront_size == 0) // Route to destination not found
		{
			current_route[0] = ROUTE_END;
			return;
		}
		++distance;
		current_wavefront_size = new_wavefront_size;
		swap_wavefronts(&current_wavefront, &new_wavefront);
	}
}

void flood_fill_to_unmapped() {
	reset_flood_fill_values();
	*current_wavefront = current_position;
	current_wavefront_size = 1;
	
	uint8_t distance = 0;
	while(1)
	{
		new_wavefront_size = 0;
		for(int i = 0; i < current_wavefront_size; ++i)
		{
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y] == UNMAPPED)
			{
				ff_map[(current_wavefront + i)->x][(current_wavefront + i)->y] = distance;
				calculate_route((coordinate){ (current_wavefront + i)->x, (current_wavefront + i)->y });
				return;
			}
			ff_map[(current_wavefront + i)->x][(current_wavefront + i)->y] = distance;
			
			if(map[(current_wavefront + i)->x - 1][(current_wavefront + i)->y] == NOT_WALL || map[(current_wavefront + i)->x - 1][(current_wavefront + i)->y] == UNMAPPED) // NORTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x - 1, (current_wavefront + i)->y};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y + 1] == NOT_WALL || map[(current_wavefront + i)->x][(current_wavefront + i)->y + 1] == UNMAPPED) // EAST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y + 1};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x + 1][(current_wavefront + i)->y] == NOT_WALL || map[(current_wavefront + i)->x + 1][(current_wavefront + i)->y] == UNMAPPED) // SOUTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x + 1, (current_wavefront + i)->y};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y - 1] == NOT_WALL || map[(current_wavefront + i)->x][(current_wavefront + i)->y - 1] == UNMAPPED) // WEST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y - 1};
				++new_wavefront_size;
			}
		}
		if(new_wavefront_size == 0) // Route to destination not found
		{
			current_route[0] = ROUTE_END;
			return;
		}
		++distance;
		current_wavefront_size = new_wavefront_size;
		swap_wavefronts(&current_wavefront, &new_wavefront);				
	}
}

void flood_fill_home_optimistic() {
	reset_flood_fill_values();
	*current_wavefront = current_position;
	current_wavefront_size = 1;
	coordinate destination = start_position;
	uint8_t distance = 0;
	while(1)
	{
		new_wavefront_size = 0;
		for(int i = 0; i < current_wavefront_size; ++i)
		{
			ff_map[(current_wavefront + i)->x][(current_wavefront + i)->y] = distance;
			if((current_wavefront + i)->x == destination.x && (current_wavefront + i)->y == destination.y)
			{
				calculate_route(destination);
				return;
			}
			
			if(map[(current_wavefront + i)->x - 1][(current_wavefront + i)->y] == NOT_WALL || map[(current_wavefront + i)->x - 1][(current_wavefront + i)->y] == UNMAPPED) // NORTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x - 1, (current_wavefront + i)->y};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y + 1] == NOT_WALL || map[(current_wavefront + i)->x][(current_wavefront + i)->y + 1] == UNMAPPED) // EAST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y + 1};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x + 1][(current_wavefront + i)->y] == NOT_WALL || map[(current_wavefront + i)->x + 1][(current_wavefront + i)->y] == UNMAPPED) // SOUTH
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x + 1, (current_wavefront + i)->y};
				++new_wavefront_size;
			}
			if(map[(current_wavefront + i)->x][(current_wavefront + i)->y - 1] == NOT_WALL || map[(current_wavefront + i)->x][(current_wavefront + i)->y - 1] == UNMAPPED) // WEST
			{
				*(new_wavefront + new_wavefront_size) = (coordinate){(current_wavefront + i)->x, (current_wavefront + i)->y - 1};
				++new_wavefront_size;
			}
		}
		if(new_wavefront_size == 0) // Route to destination not found
		{
			current_route[0] = ROUTE_END;
			return;
		}
		++distance;
		current_wavefront_size = new_wavefront_size;
		swap_wavefronts(&current_wavefront, &new_wavefront);
	}
}