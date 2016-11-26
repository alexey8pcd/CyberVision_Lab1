#ifndef DESCRIPTORSMATCHER_H
#define DESCRIPTORSMATCHER_H
#include "vector"
#include "descriptor.h"
#include "descriptorssearcher.h"
using namespace std;

vector<pair<Descriptor<DESC_SIZE>,
Descriptor<DESC_SIZE>>>
associate(vector<Descriptor<DESC_SIZE>> &descriptors1,
          vector<Descriptor<DESC_SIZE>> &descriptors2);

#endif // DESCRIPTORSMATCHER_H
