#ifndef POOL_LINKER_H
#define POOL_LINKER_H

class PLinker {
public:
	void* mData;
	PLinker* mNextLink;
};

#endif // POOL_LINKER_H