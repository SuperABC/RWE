#include "Sgs/interpreter/sgsmem.h"

SgsMemory::SgsMemory() {
	localMem = new MemBlock();
	globeMem = new MemBlock();
}
SgsMemory::~SgsMemory() {
	MemBlock *tmp;
	tmp = localMem->next;
	while (localMem) {
		for (int i = 0; i < localMem->offset; i++)delete localMem->cont[i];
		delete localMem;
		localMem = tmp;
		if (tmp)tmp = tmp->next;
	}
	tmp = globeMem->next;
	while (globeMem) {
		for (int i = 0; i < globeMem->offset; i++)delete globeMem->cont[i];
		delete globeMem;
		globeMem = tmp;
		if (tmp)tmp = tmp->next;
	}
}

void *SgsMemory::alloc(unsigned int size, bool local) {
	MemBlock *tmp;
	if(local)tmp = localMem;
	else tmp = globeMem;

	while (tmp->offset == 255) {
		if(!tmp->next)
			tmp->next = new MemBlock();
		tmp = tmp->next;
	}

	tmp->cont[tmp->offset] = new char[size];
	return tmp->cont[tmp->offset++];
}
void *SgsMemory::alloc(void *ptr, bool local) {
	MemBlock *tmp;
	if (local)tmp = localMem;
	else tmp = globeMem;

	while (tmp->offset == 255) {
		if (!tmp->next)
			tmp->next = new MemBlock();
		tmp = tmp->next;
	}

	tmp->cont[tmp->offset] = ptr;
	return tmp->cont[tmp->offset++];
}
void SgsMemory::clear() {
	MemBlock *tmp;
	tmp = localMem->next;
	while (localMem) {
		for (int i = 0; i < localMem->offset; i++)delete localMem->cont[i];
		delete localMem;
		localMem = tmp;
		if (tmp)tmp = tmp->next;
	}
	localMem = new MemBlock();
}