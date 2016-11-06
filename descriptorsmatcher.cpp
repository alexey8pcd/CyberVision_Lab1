#include "descriptorsmatcher.h"

vector<pair<Descriptor<SIFT_DESC_SIZE>, Descriptor<SIFT_DESC_SIZE> > >
associate(vector<Descriptor<SIFT_DESC_SIZE> >& descriptors1,
        vector<Descriptor<SIFT_DESC_SIZE> >& descriptors2)
{
    vector<pair<Descriptor<SIFT_DESC_SIZE>,
            Descriptor<SIFT_DESC_SIZE>>> pairs =
            vector<pair<Descriptor<SIFT_DESC_SIZE>, Descriptor<SIFT_DESC_SIZE>>>();

    if(!descriptors1.empty() && !descriptors2.empty()){
        for (int i = 0; i < descriptors1.size(); ++i) {
            if(descriptors2.empty()){
                break;
            }
            Descriptor<SIFT_DESC_SIZE> descriptor = descriptors1.at(i);
            int nearestIndex = 0;
            Descriptor<SIFT_DESC_SIZE> nearest = descriptors2.at(nearestIndex);
            float min = descriptor.getDistance(nearest);
            for (int j = 1; j < descriptors2.size(); ++j) {
                Descriptor<SIFT_DESC_SIZE> descriptor2 = descriptors2.at(j);
                float distance = descriptor.getDistance(descriptor2);
                if(distance < min){
                    nearest = descriptor2;
                    min = distance;
                    nearestIndex = j;
                }
            }
            pair<Descriptor<SIFT_DESC_SIZE>,
                    Descriptor<SIFT_DESC_SIZE>> p(descriptor, nearest);
            descriptors2.erase(descriptors2.begin() + nearestIndex);
            pairs.push_back(p);
        }
    }
    return pairs;
}

