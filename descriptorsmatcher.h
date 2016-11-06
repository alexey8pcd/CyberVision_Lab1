#ifndef DESCRIPTORSMATCHER_H
#define DESCRIPTORSMATCHER_H
#include "vector"
#include "descriptor.h"
#include "siftdescriptorssearcher.h"
using namespace std;

vector<pair<Descriptor<SIFT_DESC_SIZE>,
Descriptor<SIFT_DESC_SIZE>>>
associate(vector<Descriptor<SIFT_DESC_SIZE>> &descriptors1,
          vector<Descriptor<SIFT_DESC_SIZE>> &descriptors2);

#endif // DESCRIPTORSMATCHER_H
