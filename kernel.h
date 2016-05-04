#ifndef KERNEL_H
#define KERNEL_H


class Kernel
{
    private:
        int * coefficients;
        int size;
    public:
        Kernel();
        int getIntensity(int i, int j);
        int getSize();
        void setCoefficients(int * coefficients);
        void toSobelX();
        void toSobelY();
        void setSize(int size);
};

#endif // KERNEL_H
