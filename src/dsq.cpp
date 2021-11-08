#include "dsq.hpp"

float hmEarth[16] = {
    0.05, 235.0/255.0, 49.0/255.0, 16.0/255.0, //Red
    0.2, 10.0/255.0, 10.0/255.0, 0.0, //Black
    0.6, 90.0/255.0, 160.0/255.0, 70.0/255.0, //Green
    0.9, 200.0/255.0, 210.0/255.0, 190.0/255.0 //White
};

float test[16] = {
    0.05, 235.0/255.0, 49.0/255.0, 16.0/255.0, //Red
    0.2, 10.0/255.0, 10.0/255.0, 0.0, //Black
    0.6, 90.0/255.0, 160.0/255.0, 70.0/255.0, //Green
    0.9, 200.0/255.0, 210.0/255.0, 190.0/255.0 //White
};

/**
 * @brief 
 * 
 * @param val current value
 * @param mode percent, 'r', 'g', 'b'
 * @param board array fill with rgb from min to max
 * @param size array lines
 * @param min minimmum value
 * @param max maximum value
 * @return float 
 */
float heatMap(float val, char mode, float* board, int size, float min, float max)
{
    int idx = 0, modeVal;
    val -= min;
    val /= (max-min);
    while(board[4*idx]<val && idx<size)
        idx++;
    switch(mode)
    {
        case 'r':
            modeVal = 1;
            break;
        case 'g':
            modeVal = 2;
            break;
        case 'b':
            modeVal = 3;
            break;
    }
    if(idx == 0 || idx == size)
    {
        if(idx) idx--;
        return board[(4*idx)+modeVal];
    }
    else 
    {
        float a = (board[(4*idx)+modeVal]-board[(4*(idx-1))+modeVal])/(board[(4*idx)]-board[(4*(idx-1))]);
        return (val-board[(4*(idx-1))])*a+board[(4*(idx-1))+modeVal];
    }
}

std::vector<float> mergeSort(std::vector<float> f)
{
    int s = f.size();
    if(s<=1) return f;
    else
    {
        int step = 1, av, i, k1, k2, lim;
        std::vector<float> z;
        while(pow(2, step-1)< s)
        {
            av = pow(2, step);
            i = 0;
            z={};
            while(i<s)
            {
                if((s-i)<av && (s-i)%av)
                {
                    k1 = 0; k2 = 0; lim;
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
                    k1 = 0; k2 = 0;
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
    int edge = size/2;
    float* K = new float [fieldSize*fieldSize]();
    std::vector<float> v, k;

    for(int i = 0; i<fieldSize*fieldSize; i++)
        mean += M[i];
    mean /= fieldSize*fieldSize;
    

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
    int max = 0, min = 0, mod, dv;
    float* M = new float [6*size*size]();

    for(int i = 0; i<size*size; i++)
    {
        if(diamond[i]>max) max = diamond[i];
        else if(diamond[i]<min) min = diamond[i];
    }
    
    for(int i = 0; i<6*size*size; i++)
    {
        mod = i%6;
        dv = int(i/6);
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
            case 3:
                M[i] = heatMap(diamond[dv], 'r', hmEarth, 4, min, max);
                break;
            case 4:
                M[i] = heatMap(diamond[dv], 'g', hmEarth, 4, min, max);
                break;
            case 5:
                M[i] = heatMap(diamond[dv], 'b', hmEarth, 4, min, max);
                break;
        }
    }
    return M;
}

unsigned int* MapIndices(int size)
{
    int s = 6*pow(size-1, 2), squareIdx;
    unsigned int* M = new unsigned int [s]();

    for(int i = 0; i<size-1; i++)
    {
       for(int j = 0; j<size-1; j++)
        {
            squareIdx = (j+(size-1)*i);
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

/**
 * @brief DiamondSquare algorithm
 * 
 * @param size Square border size (Must be odd number)
 * @param number Number of square on a border
 * @param smoothness [0 1]. Percentage of amplitude decrease
 * @param filter Odd number for low pass filter
 * @return float* 2D map of altitude values
 */
float* diamondSquare(int size, int number, float smoothness, int filter)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1, 1);
    float* M = new float [size*size*number*number]();
    int xNumb, yNumb, i, id, shift, somme, n;
    float moyenne;
    
    for(int numb = 0; numb<pow(number, 2); numb++)
    {
        xNumb = size*(numb/number);
        yNumb = size*(numb%number);
        
        //Left up
        M[(xNumb*size*number)+yNumb] = float(size*dis(gen));
        //Right up
        M[(xNumb*size*number)+yNumb+size-1] = float(size*dis(gen));
        //Left down
        M[((xNumb+size-1)*size*number)+yNumb] = float(size*dis(gen));
        //Right down
        M[((xNumb+size-1)*size*number)+yNumb+size-1] = float(size*dis(gen));

        if(numb)
        {
            if(numb/number) //If not in first row, apply upper edge value from previous row
            { 
                for(int edge = 0; edge<size; edge++)
                {   
                    M[(xNumb*size*number)+yNumb+edge] = M[((xNumb-1)*size*number)+yNumb+edge];
                }
            }
            if(numb%number) //If not first column, apply left edge previous right edge
            {
                for(int edge = 0; edge<size; edge++)
                {   
                    M[((xNumb+edge)*size*number)+yNumb] = M[((xNumb+edge)*size*number)+yNumb-1];
                }
            }
        }
        i = size - 1;
        while(i>1)
        {
            id = i/2;
            for(int x = id; x < size; x+=i)
            {
                for(int y = id; y < size; y+=i)
                {
                    moyenne = (M[((size)*number*(xNumb + x - id))+ yNumb + y - id] 
                                    + M[((size)*number*(xNumb + x - id))+ yNumb + y + id] 
                                    + M[((size)*number*(xNumb + x + id))+ yNumb + y + id] 
                                    + M[((size)*number*(xNumb + x + id))+ yNumb + y - id]) / 4.0;
                    if(M[((size)*number*(x + xNumb)) + yNumb + y]==0)
                        M[((size)*number*(x + xNumb)) + yNumb + y] = moyenne + (float(id*dis(gen)))*smoothness;
                }
            }
            shift = 0;
            for(int x = 0; x < size; x+=id)
            {
                if(!shift) shift = id;
                else shift = 0;
                for(int y = shift; y < size; y+=i)
                {
                    somme = 0; n = 0;
                    if(x>=id)
                    {
                        somme += M[((size)*number*(xNumb + x - id))+ yNumb + y];
                        n++;
                    }
                    if(x+id<size)
                    {
                        somme += M[((size)*number*(xNumb + x + id)) + yNumb + y];
                        n++;
                    }
                    if(y>=id)
                    {
                        somme += M[((size)*number*(x + xNumb)) + yNumb + y - id];
                        n++;
                    }
                    if(y+id<size)
                    {
                        somme += M[((size)*number*(x + xNumb)) + yNumb + y + id];
                        n++;
                    }
                    if(M[((size)*number*(x + xNumb)) + yNumb + y]==0)
                        M[((size)*number*(x + xNumb)) + yNumb + y] = (float(somme) / float(n)) + (float(id*dis(gen)))*smoothness;
                }
                
            }
            i = id;
        }
    }
    M[0] = 100;
    M[size*size*number*number-1] = -100;
    //return medianFilter(M, size*number, filter);
    return M;
}




