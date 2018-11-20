#pragma once

#include "crc64.h"
#include "stdio.h"

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

int vectorIncrement(char *address)
{
	crc64_vector_item *item = NULL;
	if (vectorContains(address, &item))
	{
		item->counter++;

		return 0;
	} 

	return vectorAdd(address);
}

int createVector(unsigned int capacity)
{
	if (vector)
		return -1;

	vector = (crc64_vector *)malloc(sizeof(crc64_vector));
	vector->items = (crc64_vector_item *)malloc(sizeof(crc64_vector_item) * capacity);
	vector->capacity = capacity;
	vector->count = 0;

	return 0;
}

int destroyVector()
{
	if (!vector)
		return -1;

	free(vector);
	vector = NULL;

	return 0;
}

int vectorAdd(const char *address)
{
	crc64_vector_item *item;
	if (vectorContains(address, item) == 0)
	{
		unsigned long long crc = crc64(0, (const unsigned char*)address, strlen(address));
		vector->items[vector->count].checksum = crc;
		memcpy(&vector->items[vector->count].name, address, strlen(address)); 
		vector->count++;

		vectorSort();

		return 0;
	}

	return -1;
}

int vectorPrint()
{
	for (int i = 0; i < vector->count; i++)
	{
		fprintf(stdout, "%s\t%lx\t%ld\n", vector->items[i].name, vector->items[i].checksum, vector->items[i].counter);
	}

	return 0;
}

int vectorCompare(const void * a, const void * b)
{
	const crc64_vector_item ai = *(const crc64_vector_item*)a;
	const crc64_vector_item bi = *(const crc64_vector_item*)b;

	if (ai.checksum < bi.checksum)
	{
		return -1;
	}
	else if (ai.checksum > bi.checksum)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void vectorSort()
{
	qsort(vector->items, vector->count, sizeof(crc64_vector_item), vectorCompare);
}


int vectorContains(const unsigned char *address, crc64_vector_item **found)
{
	if (!vector)
		return -1;

	unsigned long long crc = crc64(0, address, strlen(address));

	unsigned int lowerbound = 0;
	unsigned int upperbound = vector->count;
	unsigned int position = 0;

	if (upperbound < 2)
	{
		if (upperbound == 1 && vector->items[position].checksum == crc)
		{
			*found = &vector->items[position];
			return 1;
		}

		return 0;
	}

	position = (lowerbound + upperbound) / 2;

	while ((vector->items[position].checksum != crc) && (lowerbound <= upperbound))
	{
		if (vector->items[position].checksum > crc)
		{
			if (position == 0)
				break;

			upperbound = position - 1;
		}
		else
		{
			lowerbound = position + 1;
		}
		position = (lowerbound + upperbound) / 2;
	}

	if (lowerbound <= upperbound)
	{
		*found = &vector->items[position];
	}

	if (position == 0)
	{
		return vector->items[position].checksum == crc;
	}

	return (lowerbound <= upperbound);
}

