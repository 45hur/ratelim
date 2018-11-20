#pragma once

#include <stdint.h>

typedef struct
{
	uint64_t checksum;
	uint64_t counter;
	char name[16];
} crc64_vector_item;

typedef struct
{
	crc64_vector_item *items;
	unsigned int capacity;
	unsigned int count;
} crc64_vector;

crc64_vector *vector;

int createVector(unsigned int capacity);
int destroyVector();
int vectorAdd(const char *address);
int vectorCompare(const void * a, const void * b);
int vectorContains(const char *address, crc64_vector_item **found);
int vectorIncrement(char *address);
int vectorPrint();
int vectorSort();