__kernel void blur(__global unsigned char *pixels, global unsigned char *outputPixels, int width, int height, int k)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    float avgr  = 0;
    float avgg = 0;
    float avgb  = 0;
    
    int n = 0;
    for(int y = i - k; y <= i + k; y++)
    {
        for(int x = j - k; x <= j + k; x++)
        {
            if(x == j && y == i) continue;
            if(x < 0 || x >= width || y < 0 || y >= height) continue;
            avgr +=  (pixels[(x + y * width) * 4]);
            avgg +=  (pixels[(x + y * width) * 4 + 1]);
            avgb +=  (pixels[(x + y * width) * 4 + 2]);
            n++;
        }
    }
    avgr /= (float) n;
    avgg /= (float) n;
    avgb /= (float) n;
    outputPixels[(j + i * width) * 4] = (int) avgr;
    outputPixels[(j + i * width) * 4 + 1] = (int) avgg;
    outputPixels[(j + i * width) * 4 + 2] = (int) avgb;
    outputPixels[(j + i * width) * 4 + 3] = 0xFF;
}
