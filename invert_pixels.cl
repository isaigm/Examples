__kernel void invert_pixels(__global unsigned char *pixels, int width)
{
    int row = get_global_id(0);
   
    
    int start = 0;
    int end = width - 1;

    while(start < end)
    {
        int r1 = pixels[(start + row * width) * 4];
        int g1 = pixels[(start + row * width) * 4 + 1];
        int b1 = pixels[(start + row * width) * 4 + 2];
        
        
        int r2 = pixels[(end + row * width) * 4];
        int g2 = pixels[(end + row * width) * 4 + 1];
        int b2 = pixels[(end + row * width) * 4 + 2];

        pixels[(start + row * width) * 4] = r2;
        pixels[(start + row * width) * 4 + 1] = g2;
        pixels[(start + row * width) * 4 + 2] = b2;

        pixels[(end + row * width) * 4] = r1;
        pixels[(end + row * width) * 4 + 1] = g1;
        pixels[(end + row * width) * 4 + 2] = b1;
        

        start++;
        end--;
    }
 
}
