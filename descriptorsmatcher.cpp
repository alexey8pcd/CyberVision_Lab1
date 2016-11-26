#include "descriptorsmatcher.h"

vector<pair<Descriptor<DESC_SIZE>, Descriptor<DESC_SIZE> > >
associate(vector<Descriptor<DESC_SIZE> >& descriptors1,
        vector<Descriptor<DESC_SIZE> >& descriptors2)
{
    vector<pair<Descriptor<DESC_SIZE>,
            Descriptor<DESC_SIZE>>> pairs =
            vector<pair<Descriptor<DESC_SIZE>, Descriptor<DESC_SIZE>>>();
    int count = min(descriptors1.size(), descriptors2.size());
    if(count == 0){
        return pairs;
    }
    vector<int> nearest12 = vector<int>();
    vector<float> minDistance12 = vector<float>();
    for (int i = 0; i < descriptors2.size(); ++i) {
        minDistance12.push_back(0.);
        nearest12.push_back(0);
    }

    vector<int> nearest21 = vector<int>();
    vector<float> minDistance21 = vector<float>();
    for (int i = 0; i < descriptors1.size(); ++i) {
        minDistance21.push_back(0.);
        nearest21.push_back(0);
    }

    for (int i = 0; i < descriptors1.size() && descriptors2.size() > 0; ++i) {
        Descriptor<DESC_SIZE> desc1 = descriptors1.at(i);
        float min = desc1.getDistance(descriptors2.at(0));
        nearest12[i] = 0;
        minDistance12[i] = min;
        for (int j = 1; j < descriptors2.size(); ++j) {
            Descriptor<DESC_SIZE> desc2 = descriptors2.at(j);
            float distance12 = desc1.getDistance(desc2);
            if (distance12 < min) {
                min = distance12;
                nearest12[i] = j;
                minDistance12[i] = min;
            }
        }
    }
//    qDebug() << "----";
//    for (int i = 0; i < descriptors2.size(); ++i) {
//        qDebug() << nearest12[i] << " " << minDistance12[i];
//    }
    for (int i = 0; i < descriptors2.size() && descriptors1.size() > 0; ++i) {
        Descriptor<DESC_SIZE> desc2 = descriptors2.at(i);
        float min = desc2.getDistance(descriptors1.at(0));
        nearest21[i] = 0;
        minDistance21[i] = min;
        for (int j = 1; j < descriptors1.size(); ++j) {
            Descriptor<DESC_SIZE> desc1 = descriptors1.at(j);
            float distance21 = desc2.getDistance(desc1);
            if (distance21 < min) {
                min = distance21;
                nearest21[i] = j;
                minDistance21[i] = min;
            }
        }
    }
//    qDebug() << "----";
//    for (int i = 0; i < descriptors1.size(); ++i) {
//        qDebug() << nearest21[i] << " " << minDistance21[i];
//    }
    float bigValue = 1e6;
    while (count > 0) {
        float min = bigValue;
        int i1 = -1;
        int i2 = -1;
        for (int i = 0; i < descriptors1.size(); ++i) {
            int minIndex = nearest12[i];
            if (minIndex != -1){
                float distance = minDistance12[i];
                if(distance < min){
                    min = distance;
                    i1 = i;
                }
            }
        }
        for (int i = 0; i < descriptors2.size(); ++i) {
            int minIndex = nearest21[i];
            if (minIndex != -1){
                float distance = minDistance21[i];
                if(distance < min){
                    min = distance;
                    i1 = -1;
                    i2 = i;
                }
            }
        }
        if(i1 != -1){
            Descriptor<DESC_SIZE> d1 = descriptors1.at(i1);
            int n12 = nearest12[i1];
            Descriptor<DESC_SIZE> d2 = descriptors2.at(n12);
            pair<Descriptor<DESC_SIZE>, Descriptor<DESC_SIZE>> p(d1, d2);
            pairs.push_back(p);
            --count;
            nearest21[n12] = -1;
            nearest12[i1] = -1;
        } else if (i2 != -1) {
            Descriptor<DESC_SIZE> d1 = descriptors2.at(i2);
            int n21 = nearest21[i2];
            Descriptor<DESC_SIZE> d2 = descriptors1.at(n21);
            pair<Descriptor<DESC_SIZE>, Descriptor<DESC_SIZE>> p(d1, d2);
            pairs.push_back(p);
            --count;
            nearest12[n21] = -1;
            nearest21[i2] = -1;
        }
    }
    return pairs;
}

