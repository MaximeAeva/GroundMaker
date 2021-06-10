#include "dsq.hpp"

float mappedVert[] = {
        // positions         // colors
         0.0f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  // bottom right
         0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.0f  // top 
    };

unsigned int mappedIdx[] = {  // Notons que l’on commence à 0!
0, 1, 2,   // premier triangle
2, 3, 1   // second triangle
};

std::vector<float> mergeSort(std::vector<float> f)
{
    int s = f.size();
    if(s<=1) return f;
    else
    {
        int step = 1;
        std::vector<float> z;
        while(pow(2, step-1)< s)
        {
            int av = pow(2, step);
            int i = 0;
            z={};
            while(i<s)
            {
                if((s-i)<av && (s-i)%av)
                {
                    int k1 = 0, k2 = 0, lim;
                    if(pow(2, step-1)<(s-i)) lim = pow(2, step-1);
                    else lim = (s-i);
                    while(k1<lim || k2<(s-i-lim))
                    {
                        if(k1==lim)
                        {
                            z.push_back(f[i+pow(2, step-1)+k2]);
                            k2++;
                        }
                        else if(k2==(s-i-lim))
                        {
                            z.push_back(f[i+k1]);
                            k1++;
                        }
                        else if(f[i+k1] >= f[i+pow(2, step-1)+k2])
                        {
                            z.push_back(f[i+pow(2, step-1)+k2]);
                            k2++;
                        }
                        else if(f[i+k1] < f[i+pow(2, step-1)+k2])
                        {
                            z.push_back(f[i+k1]);
                            k1++;
                        }
                    }

                }
                else
                {
                    int k1 = 0, k2 = 0;
                    while(k1<pow(2, step-1) || k2<pow(2, step-1))
                    {
                        if(k1==pow(2, step-1))
                        {
                            z.push_back(f[i+pow(2, step-1)+k2]);
                            k2++;
                        }
                        else if(k2==pow(2, step-1))
                        {
                            z.push_back(f[i+k1]);
                            k1++;
                        }
                        else if(f[i+k1] >= f[i+pow(2, step-1)+k2])
                        {
                            z.push_back(f[i+pow(2, step-1)+k2]);
                            k2++;
                        }
                        else if(f[i+k1] < f[i+pow(2, step-1)+k2])
                        {
                            z.push_back(f[i+k1]);
                            k1++;
                        }
                    }
                }
                i+=av;
            }
            step++;
            f={};
            f = z;
        }
        return f;
    }
}

float* medianFilter(float* M, int fieldSize, int size)
{
    float mean = 0;
    for(int i = 0; i<fieldSize*fieldSize; i++)
        mean += M[i];
    mean /= fieldSize*fieldSize;
    int edge = size/2;
    float* K = new float [fieldSize*fieldSize]();
    std::vector<float> v;
    std::vector<float> k;
    for(int i = 0; i<fieldSize; i++)
    {
        for(int j = 0; j<fieldSize; j++)
        {
            v.clear();
            for(int k = -edge; k<edge; k++)
            {
                for(int l = -edge; l<edge; l++)
                {
                    //mean padding
                    if((i+k)<0 || (j+l)<0 || (i+k)>fieldSize-1 || (j+l)>fieldSize-1)
                        v.push_back(mean);
                    else
                        v.push_back(M[((i+k)*fieldSize)+(j+l)]);
                }
            }
            k = mergeSort(v);
            K[(i*fieldSize)+j] = k[pow(edge, 2)+1];
        }
    }
    return K;
}

float* MapVertices(int size, float step, float* diamond)
{
    int max = 0, min = 0;
    for(int i = 0; i<size*size; i++)
    {
        if(diamond[i]>max) max = diamond[i];
        else if(diamond[i]<min) min = diamond[i];
    }
    float* M = new float [6*size*size]();
    for(int i = 0; i<6*size*size; i++)
    {
        int mod = i%6;
        int dv = int(i/6);
        switch(mod)
        {
            case 2://x
                M[i]=float(step*(dv%size));
                break;
            case 0://y
                M[i]=float(step*(dv/size));
                break;
            case 1://z
                M[i]=diamond[dv]; //float((rand() % 3)-1.5);
                break;
            case 4:
                M[i] = (M[i-3]-min)/(max-min);
                break;
            case 3:
            case 5:
                M[i] = 0.0f;
                break;
        }
    }
    return M;
}

unsigned int* MapIndices(int size)
{
    int s = 6*pow(size-1, 2);
    unsigned int* M = new unsigned int [s]();
    for(int i = 0; i<size-1; i++)
    {
       for(int j = 0; j<size-1; j++)
        {
            int squareIdx = (j+(size-1)*i);
            if(squareIdx%2)
            {
            M[6*squareIdx] = squareIdx + i;
            M[(6*squareIdx)+1] = M[6*squareIdx] + 1;
            M[(6*squareIdx)+2] = M[6*squareIdx] + size;
            M[(6*squareIdx)+3] = M[6*squareIdx] + size;
            M[(6*squareIdx)+4] = M[6*squareIdx] + size + 1;
            M[(6*squareIdx)+5] = M[6*squareIdx] + 1;
            }
            else
            {
            M[6*squareIdx] = squareIdx + i;
            M[(6*squareIdx)+1] = M[6*squareIdx] + size;
            M[(6*squareIdx)+2] = M[6*squareIdx] + size + 1;
            M[(6*squareIdx)+3] = M[6*squareIdx] + size + 1;
            M[(6*squareIdx)+4] = squareIdx + i;
            M[(6*squareIdx)+5] = squareIdx + i + 1;
            }
        }
    }
    return M;
}

float* diamondSquare(int size, float smoothness, int filter)
{
    float* M = new float [size*size]();
    M[0] = float(((rand() % 100)/(2*size))-size);
    M[size-1] = float(((rand() % 100)/(2*size))-size);
    M[size*(size-1)] = float(((rand() % 100)/(2*size))-size);
    M[(size*size)-1] = float(((rand() % 100)/(2*size))-size);
    
    int i = size - 1;
    while(i>1)
    {
        int id = i/2;
        for(int x = id; x < size; x+=i)
        {
            for(int y = id; y < size; y+=i)
            {
                float moyenne = (M[(size*(x - id))+y - id] + M[(size*(x - id))+y + id] + M[(size*(x + id))+y + id] + M[(size*(x + id))+y - id]) / 4.0;
                M[(size*x)+y] = moyenne + (((rand() % 100)/(2*id))-id)*smoothness;
            }
        }
        int shift = 0;
        for(int x = 0; x < size; x+=id)
        {
            if(!shift) shift = id;
            else shift = 0;
            for(int y = shift; y < size; y+=i)
            {
                int somme = 0, n = 0;
                if(x>=id)
                {
                    somme += M[(size*(x - id))+ y];
                    n++;
                }
                if(x+id<size)
                {
                    somme += M[(size*(x + id)) + y];
                    n++;
                }
                if(y>=id)
                {
                    somme += M[(size*x) + y - id];
                    n++;
                }
                if(y+id<size)
                {
                    somme += M[(size*x) + y + id];
                    n++;
                }
                M[(size*x) + y] = (float(somme) / float(n)) + (((rand() % 100)/(2*id))-id)*smoothness;
            }
            
        }
        i = id;
    }
    return medianFilter(M, size, 2*filter+1);
}




